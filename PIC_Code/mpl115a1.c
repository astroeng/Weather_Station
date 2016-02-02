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
#include "picduino.h"
#include "mpl115a1.h"

void mpl115a1_startConversion(SpiConfigType* spiDevice)
{
    unsigned int startConversion[] = {0x24,0x00};
    while(spi_busy(spiDevice)) {}
    spi_sendData(spiDevice,startConversion,sizeof(startConversion));
    while(spi_busy(spiDevice)) {}

    spi_getData(spiDevice,0,0);
}

void mpl115a1_getCalibration(SpiConfigType* spiDevice, MPL115A1CalibrationDataType* calData)
{
    unsigned int calibrationValues[] = {0x88,0x00,0x8A,0x00,0x8C,0x00,0x8E,0x00,0x90,0x00,0x92,0x00,0x94,0x00,0x96,0x00,0x00};

    spi_sendData(spiDevice,calibrationValues,sizeof(calibrationValues));
    while(spi_busy(spiDevice) == 1) {}

    spi_getData(spiDevice,calibrationValues,sizeof(calibrationValues));
    
    calData->a0_coeff = (calibrationValues[1] << 8) | calibrationValues[3];
    calData->b1_coeff = (calibrationValues[5] << 8) | calibrationValues[7];
    calData->b2_coeff = (calibrationValues[9] << 8) | calibrationValues[11];
    calData->c12_coeff = (calibrationValues[13] << 8) | calibrationValues[15];

}

void mpl115a1_getData(SpiConfigType* spiDevice, MPL115A1AdcDataType* adcData)
{
    /* Make an array with the addresses of the data needed from the SPI module. */
    unsigned int dataValues[] = {0x80,0x00,0x82,0x00,0x84,0x00,0x86,0x00,0x00};
    
    spi_sendData(spiDevice,dataValues,sizeof(dataValues));
    
    while(spi_busy(spiDevice) == 1) {}

    spi_getData(spiDevice,dataValues, sizeof(dataValues));
    
    adcData->pAdc = ((dataValues[1] << 8) | dataValues[3]);
    adcData->tAdc = ((dataValues[5] << 8) | dataValues[7]);
    
    adcData->pAdc >>= 6;
    adcData->tAdc >>= 6;
    
}

unsigned long mpl115a1_computePressure(MPL115A1CalibrationDataType* calData, MPL115A1AdcDataType* adcData)
{
    long x1_tAdc_x_c12, x2_b1_p_x1, x3_pAdc_x_x2, x4_a0_p_x3, y1_b2_x_pAdc;
    unsigned long z1;
    
    x1_tAdc_x_c12 = (long)calData->c12_coeff * (long)adcData->tAdc;
    x2_b1_p_x1    = (((long)calData->b1_coeff<<11) + x1_tAdc_x_c12)>>11;
    x3_pAdc_x_x2  = x2_b1_p_x1 * (long)adcData->pAdc;
    x4_a0_p_x3    = ((long)calData->a0_coeff<<10) + x3_pAdc_x_x2;
            
    y1_b2_x_pAdc  = (((long)calData->b2_coeff)>>1) * (long)adcData->tAdc; // si_a2x2
    
    z1 = x4_a0_p_x3 + y1_b2_x_pAdc;
    
    /* The idea is to return pressure in pascal with a decent bit of resolution.
     * This fixed point computation matches the equivalent operations of floating
     * point math within a pretty small margin. */
    
    /* The largest pressure the sensor is capable of reading is 115 kpa, as an
     * upper limit the code will assume around 130 kpa. This gives a fair bit of
     * margin without sacrificing too much information. I wanted to avoid floating
     * point math since it is emulated. However, I should do an experiment to see
     * how slow it is as compared to this. */
    
    /* 65 * 4000 = 260000 and so does 416 * 625 */
    /* z1 = z1 * 260000 / 1023 / 8192 / 4 + 50000 */
    z1 *= 416;
    z1 /= 1023;
    z1 *= 625;
    z1 /= 8192;
    z1 = (z1/4) + 50000;

    return (z1);
}
int mpl115a1_computeTemperature(MPL115A1CalibrationDataType* calData, MPL115A1AdcDataType* adcData)
{
    return (int)((25.0 + ((double)adcData->tAdc - 514.0) / -5.35)*10.0);
}

int mpl115a1_run(SpiConfigType* spiDevice, MPL115A1CalibrationDataType* calData, MPL115A1DataType* data)
{
    MPL115A1AdcDataType pressureSensorData;
    
    mpl115a1_startConversion(spiDevice);
    delay(4ul);
    /* Get pressure and temperature data. */
    mpl115a1_getData(spiDevice, &pressureSensorData);

    data->pressure_pa   = mpl115a1_computePressure(calData, &pressureSensorData);
    data->temperature_c = mpl115a1_computeTemperature(calData, &pressureSensorData);
    data->lastSample    = millis();
    
    return 0;
}