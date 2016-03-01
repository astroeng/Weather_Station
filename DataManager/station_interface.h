/* Derek Schacht
 *  2016 02 28
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef STATION_INTERFACE_H
#define STATION_INTERFACE_H

#include "system_types.h"

typedef enum
{
  WeatherMessage = 0x40001111,
  StatusMessage  = 0x40002222,
  InvalidMessage = 0x4FFFFFFF,
} MessageKindType;


typedef struct
{
  u_int32 messageCount;
  u_int32 messageKind;
} StationHeaderType;

typedef struct
{
  int32 humidity;
  int32 pressure;
  int32 temperature;
  int32 windDirection;
  int32 windSpeed;
  int32 windDirectionAtMaxWindSpeed;
  int32 maxWindSpeed;
  int32 windSpeedStdev;
  int32 irLight;
  int32 uvLight;
  int32 whiteLight;
} WeatherDataType;

typedef struct
{
  StationHeaderType header;
  WeatherDataType   data;
} WeatherMessageType;


typedef struct
{
  int32 task1_average_execution_time;
  int32 task1_max_execution_time;
  int32 task2_average_execution_time;
  int32 task2_max_execution_time;
  int32 task3_average_execution_time;
  int32 task3_max_execution_time;
  int32 task4_average_execution_time;
  int32 task4_max_execution_time;
  int32 batteryVoltage;
  int32 uptime;
} StatusDataType;

typedef struct
{
  StationHeaderType header;
  StatusDataType    data;
} StatusMessageType;


MessageKindType messageType(byte* buffer);

#endif
