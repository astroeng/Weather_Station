#include "picduino.h"
#include "adc_support.h"


volatile unsigned int* adcValues[] = {&ADC1BUF0,&ADC1BUF1,&ADC1BUF2,&ADC1BUF3,
                                      &ADC1BUF4,&ADC1BUF5,&ADC1BUF6,&ADC1BUF7,
                                      &ADC1BUF8,&ADC1BUF9,&ADC1BUFA,&ADC1BUFB,
                                      &ADC1BUFC,&ADC1BUFD,&ADC1BUFE,&ADC1BUFF};


void adc_init(AdcConfigType* config)
{
    /* Nothing to do yet. */
}

/* This is the configuration that will allow the ADC to auto sample the analog
 * pins. The update rate is set by the system clock and the values in the ADCS
 * and SAMC registers.
 */
void adc_begin(AdcConfigType* config)
{

    ADC_CONTROL1.ADON   = 0;
    ADC_CONTROL1.ADSIDL = 0;
    ADC_CONTROL1.FORM   = 0;
    ADC_CONTROL1.SSRC   = 7;
    ADC_CONTROL1.ASAM   = 1;
    
    ADC_CONTROL2.VCFG   = 0;
    ADC_CONTROL2.CSCNA  = 1;
    ADC_CONTROL2.SMPI   = 5;
    ADC_CONTROL2.BUFM   = 0;
    ADC_CONTROL2.ALTS   = 0;
    
    ADC_CONTROL3.ADCS   = 31;
    ADC_CONTROL3.ADRC   = 0;
    ADC_CONTROL3.SAMC   = 15;
    
    ADC_INPUT_SELECT.CH0NA = 0;
    ADC_INPUT_SELECT.CH0NB = 0;
    ADC_INPUT_SELECT.CH0SA = 0;
    ADC_INPUT_SELECT.CH0SB = 0;
    
    ADC_PORT_CONFIG_1 = 0b1111001111111111;
    ADC_PORT_CONFIG_2 = 0b1111111111111111;
    ADC_SCAN_SELECT_1 = 0b0000111111000000;
    ADC_SCAN_SELECT_2 = 0b0000000000000000;

    ADC_INTERRUPT_ENABLE(0);
    
    ADC_CONTROL1.ADON = 1;
    
}


/* This function just returns the latest sample from the ADC buffer. The buffer
 * update rate is determined by the number of channels being sampled.
 */

unsigned int analogRead(unsigned char index)
{
    if (index >= A0 && A5 >= index)
    {
        return (*adcValues[index - A0]);
    }
    return 0;
}