/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due. 
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find 
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
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

void createHeader(const char* publicKey, const char* privateKey, int* index, char* string)
{
  strCopy(preamble,       index, string);
  strCopy(publicKey,      index, string);
  strCopy(privateKeyText, index, string);
  strCopy(privateKey,     index, string);
}

void addBodyText(const char* bodyString, long value, int* index, char* string)
{
  strCopy(bodyString, index, string);
  toString(value,     index, string);
}
/* This function will build the string that is sent after the URL to
 * Sparkfun. The string contains the keys as well as all of the data
 * that is expected by Sparkfun. This is JSON in it's lightest possible
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
  char i = 0;

  weatherMessageCount++;
  currentTime = millis();
  
  /* The order of these must match the string array defined in the
   * sparkfun_log.h file.
   */
  long dataArray[] = {currentTime - logTime,
                      humidity,
                      pressure,
                      temperature,
                      ir_light,
                      uv_light,
                      white_light,
                      wind_direction,
                      wind_speed,
                      wind_speed_std,
                      wind_speed_max,
                      wind_speed_max_direction,
                      weatherMessageCount};
  
  logTime = currentTime;
  
  TEST(Serial.println((int)(&ls_index),HEX));
  TEST(Serial.println((int)(&currentTime),HEX));

  createHeader(weatherPublicKey, weatherPrivateKey, &ls_index, logString);
  
  for (i = 0; i < log_weather_size; i++)
  {
    addBodyText(weatherStrings[i], dataArray[i], &ls_index, logString);
  }

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
  char i;
  systemMessageCount++;

  /* The order of these must match the string array defined in the
   * sparkfun_log.h file.
   */  
  long dataArray[] = {task1_average_time,
                      task1_max_time,
                      task2_average_time,
                      task2_max_time,
                      task3_average_time,
                      task3_max_time,
                      task4_average_time,
                      task4_max_time,
                      battery_voltage,
                      uptime,
                      systemMessageCount};
  
  createHeader(systemPublicKey, systemPrivateKey, &ls_index, logString);
  
  for (i = 0; i < log_weather_size; i++)
  {
    addBodyText(systemStrings[i], dataArray[i], &ls_index, logString);
  }

  logString[ls_index] = 0;

}
