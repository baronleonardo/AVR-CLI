#ifndef COMMANDS_H
#define COMMANDS_H

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

int led( char* cmd, char* args );
int delayMS( char* cmd, char* args );
int read( char* cmd, char* args );
int write( char* cmd, char* args );

#ifdef __cplusplus
}
#endif

#endif // COMMANDS_H
