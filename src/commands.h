#ifndef COMMANDS_H
#define COMMANDS_H

#include "Arduino.h"
#include "definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CommandNode {
    char* cmd_name;
    // char* description;
    cmd_func_t cmd_func;
    struct CommandNode* next;
} CommandNode;

extern CommandNode Commands[];

int led( char*, char**, int8_t );
int delayMS( char*, char**, int8_t );
int read( char*, char**, int8_t );
int write( char*, char**, int8_t );
//int set(char*, char**, int8_t);
int print(char*, char**, int8_t);

/* Private functions */
size_t __get_Commands_count();

#ifdef __cplusplus
}
#endif

#endif // COMMANDS_H
