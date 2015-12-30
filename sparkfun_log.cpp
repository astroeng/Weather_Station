

#include "sparkfun_log.h"
#include <avr/pgmspace.h>

#define TEST(x)

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

  if (value < 0)
  {
    array[count] = '-';
    value *= -1;
    count++;
  }
  if (value == 0)
  {
    array[count] = value % 10 + '0';
    count++;
  }
  while (value > 0)
  {
    array[count] = value % 10 + '0';
    value = value / 10;
    count++;
  }
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

void createLoggingString(long pressure, 
                         int humidity,    
                         int temperature, 
                         int uv_light, 
                         int ir_light,  
                         int white_light, 
                         int wind_speed,  
                         int wind_direction, 
                         char* logString)
{

  int ls_index = 0;

  TEST(Serial.println((int)(&ls_index),HEX));
  TEST(Serial.println((int)(&currentTime),HEX));

  strCopy(preamble,&ls_index,logString);
  strCopy(publicKey,&ls_index,logString);
  strCopy(privateKeyText,&ls_index,logString);
  strCopy(privateKey,&ls_index,logString);
  

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

  logString[ls_index] = 0;

  TEST(Serial.println(ls_index));
  TEST(Serial.println(logString));
}
