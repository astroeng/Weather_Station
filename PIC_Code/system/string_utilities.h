/* Derek Schacht 
 *  2016 01 31
 *    Basic string utilities
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

static unsigned char returnC = '\r';
static unsigned char formFeed = 0x0C;

unsigned char* toString(unsigned long value);

#endif