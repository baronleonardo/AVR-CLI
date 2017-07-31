#include "CLI.h"
#include <string.h>
#include "wserial.h"

// 26 represents the 26 letters in the english alphabets
#define ALPHA_LEN 26
#define MAX_CMD_LEN 100

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
      NULL
    },

    {
        "delay",
        "Desc: delay for X milisecond\n"
        "Usage: delay <milisecond>",
        delayMS,
        NULL
    },
};

static int8_t CLI_cmdTree[ALPHA_LEN];
static char __cmd[MAX_CMD_LEN];

static Command* __getCmd(char* cmd_name);

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
    wSerial_println( "Welcome ...\n\n" );

    while( true )
    {
        wSerial_print("> ");
        wSerial_mirrorReceivedDataUntil('\n', __cmd, MAX_CMD_LEN);

        CLI_execCmd( __cmd );
    }
}

int CLI_execCmd( char* cmd )
{
    char* args = cmd;
    Command* cmd_obj;

    cmd = strtok(args, " ");
    args = strtok(NULL, "");

    cmd_obj = __getCmd(cmd);

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
    int8_t cmdIndex = CLI_cmdTree[cmd_name[0] - 'a'];
    Command* command_obj;

    if( cmdIndex == -1 )
        return NULL;
    else if( strcmp(Commands[cmdIndex].cmd, cmd_name) == 0 )
        return &Commands[cmdIndex];
    else if( Commands[cmdIndex].next == NULL )
        return NULL;
    else
    {
        while( ((command_obj = Commands[cmdIndex].next) != NULL) &&
               (strcmp(command_obj->cmd, cmd_name) == 0) );

        return command_obj;
    }
}
