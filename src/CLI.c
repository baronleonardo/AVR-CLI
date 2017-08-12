#include "CLI.h"
#include <string.h>
#include "wserial.h"
#include <stdbool.h>

// 26 represents the 26 letters in the english alphabets
#define ALPHA_LEN 26
#define MAX_CMD_LEN 100
#define MAX_VAR_LEN 10
#define ASCII_TO_INDEX(ascii) (ascii - 'a')

typedef int (*cmd_func_t)(char* cmd, char* args);

typedef struct Command {
    char* cmd;
    char* description;
    cmd_func_t cmd_func;
    struct Command* next;
} Command;

static Command Commands[] = {
    { "led",
      "Desc: control the red led state (ON/OFF) on pin 13\n"
      "Usage: led [-i] <ON|OFF>",
      led,
      NULL },

    { "delay",
      "Desc: delay for X milisecond\n"
      "Usage: delay <milisecond>",
      delayMS,
      NULL },

    { "set",
      "Desc: create a variable and assign a value to it\n"
      "Usage: set <variable> <value>\n"
      "NOTE: variables are only one letter a, or b, or c, ...",
      set,
      NULL },

    { "print",
      "Desc: print a string or a variable content\n"
      "Usage: print <variable>|<string>",
      print,
      NULL },

    { "write",
      "Desc: write analog/digital data to <pin>\n"
      "      d -> digital, a -> analog\n"
      "Usage: write d|a[0-99] <value>\n"
      "Example: write d10 1",
      write,
      NULL },

    { "read",
      "Desc: read analog/digital data from <pin>\n"
      "      d -> digital, a -> analog\n"
      "Usage: read d|a[0-99]\n"
      "Example: read d10",
      read,
      NULL },
};

static int8_t CLI_cmdTree[ALPHA_LEN];
static char __input[MAX_CMD_LEN];
static char __cmd[MAX_CMD_LEN];
static char __variables[ALPHA_LEN][MAX_VAR_LEN];

static Command* __getCmd(char* cmd_name);
static char* CLI_prepCmd(char* cmd, char* preparedCmd);

void CLI_init( uint32_t baudrate )
{
    // TODO: need testing

    size_t cmds_len = sizeof(Commands) / sizeof(Commands[0]);
    int8_t cmdTreeIndex;
    Command* cmd;

    // init the tree with `-1`
    memset( CLI_cmdTree, -1, ALPHA_LEN);

    for( int iii = 0; iii < cmds_len; ++iii )
    {
        cmdTreeIndex = Commands[iii].cmd[0] - 'a';

        if( CLI_cmdTree[cmdTreeIndex] == -1 )
            CLI_cmdTree[cmdTreeIndex] = iii;

        else
        {
            cmd = &Commands[ CLI_cmdTree[cmdTreeIndex] ];

            while( (cmd = cmd->next) != NULL );
            cmd->next = &Commands[iii];
        }
    }

    // init Serial
    wSerial_begin( baudrate );
    // wait for Serial initiation
    while( wSerial_availableForWrite() == 0 );
}

void CLI_exec()
{
    char c;
    uint8_t counter;
    char* token;
    char* delem;

    wSerial_println( "~~~ Welcome ... ~~~\n\n" );

    while( true )
    {
        counter = 0;

        wSerial_print("> ");

        while( counter < MAX_CMD_LEN - 1 )
        {
            c = wSerial_read();

            if( c > 0 )
            {
                wSerial_printChar(c);

                __input[counter++] = c;

                if( c == '\n' ) break;
            }
        }

        __input[counter] = '\0';

        if( counter < MAX_CMD_LEN - 1)
        {
            delem = token = __input;

            while( (delem = strchr(token, ';')) != NULL )
            {
                *delem = '\0';
                CLI_execCmd( CLI_prepCmd( token, __cmd ) );
                token = delem + 1;
            }

            CLI_execCmd( CLI_prepCmd( token, __cmd ) );
        }
    }
}

int CLI_execCmd( char* cmd )
{
    // wSerial_println(cmd);

    char* cmd_name;
    char* args;
    Command* cmd_obj;
	char* var = NULL;
	char* varValue;

    cmd_name = cmd;
    args = strchr(cmd, ' ');

    if( args == NULL ) return -1;

    *args++ = '\0';

    // cmd_name = strtok(cmd, " ");
    // args = strtok(NULL, " ");

	var = strchr( args, '$' );

	if( var != NULL )
	{
		varValue = __variables[ ASCII_TO_INDEX(var[1]) ];
		// clear the variable representation from cmd (like: $x)
		var[0] = var[1] = ' ';
		strncpy(var, varValue, strlen(varValue));
	}

    cmd_obj = __getCmd(cmd_name);

    if( cmd_obj == NULL )
    {
        wSerial_println("Invalid command");
        return -1;
    }

    if( strstr(args, "-h") != NULL )
    {
        wSerial_println(cmd_obj->description);
        return 0;
    }

    // find the cmd command func to execute it
    return cmd_obj->cmd_func(cmd, args);
}

Command* __getCmd(char* cmd_name)
{
    if( cmd_name[0] - 'a' < 0 )
        return NULL;

    int8_t innerCmd = CLI_cmdTree[cmd_name[0] - 'a'];
    Command* command_obj;

    if( innerCmd == -1 )
        return NULL;
    else if( strcmp(Commands[innerCmd].cmd, cmd_name) == 0 )
        return &Commands[innerCmd];
    else if( Commands[innerCmd].next == NULL )
        return NULL;
    else
    {
        while( ((command_obj = Commands[innerCmd].next) != NULL) &&
               (strcmp(command_obj->cmd, cmd_name) == 0) );

        return command_obj;
    }
}

char* CLI_prepCmd( char* cmd, char* preparedCmd )
{
    char* var;
    uint8_t varLen;
    char* __preparedCmd = preparedCmd;
    char* innerCmdHead;
    char* innerCmdTail;
    int cmdReturnValue;

    // get rid of spaces in the begining
    while( *cmd == ' ' ) cmd++;

    for( ; *cmd != '\0'; preparedCmd++, cmd++ )
    {
        if( *cmd == '$' )
        {
            var = __variables[ ASCII_TO_INDEX(*(cmd + 1)) ];
            varLen = strlen(var);
            strncpy( preparedCmd, var, varLen );

            // move cmd to the first space after `var`
            cmd = strchr( cmd, ' ' );
        }

        else if( *cmd == '`' )
        {
            // get the beginnging and the ending of the inner cmd
            innerCmdHead = cmd; 
            innerCmdTail = strchr( innerCmdHead + 1, '`' );

            *innerCmdTail = '\0';
            cmdReturnValue = CLI_execCmd( CLI_prepCmd( innerCmdHead + 1, preparedCmd ) );
            itoa( cmdReturnValue, preparedCmd , 10 );

            // update cmd, preparedCmd
            cmd  = innerCmdTail;
            preparedCmd = strchr( preparedCmd, '\0' );
            continue;
        }

        *preparedCmd = *cmd;
    }

    *preparedCmd = '\0';

    return __preparedCmd;
}

/**************************** System commands ****************************/
// 26 available variables has 10 bytes value max

int set( char* cmd, char* args )
{
    args = strtok(args, " \n\t\r");
    int8_t var_index = ASCII_TO_INDEX(args[0]);

    strncpy( __variables[ var_index ],
             strtok(NULL, " \n\t\r"),
             MAX_VAR_LEN - 1 );

    // terminate the value in case the value entered is larger than 10 bytes
    // in this case it will be terminated
    __variables[ var_index ][ MAX_VAR_LEN - 1 ] = '\0';

    return 0;
}

int print( char* cmd, char* args )
{
    wSerial_print(args);

    return 0;
}
