/* Derek Schacht 
 *  2016 01 25 
 *   Arduino style Sketch Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */


#include "xc.h"
#include "picduino.h"
#include "clock_setup.h"

int main(void) 
{
    clockConfig();
    pinInit();
    
    systemTimerConfig();
    
    setup();
    
    while(1)
    {
        loop();
    }
    
}
