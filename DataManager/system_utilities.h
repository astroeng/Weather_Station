/* Derek Schacht
 *  2016 03 03
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef SYSTEM_UTILITIES_H
#define SYSTEM_UTILITIES_H

#include <ctime>

typedef long long time64_t;

extern timespec boot;
extern volatile int serverRunning;

void time_init();

time64_t micros();
time64_t millis();

void delay(time64_t millis);
void delayMicroseconds(time64_t micros);

#endif
