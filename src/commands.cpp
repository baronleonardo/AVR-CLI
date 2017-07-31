#include "commands.h"

#define RED_LED 13

extern "C" {

int led( char* cmd, char* args )
{
    // get the first argument
    args = strtok( args, " \t\r\n" );

    // `-i` means initiate
    if( strcmp(args, "-i") == 0 )
    {
        pinMode(RED_LED, OUTPUT);
        // get the second argument
        args = strtok( NULL, " \t\r\n" );
    }

    if( strcmp(args, "ON") == 0 )
        digitalWrite(RED_LED, HIGH);

    else if( strcmp(args, "OFF") == 0 )
        digitalWrite(RED_LED, LOW);

    else
    {
        Serial.println("Wrong arguments!");
        return -1;
    }

    return 0;
}

int delayMS( char* cmd, char* args )
{
    int amount = atoi( strtok(args, " \t\r\n") );
    delay( amount );

    return 0;
}

}
