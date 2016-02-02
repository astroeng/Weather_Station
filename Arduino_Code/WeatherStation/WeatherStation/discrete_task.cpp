/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include <Arduino.h>
#include <argent_80422.h>

#include "discrete_task.h"
#include "global_data.h"
#include "config.h"

/* Setup the weatherVane and other gear on it. */
Argent_80422 weatherVane(WIND_DIRECTION,
                         WIND_SPEED,
                         RAIN_FALL);

DiscreteDataType discreteDataBuffer;

/* ISR declarations */
void localWindSpeed_ISR()
{
  weatherVane.windSpeed_ISR();
}
void localRainFall_ISR()
{
  weatherVane.rainFall_ISR();
}


/* Circular values are special the next two functions wrap the circle stuff up.
 * Use the Mitsuta Method (ish) for circular averages. This still isn't perfect but
 * will suite the needs of the weather station.
 */
void updateWindDirection(long working_value)
{
  long delta = discreteDataBuffer.collectedData[station_wind_direction].getMean() - working_value;

  if (delta < -1800)
  {
    working_value -= 3600;
  }
  else if (delta > 1800)
  {
    working_value += 3600;
  }

  discreteDataBuffer.collectedData[station_wind_direction].includeValue(working_value);

}

unsigned int getWindDirection()
{
  int working_value;

  working_value = discreteDataBuffer.collectedData[station_wind_direction].getMean();

  if (working_value >= 3600)
  {
    working_value -= 3600;
  }
  else if(working_value < 0)
  {
    working_value += 3600;
  }

  return working_value;
}


void setupDiscreteSensors()
{
  /* TODO: A0 is only used as a pulldown for the prototype. */
  pinMode(A0,OUTPUT);
  pinMode(BATTERY_PIN,INPUT);
  digitalWrite(A0,HIGH);
  weatherVane.begin();

  /* Attach the interrupts this is for the weatherVane. */
  /* TODO: move this to the WeatherVane.begin code... maybe */
  attachInterrupt(digitalPinToInterrupt(2), localWindSpeed_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(3), localRainFall_ISR, RISING);
}


/* This function reads the sensors that are attached directly to the Arduino.
 * The wind speed, wind direction, and rain fall sensors fall into this function.
 * NOTE: Rain processing is done in the logging task.
 *
 * In the future one of the light sensors will be here as well.
 * TODO: Add UV light sensor.
 */

void readDiscreteSensors()
{
  long currentWindspeed;
  long currentWindDirection;

  currentWindspeed = weatherVane.getWindSpeed();
  currentWindDirection = weatherVane.getWindDirection();
  
  updateWindDirection(currentWindDirection);
  discreteDataBuffer.collectedData[station_wind_speed].includeValue(currentWindspeed);

  if (currentWindspeed == discreteDataBuffer.collectedData[station_wind_speed].getMax())
  {
    discreteDataBuffer.windDirectionForMaxSpeed = currentWindDirection;
  }

  discreteDataBuffer.batteryVoltage.includeValue(((long)analogRead(BATTERY_PIN) * 500l) / 1023l);

  taskRunTime[task_discrete_read].includeValue(schedule.getTaskExecutionTime(task_discrete_read));
}

/* Data Output Function */
DiscreteDataType* getDiscreteData()
{
  return &discreteDataBuffer;
}

unsigned int getRainfall()
{
  unsigned int value = weatherVane.getRainFall();
  weatherVane.resetRainFall();

  return value;
}

void resetDiscreteData()
{
  int i;
  for (i = 0; i < station_discrete_data_size; i++)
  {
    discreteDataBuffer.collectedData[i].reset();
  }
  discreteDataBuffer.batteryVoltage.reset();
}
