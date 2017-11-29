#include "__CLI.h"
#include <string.h>
#include <stdbool.h>
#include "wserial.h"

static int8_t CLI_cmdTree[ALPHA_LEN];
typedef struct Input {
    bool hasMultipleCmd;
    char data[CMD_MAX_LEN];
} Input;
static Input __input;
// TODO: design new command system
// static char __cmd[MAX_CMD_LEN];
// static char __variables[ALPHA_LEN][MAX_VAR_LEN];

static Command currentCmd;

/*************************** Functions Prototypes ***************************/
static CommandNode* __CLI_getCmd( char* cmd_name );
static char* __has_args(char* cmd);
static Command* __CLI_prepCmd( char* cmd, Command* preparedCmd );
/****************************************************************************/

void __CLI_init( uint32_t baudrate )
{
    // TODO: need testing

    size_t cmds_len = __get_Commands_count();
    int8_t cmdTreeIndex;
    CommandNode* cmd;

    // init the tree with `-1`
    memset( CLI_cmdTree, -1, ALPHA_LEN );

    for( int iii = 0; iii < cmds_len; ++iii )
    {
        cmdTreeIndex = Commands[iii].cmd_name[0] - 'a';

        if( CLI_cmdTree[cmdTreeIndex] == -1 )
            CLI_cmdTree[cmdTreeIndex] = iii;

        else
        {
            cmd = &Commands[ CLI_cmdTree[cmdTreeIndex] ];

            while( (cmd = cmd->next) != NULL );
            cmd->next = &Commands[iii];
        }
    }

    __input.hasMultipleCmd = false;

    currentCmd.param_len = 0;

    // init Serial
    wSerial_begin( baudrate );
    // wait for Serial initiation
    while( wSerial_availableForWrite() == 0 );
}

void __CLI_mainLoop()
{
    char c;
    uint8_t counter = 0;
    char* token;
    char* delem;
    Command *cmd;

    wSerial_println( "~~~ Welcome ... ~~~\n\n" );

    while( true )
    {
        cmd = &currentCmd;
        currentCmd.param_len = 0;
        counter = 0;
        __input.hasMultipleCmd = false;

        wSerial_print("> ");

        // receiving cmd
        while( counter < CMD_MAX_LEN - 1 )
        {
            c = wSerial_read();

            if( c > 0 )
            {
                wSerial_printChar(c);

                __input.data[counter++] = c;

                if( c == ';' ) __input.hasMultipleCmd = true;

                else if( c == '\r' ) counter--;
                else if( c == '\n') break;
            }
        }

        if( counter == CMD_MAX_LEN - 1 )
            continue;

        __input.data[counter - 1] = '\0';

        // processing cmd
        delem = token = __input.data;

        // tokenize input to multiple cmds if input has `;`
        if(__input.hasMultipleCmd) {
            while( ( delem = strchr(token, ';') ) != NULL )
            {
                *delem = '\0';

                cmd = __CLI_prepCmd( token, cmd );
                if( cmd )
                    __CLI_exec( cmd );
                else
                    wSerial_println("Invalid command");

                token = delem + 1;
            }
        }

        cmd = __CLI_prepCmd( token, cmd );
        if( cmd != NULL ) {
            __CLI_exec( cmd );
        }
        else
            wSerial_println("Invalid command");
    }
}

int __CLI_exec( Command* cmd )
{
    return cmd->cmd->cmd_func( cmd->cmd->cmd_name,
                               cmd->params,
                               cmd->param_len );
}

CommandNode *__CLI_getCmd(char* cmd_name)
{
    if( cmd_name[0] - 'a' < 0 )
        return NULL;

    CommandNode* command_node;
    int8_t cmd_index = CLI_cmdTree[cmd_name[0] - 'a'];

    if( cmd_index == -1 )
        return NULL;
    else if( strcmp(Commands[cmd_index].cmd_name, cmd_name) == 0 )
        return &Commands[cmd_index];
    else if( Commands[cmd_index].next == NULL )
        return NULL;
    else {
        while( ((command_node = Commands[cmd_index].next) != NULL) &&
               (strcmp(command_node->cmd_name, cmd_name) == 0) );

        return command_node;
    }
}

char* __has_args(char* cmd) {
    // dectect presence of args
    for( uint8_t iii = 0; iii <= VAR_MAX_LEN && cmd[iii] != '\0'; ++iii ) {
        if( cmd[iii] == ' ' ) {
            return &cmd[iii + 1];
        }
    }

    return NULL;
}

Command* __CLI_prepCmd( char* cmd, Command* preparedCmd ) {
//    char* cmd_name;
    char* args = NULL;
    CommandNode* cmd_node;
//    int8_t first_arg_index = -1;
    char* var = NULL;

    // get rid of spaces in the begining
    while( *cmd == ' ' ) cmd++;

    // dectect presence of args
    // if so, separate cmd name from its args
    if( (args = __has_args(cmd)) != NULL )
        *(args - 1) = '\0';

    // check database for the presence of this cmd name
    cmd_node = __CLI_getCmd(cmd);

    if( cmd_node == NULL )
        return NULL;

    // tokenize args to an array of args
    if( args != NULL ) {
        // search for variables
        preparedCmd->params[preparedCmd->param_len++] = args;

        for( var = strchr( args, ' ' );
             var != NULL;
             var = strchr( var+1, ' ' ) )
        {
            *var = '\0';

            if( *(var+1) != ' ' )
                preparedCmd->params[preparedCmd->param_len++] = var + 1;
        }
    }

    preparedCmd->cmd = cmd_node;

    return preparedCmd;
}

/**************************** System commands ****************************/
// 26 available variables has 10 bytes value max

//int set( char* cmd, char* args )
//{
//    args = strtok(args, " \n\t\r");
//    int8_t var_index = ASCII_TO_INDEX(args[0]);

//    strncpy( __variables[ var_index ],
//             strtok(NULL, " \n\t\r"),
//             MAX_VAR_LEN - 1 );

//    // terminate the value in case the value entered is larger than 10 bytes
//    // in this case it will be terminated
//    __variables[ var_index ][ MAX_VAR_LEN - 1 ] = '\0';

//    return 0;
//}
