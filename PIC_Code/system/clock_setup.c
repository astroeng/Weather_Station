/* Derek Schacht 
 *  2016 01 25 
 *  Clock Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */
#include "xc.h"

#define xOSC_FAST_RC_WITH_POST_AND_PLL 0b001
#define xOSC_FAST_RC                   0b000


void clockConfig()
{
    OSCCONbits.COSC = xOSC_FAST_RC;
    OSCCONbits.NOSC = xOSC_FAST_RC_WITH_POST_AND_PLL;
    OSCCONbits.CLKLOCK = 0;
    OSCCONbits.IOLOCK = 0;
    // Read Only OSCCONbits.LOCK
    // Read Only OSCCONbits.CF
    OSCCONbits.POSCEN = 1;
    OSCCONbits.SOSCEN = 0;
    OSCCONbits.OSWEN = 1;
    
    CLKDIVbits.ROI = 0;
    CLKDIVbits.DOZE = 0;
    CLKDIVbits.DOZEN = 0;
    CLKDIVbits.RCDIV = 0;
    
    OSCTUNbits.TUN = 0;
}