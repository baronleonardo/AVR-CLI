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

//    { "set",
//      // "Desc: create a variable and assign a value to it\n"
//      // "Usage: set <variable> <value>\n"
//      // "NOTE: variables are only one letter a, or b, or c, ...",
//      set,
//      NULL },

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
    // `-i` means initiate
    if( strcmp(args[0], "-i") == 0 )
        pinMode(RED_LED, OUTPUT);

    if( strcmp(args[1], "ON") == 0 )
        digitalWrite(RED_LED, HIGH);

    else if( strcmp(args[1], "OFF") == 0 )
        digitalWrite(RED_LED, LOW);

    else {
        Serial.println("Wrong arguments!");
        return -1;
    }

    return 0;
}

int delayMS( char* cmd_name, char** args, int8_t args_len )
{
    int amount = atoi( args[0] );

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
    uint8_t pinNum;
   
    pinNum = atoi( &args[0][1] );
    
    // This is digital
    if( args[0][0] == 'd' ) {
        pinMode(pinNum, INPUT);
        return digitalRead(pinNum);
    }

    // This is analog
    else if( args[0][0] == 'a' )
        return analogRead(pinNum);

    return -1;
}

int write( char* cmd_name, char** args, int8_t args_len )
{
    uint8_t pinNum;

    pinNum = atoi( &args[0][1] );

    // This is digital
    if( args[0][0] == 'd' ) {
        pinMode(pinNum, HIGH);
        digitalWrite( pinNum, atoi(args[1]) );
    }

    // This is analog
    else if( args[0][0] == 'a' )
        analogWrite( pinNum, atoi(args[1]) );

    return 0;
}

int print( char* cmd, char** args, int8_t args_len )
{
    for(uint8_t iii = 0; iii < args_len; iii++) {
        wSerial_print( args[iii] );

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
