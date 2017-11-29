#ifndef __CLI_H
#define __CLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "commands.h"
#include "definitions.h"

typedef struct Command {
    CommandNode* cmd;
    int8_t param_len;
    char* params[MAX_PARAMS];
} Command;

void __CLI_init( uint32_t baudrate );
void __CLI_mainLoop();
int  __CLI_exec( Command *cmd );

#ifdef __cplusplus
}
#endif

#endif // __CLI_H
