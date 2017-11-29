#include "Arduino.h"
#include "__CLI.h"

void setup() {
    __CLI_init(BAUDRATE);
}

void loop() {
    __CLI_mainLoop();
}
