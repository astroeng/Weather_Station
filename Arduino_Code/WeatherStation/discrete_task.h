/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef DISCRETE_TASK_H
#define DISCRETE_TASK_H

#include <numerical_statistics.h>

#define DISCRETE_INTERVAL 1000

/* Variables for saving the fun numbers. */
typedef enum
{
  station_wind_direction = 0,
  station_wind_speed,
  station_uv_light,
  station_discrete_data_size
} DiscreteDataEnumType;

typedef struct
{
  Numerical_Statistics collectedData[station_discrete_data_size];
  Numerical_Statistics batteryVoltage;
  int                  windDirectionForMaxSpeed;

} DiscreteDataType;

void setupDiscreteSensors();

void readDiscreteSensors();

DiscreteDataType* getDiscreteData();

unsigned int getRainfall();
unsigned int getWindDirection();

void resetDiscreteData();

#endif
