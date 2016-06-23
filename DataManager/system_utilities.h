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

// Initialization call for time functions in this file. Must be called
// at program start.. or anytime time needs to be rebaselined to now.

void time_init();

// Arduino style time functions. 

time64_t micros();
time64_t millis();

// Arduino style delay functions.

void delay(time64_t millis);
void delayMicroseconds(time64_t micros);

// This function will return a time string of the form:
//   YYYY-MM-DDTHH:MM:SS:mmmZ
// Notes - for the moment mmm is always 000

char* timeDateString(char* buffer, int buffer_len);

// This function will return true if time has crossed midnight since
// the last call to the function.

bool crossedMidnight();

#endif
