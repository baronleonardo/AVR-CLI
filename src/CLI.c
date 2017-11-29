#include "CLI.h"
#include "definitions.h"
#include <stdlib.h>
#include <string.h>

#define ASCII_TO_INDEX(ascii) (ascii - 'a')

static char __variables[ALPHA_LEN][VAR_MAX_LEN];

static bool __validVar( char var );

bool Var_isVar( char* input ) {
    if( input[0] == '$' )
        return true;
    else
        return false;
}

char Var_getVarFromArg( char* input ) {
    if( input[0] == '$' )
        return input[1];
    else
        return 0;
}

int Var_getInt( char var ) {
    // TODO: we need better design for validating var
    if( !__validVar(var) ) return -1;

    return atoi( __variables[ASCII_TO_INDEX(var)] );
}

float Var_getFloat( char var ) {
    // TODO: we need better design for validating var
    if( !__validVar(var) ) return -1;

    return atof( __variables[ASCII_TO_INDEX(var)] );
}

char Var_getChar( char var ) {
    // TODO: we need better design for validating var
    if( !__validVar(var) ) return 0;

    return __variables[ASCII_TO_INDEX(var)][0];
}

const char* Var_getStr( char var ) {
    // TODO: we need better design for validating var
    if( !__validVar(var) ) return NULL;

    return __variables[ASCII_TO_INDEX(var)];
}


bool Var_setInt( char var, int data ) {
    char* result;

    // validate `var`
    if( !__validVar(var) ) return false;

    result = itoa(data, __variables[ASCII_TO_INDEX(var)], 10);

    if( !result )
        return false;
    else
        return true;
}

bool Var_setFloat( char var, float data ) {
    char* result;

    // validate `var`
    if( !__validVar(var) ) return false;

    result = dtostrf( data,
                      VAR_MAX_LEN,
                      FLOAT_PRECITION,
                      __variables[ASCII_TO_INDEX(var)] );

    if( !result )
        return false;
    else
        return true;
}

bool Var_setChar( char var, char c ) {

    // validate `var`
    if( !__validVar(var) ) return false;

    __variables[ASCII_TO_INDEX(var)][0] = c;

    return true;
}

bool Var_setStr( char var, char data[], uint8_t data_len ) {
    char* result;

    if( data_len > 10 )
        return false;

    // validate `var`
    if( !__validVar(var) ) return false;

    result = memcpy( __variables[ASCII_TO_INDEX(var)], data, data_len );

    if( !result )
        return false;
    else
        return true;
}

/************************* Private functions *************************/
bool __validVar( char var ) {
    if( var >= 'a' && var <= 'z' ) return true;
    else return false;
}
