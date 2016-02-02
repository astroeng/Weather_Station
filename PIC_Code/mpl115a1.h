/* Derek Schacht 
 *  2016 01 25 
 *  Code Module for interfacing with an SPI based MPL11A1
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#include "spi_support.h"

#define PASCAL_TO_INHG 0.295333727

typedef struct
{
  int a0_coeff;  /* 16 bit value 3 fraction bits */
  int b1_coeff;  /* 16 bit value 12 fraction bits */
  int b2_coeff;  /* 16 bit value 13 fraction bits */
  int c12_coeff; /* 14 bit value all fraction bits with 9 padded 0s effectively 10 ^ -9 */
} MPL115A1CalibrationDataType;

typedef struct
{
    unsigned int tAdc;
    unsigned int pAdc;
} MPL115A1AdcDataType;

typedef struct
{
    unsigned long pressure_pa;
    unsigned int temperature_c;
    unsigned long lastSample;
} MPL115A1DataType;

void mpl115a1_startConversion(SpiConfigType* config);
void mpl115a1_getCalibration(SpiConfigType* spiDevice, MPL115A1CalibrationDataType* calData);
void mpl115a1_getData(SpiConfigType* spiDevice, MPL115A1AdcDataType* adcData);
unsigned long mpl115a1_computePressure(MPL115A1CalibrationDataType* calData, MPL115A1AdcDataType* adcData);
int mpl115a1_computeTemperature(MPL115A1CalibrationDataType* calData, MPL115A1AdcDataType* adcData);
int mpl115a1_run(SpiConfigType* spiDevice, MPL115A1CalibrationDataType* calData, MPL115A1DataType* data);
