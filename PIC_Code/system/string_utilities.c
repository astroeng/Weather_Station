/* Derek Schacht 
 *  2016 01 31
 *   Basic string utilities
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */


#include "string_utilities.h"

unsigned char buffer[9];

unsigned char* toString(unsigned long value)
{
    buffer[8] = 0;
    buffer[7] = ' ';
    buffer[6] = (value % 10) + '0';
    value /= 10;
    buffer[5] = (value % 10) + '0';
    value /= 10;
    buffer[4] = (value % 10) + '0';
    value /= 10;
    buffer[3] = (value % 10) + '0';
    value /= 10;
    buffer[2] = (value % 10) + '0';
    value /= 10;
    buffer[1] = (value % 10) + '0';
    value /= 10;
    buffer[0] = (value % 10) + '0';
    
    return buffer;
}
