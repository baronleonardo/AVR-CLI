#include "Arduino.h"
#include "CLI.h"

void setup()
{
    CLI_init(9600);
}

void loop()
{
    CLI_exec();
}
