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

#include "client_interface.h"
#include "system_types.h"

typedef enum
{
  WeatherMessage = ('f' << 24) + ('d' << 16) + ('s' << 8) + 'a',
  StatusMessage  = ('r' << 24) + ('e' << 16) + ('w' << 8) + 'q',
  QuitMessage    = ('t' << 24) + ('i' << 16) + ('u' << 8) + 'q',
  InvalidMessage = 0x4FFFFFFF,
} MessageKindType;


typedef struct
{
  u_int32 messageKind;
} StationHeaderType;

typedef struct
{
  u_int32 messageCount;
  u_int32 intervalTime;
  u_int32 humidity;
  u_int32 pressure;
  int32 temperature;
  u_int32 windDirection;
  u_int32 windSpeed;
  u_int32 windDirectionAtMaxWindSpeed;
  u_int32 maxWindSpeed;
  u_int32 windSpeedStdev;
  u_int32 irLight;
  u_int32 uvLight;
  u_int32 whiteLight;
} WeatherDataType;

typedef struct
{
  StationHeaderType header;
  WeatherDataType   data;
} WeatherMessageType;


typedef struct
{
  u_int32 messageCount;
  u_int32 task1_average_execution_time;
  u_int32 task1_max_execution_time;
  u_int32 task2_average_execution_time;
  u_int32 task2_max_execution_time;
  u_int32 task3_average_execution_time;
  u_int32 task3_max_execution_time;
  u_int32 task4_average_execution_time;
  u_int32 task4_max_execution_time;
  u_int32 batteryVoltage;
  u_int32 uptime;
} StatusDataType;

typedef struct
{
  StationHeaderType header;
  StatusDataType    data;
} StatusMessageType;


MessageKindType messageType(byte* buffer);

int processWeatherMessage(Client_Interface* client);

int processStatusMessage(Client_Interface* client);

#endif
