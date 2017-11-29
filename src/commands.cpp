#include "commands.h"
#include "wserial.h"
#include "CLI.h"

#define RED_LED 13

CommandNode Commands[] = {
    { "led",
      // "Desc: control the red led state (ON/OFF) on pin 13\n"
      // "Usage: led [-i] <ON|OFF>",
      led,
      NULL },

    { "delay",
      // "Desc: delay for X milisecond\n"
      // "Usage: delay <milisecond>",
      delayMS,
      NULL },

    { "set",
      // "Desc: create a variable and assign a value to it\n"
      // "Usage: set <variable> <value>\n"
      // "NOTE: variables are only one letter a, or b, or c, ...",
      set,
      NULL },

    { "print",
      // "Desc: print a string or a variable content\n"
      // "Usage: print <variable>|<string>",
      print,
      NULL },

    { "write",
      // "Desc: write analog/digital data to <pin>\n"
      // "      d -> digital, a -> analog\n"
      // "Usage: write d|a[0-99] <value>\n"
      // "Example: write d10 1",
      write,
      NULL },

    { "read",
      // "Desc: read analog/digital data from <pin>\n"
      // "      d -> digital, a -> analog\n"
      // "Usage: read d|a[0-99]\n"
      // "Example: read d10",
      read,
      NULL },
};

size_t __get_Commands_count() {
    return sizeof(Commands) / sizeof(Commands[0]);
}

/****************************************************************************/

int led( char* cmd_name, char **args , int8_t args_len )
{
    uint8_t pinStateIndex = 0;

    // `-i` means initiate
    if( args[0][0] == '-' ) {
        pinStateIndex = 1;

        if( args[0][1] == 'i' )
            pinMode(RED_LED, OUTPUT);
        else
            return -1;
    }

    if( strcmp(args[pinStateIndex], "ON") == 0 )
        digitalWrite(RED_LED, HIGH);

    else if( strcmp(args[pinStateIndex], "OFF") == 0 )
        digitalWrite(RED_LED, LOW);

    else {
        Serial.println("Wrong arguments!");
        return -1;
    }

    return 0;
}

int delayMS( char* cmd_name, char** args, int8_t args_len )
{
    int amount;

    // check if there is any variable
    if( !Var_isVar(args[0]) )
        amount = atoi( args[0] );
    else
        amount = Var_getInt( Var_getVarFromArg(args[0]) );

    if( amount != -1 )
        delay( amount );

    else {
        Serial.println("Wrong arguments!");
        return -1;
    }

    return 0;
}

int read( char* cmd_name, char** args, int8_t args_len )
{
    // TODO: add args for initiate pin before read
    uint8_t pinNum;
    uint8_t pinState;
    uint8_t var;

    // check if there is any variable
    if( !Var_isVar(args[1]) )
        return -1;
    else
        var = Var_getVarFromArg(args[1]);
   
    pinNum = atoi( &args[0][1] );
    
    // This is digital
    if( args[0][0] == 'd' )
        pinState = digitalRead(pinNum);

    // This is analog
    else if( args[0][0] == 'a' )
        pinState = analogRead(pinNum);

    if( !Var_setInt(var, pinState) )
        return -1;

    return 0;
}

int write( char* cmd_name, char** args, int8_t args_len )
{
    // TODO: add args for initiate pin before write
    uint8_t pinNum;
    uint8_t pinState;

    // check if there is any variable
    if( !Var_isVar(args[1]) )
        pinState = atoi(args[1]);
    else
        pinState = Var_getInt( Var_getVarFromArg(args[1]) );

    if( (pinState != 0) && (pinState != 1) )
        return -1;

    pinNum = atoi( &args[0][1] );

    // This is digital
    if( args[0][0] == 'd' ) {
        pinMode(pinNum, HIGH);
        digitalWrite( pinNum, pinState );
    }

    // This is analog
    else if( args[0][0] == 'a' )
        analogWrite( pinNum, pinState );

    return 0;
}

int print( char* cmd, char** args, int8_t args_len )
{
    const char* data;

    for(uint8_t iii = 0; iii < args_len; iii++) {
        // check existance of variables
        if( !Var_isVar(args[iii]) )
            wSerial_print( args[iii] );
        else {
            if( (data = Var_getStr( Var_getVarFromArg(args[iii]) )) != NULL )
                wSerial_print( data );

            else
                return -1;
        }

        if( iii < args_len - 1 )
            wSerial_printChar(' ');
    }

    return 0;
}

int set( char* cmd, char** args, int8_t args_len )
{
    if( !Var_setStr(args[0][0], args[1], strlen(args[1])) )
        return -1;

    return 0;
}
