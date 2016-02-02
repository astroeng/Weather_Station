/* Derek Schacht 
 *  2016 01 25 
 *  Arduino style Sketch Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#ifndef PICDUINO_H
#define PICDUINO_H

typedef void (*VoidFuncPtr) ( void );

#include "io_support.h"
#include "remappable_pin_support.h"
#include "timer_support.h"
#include "uart_support.h"

#define min(x,y) x < y ? x : y
#define max(x,y) x > y ? x : y

#define _SYSTEM_ISR  __attribute__((interrupt, no_auto_psv, shadow))

#define _USER_ISR __attribute__((interrupt, no_auto_psv, shadow))

#define CORE_CLOCK_HZ 16000000UL

/* These functions will be defined by the user in a file or files of their
 * choosing. 
 */
void setup();
void loop();

#endif
