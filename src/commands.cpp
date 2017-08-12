#include "commands.h"
#include "wserial.h"

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

int read( char* cmd, char* args )
{
    uint8_t pinNum;
    char *arg_pin;
   
    arg_pin = strtok(args, " \r\n\t");
    pinNum = atoi(arg_pin + 1);
    
    // This is digital
    if( arg_pin[0] == 'd' )
    {
        pinMode(pinNum, INPUT);
        return digitalRead(pinNum);
    }

    // This is analog
    else if( arg_pin[0] == 'a' )
    {
        return analogRead(pinNum);
    }

    return -1;
}

int write( char* cmd, char* args )
{
    uint8_t pinNum;
    char *arg_pin;
    char *arg_value;
   
    arg_pin = strtok(args, " \r\n\t");
    pinNum = atoi(arg_pin + 1);
    arg_value = strtok(NULL, " \r\n\t");
    
    // This is digital
    if( arg_pin[0] == 'd' )
    {
        pinMode(pinNum, HIGH);
        digitalWrite(pinNum, atoi(arg_value));
    }

    // This is analog
    else if( arg_pin[0] == 'a' )
    {
        analogWrite(pinNum, atoi(arg_value));
    }

    return 0;
}

}
