/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include "station_types.h"
#include "global_data.h"
#include "weather_log_task.h"
#include "discrete_task.h"
#include "digital_task.h"

/* Some conversion constants */
#define PASCAL_TO_PSI 0.145037738
#define PASCAL_TO_INHG 0.295333727
#define C_TO_F(x) ((9*x)/5)+3200

unsigned long weatherMessageCount = 0;
unsigned long weatherLogTime      = 0;

/* Function to populate logString with the weather data. */

void sendWeatherLog()
{
  unsigned int rainFall = getRainfall();

  unsigned long currentTime = millis();

  DigitalDataType*  digitalDataPtr  = getDigitalData();
  DiscreteDataType* discreteDataPtr = getDiscreteData();

  weatherMessageCount++;

  WeatherMessageType message;

  message.header.messageKind               = WeatherMessage;
  message.data.messageCount                = weatherMessageCount;
  message.data.intervalTime                = currentTime - weatherLogTime;
  message.data.humidity                    = digitalDataPtr->collectedData[station_air_humidity].getMean();
  message.data.pressure                    = digitalDataPtr->collectedData[station_air_pressure].getMean()*PASCAL_TO_INHG;
  message.data.temperature                 = C_TO_F(digitalDataPtr->collectedData[station_air_temperature].getMean());
  message.data.windDirection               = getWindDirection();
  message.data.windSpeed                   = discreteDataPtr->collectedData[station_wind_speed].getMean();
  message.data.windDirectionAtMaxWindSpeed = discreteDataPtr->windDirectionForMaxSpeed;
  message.data.maxWindSpeed                = discreteDataPtr->collectedData[station_wind_speed].getMax();
  message.data.windSpeedStdev              = discreteDataPtr->collectedData[station_wind_speed].getStdev()/100l;
  message.data.irLight                     = digitalDataPtr->collectedData[station_ir_light].getMean();
  message.data.uvLight                     = discreteDataPtr->collectedData[station_uv_light].getMean();
  message.data.whiteLight                  = digitalDataPtr->collectedData[station_white_light].getMean();

  sparkfun_logger.sendData((char*)&message, sizeof(message));

  weatherLogTime = currentTime;

}

/* Perform the steps to provide the remote server with weather data. */

void logData()
{

  sendWeatherLog();

  /* Reset all of the statistics classes to get ready for the next 2 minutes. */

  resetDiscreteData();
  resetDigitalData();

  taskRunTime[task_weather_log].includeValue(schedule.getTaskExecutionTime(task_weather_log));

}
