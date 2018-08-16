
#ifndef KOBUKIUTILITIES
#define KOBUKIUTILITIES

#include <stdint.h>

int kobukiInit();
uint8_t checkSumRead(uint8_t * buffer, int length);
uint8_t checkSum(uint8_t * buffer, int length);


#endif
