#ifndef CLI_H_
#define CLI_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int Var_getInt( char var );
float Var_getFloat( char var );
char Var_getChar( char var );
const char *Var_getStr( char var );

bool Var_setInt( char var, int data );
bool Var_setFloat( char var, float data );
bool Var_setChar( char var, char c );
bool Var_setStr( char var, char data[], uint8_t data_len );

/* Private prototypes */
void __Var_init();

#ifdef __cplusplus
}
#endif

#endif // CLI_H_
