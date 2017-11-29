#include "wserial.h"

extern "C" void wSerial_begin( size_t baudrate )
{
    Serial.begin(baudrate);
}

extern "C" size_t wSerial_println(const char* data)
{
    return Serial.println(data);
}

extern "C" size_t wSerial_print(const char* data)
{
    return Serial.print(data);
}

extern "C" size_t wSerial_printChar(char c)
{
    return Serial.print(c);
}

extern "C" size_t wSerial_printInt(int num)
{
    return Serial.print(num);
}

extern "C" int wSerial_available()
{
    return Serial.available();
}

extern "C" int wSerial_availableForWrite()
{
    return Serial.availableForWrite();
}

extern "C" int wSerial_read()
{
    return Serial.read();
}

//extern "C" const char* wSerial_readStringUntil(char terminator)
//{
//    return Serial.readStringUntil(terminator).c_str();
//}

extern "C" int wSerial_mirrorReceivedDataUntil( char terminator,
                                                char* buf,
                                                size_t len )
{
    char c;
    String recv_data = "";

    while( ((c = Serial.read()) != terminator) )
    {
        if( c >= 0 )
        {
            Serial.print( c );
            recv_data.operator +=( c );
        }
    }

    Serial.println();

    if( recv_data.length() > len )
        return -1;

    memcpy( buf, recv_data.c_str(), len );

    return recv_data.length();
}
