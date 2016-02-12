
#ifndef ADC_SUPPORT_H
#define ADC_SUPPORT_H

#define ADC_CONTROL1      AD1CON1bits
#define ADC_CONTROL2      AD1CON2bits
#define ADC_CONTROL3      AD1CON3bits
#define ADC_INPUT_SELECT  AD1CHSbits
#define ADC_PORT_CONFIG_1 AD1PCFGL
#define ADC_PORT_CONFIG_2 AD1PCFGH
#define ADC_SCAN_SELECT_1 AD1CSSL
#define ADC_SCAN_SELECT_2 AD1CSSH

#define ADC_INTERRUPT_ENABLE(x) IEC0bits.AD1IE = x
#define ADC_INTERRUPT_ACK()     IFS0bits.AD1IF = 0

typedef struct
{
  unsigned int adcControl1;
  unsigned int adcControl2;
  unsigned int adcControl3;
  unsigned int adcInputSelect;
  unsigned int adcPortConfigLow;
  unsigned int adcPortConfigHigh;
  unsigned int adcScanSelectLow;
  unsigned int adcScanSelectHigh;
} AdcConfigType;


void adc_init(AdcConfigType* config);
void adc_begin(AdcConfigType* config);

unsigned int analogRead(unsigned char index);
unsigned int adc_readChannel(unsigned char index);


#endif