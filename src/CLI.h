#ifndef CLI_H_
#define CLI_H_

#include <stdint.h>
#include "Arduino.h"
#include "commands.h"

#ifdef __cplusplus
extern "C" {
#endif

void CLI_init( uint32_t baudrate );
void CLI_exec();
int CLI_execCmd( char* cmd );

#ifdef __cplusplus
}
#endif

#endif // CLI_H_
