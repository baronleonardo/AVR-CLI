#ifndef WSERIAL_H
#define WSERIAL_H

#include "Arduino.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

void wSerial_begin(size_t baudrate);
size_t wSerial_println(char* data);
size_t wSerial_print(char* data);
size_t wSerial_printChar(char c);
size_t wSerial_printInt(int num);
int wSerial_available();
int wSerial_availableForWrite();
int wSerial_read();

// print all the receieved characters until `terminator`
// then return the read characters
int wSerial_mirrorReceivedDataUntil( char terminator,
                                     char* buf,
                                     size_t len );

#ifdef __cplusplus
}
#endif

#endif // WSERIAL_H
