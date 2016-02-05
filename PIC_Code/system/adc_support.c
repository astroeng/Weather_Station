#include "picduino.h"
#include "adc_support.h"

volatile unsigned char adcValuesNew = 0;
volatile unsigned int adcValues[16];

void _ISR _ADC1Interrupt()
{
    adcValuesNew = 1;

    adcValues[0]  = ADC1BUF0;
    adcValues[1]  = ADC1BUF1;
    adcValues[2]  = ADC1BUF2;
    adcValues[3]  = ADC1BUF3;
    adcValues[4]  = ADC1BUF4;
    adcValues[5]  = ADC1BUF5;
    adcValues[6]  = ADC1BUF6;
    adcValues[7]  = ADC1BUF7;
    adcValues[8]  = ADC1BUF8;
    adcValues[9]  = ADC1BUF9;
    adcValues[10] = ADC1BUFA;
    adcValues[11] = ADC1BUFB;
    adcValues[12] = ADC1BUFC;
    adcValues[13] = ADC1BUFD;
    adcValues[14] = ADC1BUFE;
    adcValues[15] = ADC1BUFF;

    ADC_INTERRUPT_ACK();
}

void adc_init(AdcConfigType* config)
{

    int i = 0;
    for (i = 0; i < 16; i++)
    {
        adcValues[i] = 0;
    }

}

void adc_begin(AdcConfigType* config)
{

    ADC_CONTROL1.ADON   = 0;
    ADC_CONTROL1.ADSIDL = 0;
    ADC_CONTROL1.FORM   = 0;
    ADC_CONTROL1.SSRC   = 7;
    ADC_CONTROL1.ASAM   = 0;

    ADC_CONTROL2.VCFG   = 0;
    ADC_CONTROL2.CSCNA  = 1;
    ADC_CONTROL2.SMPI   = 5;
    ADC_CONTROL2.BUFM   = 0;
    ADC_CONTROL2.ALTS   = 0;

    ADC_CONTROL3.ADCS   = 127;
    ADC_CONTROL3.ADRC   = 0;
    ADC_CONTROL3.SAMC   = 31;

    ADC_INPUT_SELECT.CH0NA = 0;
    ADC_INPUT_SELECT.CH0NB = 0;
    ADC_INPUT_SELECT.CH0SA = 0;
    ADC_INPUT_SELECT.CH0SB = 0;

    ADC_PORT_CONFIG_1 = 0b1111000000111111;
    ADC_PORT_CONFIG_2 = 0b1111111111111111;
    ADC_SCAN_SELECT_1 = 0b0000111111000000;
    ADC_SCAN_SELECT_2 = 0b0000000000000000;

    ADC_INTERRUPT_ENABLE(1);

    ADC_CONTROL1.ADON = 1;

}

unsigned char isNew(AdcWaitType mode)
{
    if (mode == Adc_BlockForNewValue)
    {
        while (adcValuesNew == 0) { Nop(); }
        return 0;
    }

    return adcValuesNew;
}

/* These function just return the latest sample from the ADC buffer. The buffer
 * update rate is determined by the number of channels being sampled and the 
 * sample interval triggered from the system timer ISR (timer_support.c).
 * I had planned for the ADC to auto sample but with a 16mhz clock speed the 
 * slowest the ADC would sample was 170us per channel. This seemed like a lot of
 * processing for something that might not need to be done that fast.
 */

unsigned int analogRead(unsigned char index)
{
    if (index >= A0 && A5 >= index)
    {
        return adcValues[index-A0];
    }
    return 0;
}

unsigned int adc_readChannel(unsigned char index)
{
    return adcValues[index];
}
