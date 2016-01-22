/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef DIGITAL_TASK_H
#define DIGITAL_TASK_H

#include <numerical_statistics.h>

#define DIGITAL_INTERVAL 166

/* Variables for saving the fun numbers. */
typedef enum
{
  station_air_pressure,
  station_air_temperature,
  station_air_humidity,
  station_ir_light,
  station_white_light,
  station_digital_data_size
} DigitalDataEnumType;

typedef struct
{
  Numerical_Statistics collectedData[station_digital_data_size];
} DigitalDataType;

void setupDigitalSensors( void );

void readDigitalSensors( void );

DigitalDataType* getDigitalData( void );

void resetDigitalData();

#endif
