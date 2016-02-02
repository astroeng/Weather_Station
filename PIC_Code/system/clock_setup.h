/* Derek Schacht 
 *  2016 01 25 
 *  Clock Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#ifndef CLOCK_SETUP_H
#define CLOCK_SETUP_H

#include "xc.h"

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & ICS_PGx1 & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR32 & WDTPS_PS1)
_CONFIG2(IESO_OFF & FNOSC_FRCPLL & FCKSM_CSECME & OSCIOFNC_ON & IOL1WAY_OFF & I2C2SEL_PRI & POSCMOD_NONE)
_CONFIG3(WPFP_WPFP0 & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)

void clockConfig();
        
#endif	
