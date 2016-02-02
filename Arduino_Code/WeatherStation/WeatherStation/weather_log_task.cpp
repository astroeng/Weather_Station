/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */
#include "sparkfun_log.h"

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

void buildWeatherString()
{
  unsigned int rainFall = getRainfall();

  unsigned long currentTime = millis();

  DigitalDataType*  digitalDataPtr  = getDigitalData();
  DiscreteDataType* discreteDataPtr = getDiscreteData();

  weatherMessageCount++;

  /* The entries in dataArray must match the order of the entries in
   * weatherStrings defined in sparkfun_log.h
   */

  long dataArray[] = {currentTime - weatherLogTime,
                      digitalDataPtr->collectedData[station_air_humidity].getMean(),
                      digitalDataPtr->collectedData[station_air_pressure].getMean()*PASCAL_TO_INHG,
                      C_TO_F(digitalDataPtr->collectedData[station_air_temperature].getMean()),
                      digitalDataPtr->collectedData[station_ir_light].getMean(),
                      discreteDataPtr->collectedData[station_uv_light].getMean(),
                      digitalDataPtr->collectedData[station_white_light].getMean(),
                      getWindDirection(),
                      discreteDataPtr->collectedData[station_wind_speed].getMean(),
                      discreteDataPtr->collectedData[station_wind_speed].getStdev()/100l,
                      discreteDataPtr->collectedData[station_wind_speed].getMax(),
                      discreteDataPtr->windDirectionForMaxSpeed,
                      rainFall,
                      weatherMessageCount};

  weatherLogTime = currentTime;

  createLoggingString(weatherPublicKey, weatherPrivateKey,
                      dataArray, weatherStrings, 14);

}

/* Perform the steps to provide the remote server with weather data. */

void logData()
{
  char looper;

  buildWeatherString();

  sendLoggingString(&sparkfun_logger);

  /* Reset all of the statistics classes to get ready for the next 2 minutes. */

  resetDiscreteData();
  resetDigitalData();

  taskRunTime[task_weather_log].includeValue(schedule.getTaskExecutionTime(task_weather_log));

}
