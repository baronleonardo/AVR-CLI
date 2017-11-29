#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <stdint.h>

#define MAX_PARAMS          10      // maximum parameters of a cmd
#define BAUDRATE            9600    // baudrate for uart
#define ALPHA_LEN           26      // 26 represents the 26 letters in the english alphabets
#define CMD_MAX_LEN         100     // max input length of one cmd
#define VAR_MAX_LEN         10      // max variable length
#define FLOAT_PRECITION     5       // number of digits after the decimal sign
typedef int (*cmd_func_t)(char* cmd_name, char** args, int8_t args_len);

#endif // DEFINITIONS_H_
