/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due. 
 *  Disclamer : I try and site code that I find on the internet but I am not perfect. If you find 
 *              something that should be sited let me know and I will update my code.
 *  Warranty : Absolutly None
 *  
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include "sparkfun_log.h"
#include <avr/pgmspace.h>

#define TEST(x)

unsigned long systemMessageCount = 0;
unsigned long weatherMessageCount = 0;
unsigned long logTime = 0;
unsigned long currentTime;

/* Custom function because the sprintf uses up a TON of memory and
 * it is incomplete on the Arduino platform.
 */
void toString(long value, int* index, char* string)
{
  char count = 0;
  char looper = 0;
  char array[10];

  /* If the value is negative put a negative sign in the output string then
   * invert the number.
   */
  if (value < 0)
  {
    string[(*index)] = '-';
    value *= -1;
    (*index)++;
  }
  if (value == 0)
  {
    array[count] = value % 10 + '0';
    count++;
  }
  /* This will make a string representation of the value
   * in reverse digit order.
   */
  while (value > 0)
  {
    array[count] = value % 10 + '0';
    value = value / 10;
    count++;
  }
  /* Since the above code generates the number in reverse... copy
   * from the tail of the array into the next spot on the output
   * string.
   */
  for (looper = count-1; looper >= 0; looper--)
  {
    string[(*index)] = array[looper];
    (*index)++;
  }
}

/* String copy function. This is to read strings from PROGMEM. */

void strCopy(const char* value, int* index, char* string)
{
  int v_index = 0;
  int len = strlen_P(value);
  for (v_index = 0; v_index < len; v_index++)
  {
    string[(*index)] = pgm_read_byte_near(value + v_index);
    (*index)++;
  }   
}

/* This function will build the string that is sent after the URL to
 * sparkfun. The string constains the keys as well as all of the data
 * that is expected by sparkfun. This is JSON in it's lightest possible
 * implementation.
 */

void createWeatherString(long pressure, 
                         int humidity,    
                         int temperature, 
                         int uv_light, 
                         int ir_light,  
                         int white_light, 
                         int wind_speed, 
                         int wind_speed_max,
                         int wind_speed_std,
                         int wind_speed_max_direction, 
                         int wind_direction, 
                         int rainfall,
                         char* logString)
{

  int ls_index = 0;

  weatherMessageCount++;

  TEST(Serial.println((int)(&ls_index),HEX));
  TEST(Serial.println((int)(&currentTime),HEX));

  strCopy(preamble,&ls_index,logString);
  strCopy(weatherPublicKey,&ls_index,logString);
  strCopy(privateKeyText,&ls_index,logString);
  strCopy(weatherPrivateKey,&ls_index,logString);
  

  strCopy(intervalString,&ls_index,logString);
  currentTime = millis();
  toString(currentTime - logTime,&ls_index,logString);
  logTime = currentTime;

  strCopy(humidityString,&ls_index,logString);
  toString(humidity,&ls_index,logString);

  strCopy(pressureString,&ls_index,logString);
  toString(pressure,&ls_index,logString);

  strCopy(temperatureString,&ls_index,logString);
  toString(temperature,&ls_index,logString);

  strCopy(ir_lightString,&ls_index,logString);
  toString(ir_light,&ls_index,logString);

  strCopy(uv_lightString,&ls_index,logString);
  toString(uv_light,&ls_index,logString);

  strCopy(white_lightString,&ls_index,logString);
  toString(white_light,&ls_index,logString);

  strCopy(wind_directionString,&ls_index,logString);
  toString(wind_direction,&ls_index,logString);

  strCopy(wind_speedString,&ls_index,logString);
  toString(wind_speed,&ls_index,logString);

  strCopy(wind_speed_maxString,&ls_index,logString);
  toString(wind_speed_max,&ls_index,logString);

  strCopy(wind_speed_stdevString,&ls_index,logString);
  toString(wind_speed_std,&ls_index,logString);

  strCopy(wind_speed_maxdirString,&ls_index,logString);
  toString(wind_speed_max_direction,&ls_index,logString);

  strCopy(rainfallString,&ls_index,logString);
  toString(rainfall,&ls_index,logString);

  strCopy(messageCountString,&ls_index,logString);
  toString(weatherMessageCount,&ls_index,logString);

  logString[ls_index] = 0;

  TEST(Serial.println(ls_index));
  TEST(Serial.println(logString));
}

void createSystemString(unsigned int task1_average_time,
                        unsigned int task1_max_time,
                        unsigned int task2_average_time,
                        unsigned int task2_max_time,
                        unsigned int task3_average_time,
                        unsigned int task3_max_time,
                        unsigned int task4_average_time,
                        unsigned int task4_max_time,
                        unsigned int battery_voltage,
                        unsigned long uptime,
                        char* logString)
{
  int ls_index = 0;
  systemMessageCount++;

  strCopy(preamble,&ls_index,logString);
  strCopy(systemPublicKey,&ls_index,logString);
  strCopy(privateKeyText,&ls_index,logString);
  strCopy(systemPrivateKey,&ls_index,logString);
  
  strCopy(task1_average_timeString,&ls_index,logString);
  toString(task1_average_time,&ls_index,logString);

  strCopy(task1_max_timeString,&ls_index,logString);
  toString(task1_max_time,&ls_index,logString);

  strCopy(task2_average_timeString,&ls_index,logString);
  toString(task2_average_time,&ls_index,logString);

  strCopy(task2_max_timeString,&ls_index,logString);
  toString(task2_max_time,&ls_index,logString);

  strCopy(task3_average_timeString,&ls_index,logString);
  toString(task3_average_time,&ls_index,logString);

  strCopy(task3_max_timeString,&ls_index,logString);
  toString(task3_max_time,&ls_index,logString);

  strCopy(task4_average_timeString,&ls_index,logString);
  toString(task4_average_time,&ls_index,logString);

  strCopy(task4_max_timeString,&ls_index,logString);
  toString(task4_max_time,&ls_index,logString);

  strCopy(batteryVoltageString,&ls_index,logString);
  toString(battery_voltage,&ls_index,logString);

  strCopy(uptimeString,&ls_index,logString);
  toString(uptime,&ls_index,logString);

  strCopy(messageCountString,&ls_index,logString);
  toString(systemMessageCount,&ls_index,logString);

  logString[ls_index] = 0;
}
