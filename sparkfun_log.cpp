

#include "sparkfun_log.h"
#include <avr/pgmspace.h>

unsigned long logTime = 0;
unsigned long currentTime;


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

void createLoggingString(long pressure, int humidity,    int temperature, int uv_light, 
                         int ir_light,  int white_light, int wind_speed,  int wind_direction, char* logString)
{

  int ls_index = 0;

  Serial.println((int)(&ls_index),HEX);
  Serial.println((int)(&currentTime),HEX);

  
  //(*logString) = "";

  //logString += addressString; /* Not actually needed to make the GET request to the sparkfun server. */
  //(*logString) += preamble_and_keyString;
  strCopy(preamble_and_keyString,&ls_index,logString);
  
  //(*logString) += intervalString;
  strCopy(intervalString,&ls_index,logString);
  currentTime = millis();
  //temp = String(currentTime - logTime);
  toString(currentTime - logTime,&ls_index,logString);
  logTime = currentTime;
  //(*logString) += temp;
  
  //(*logString) += humidityString;
  strCopy(humidityString,&ls_index,logString);
  //temp = String(humidity);
  //(*logString) += temp;
  toString(humidity,&ls_index,logString);
  
  //(*logString) += pressureString;
  strCopy(pressureString,&ls_index,logString);
  //temp = String(pressure);
  //(*logString) += temp;
  toString(pressure,&ls_index,logString);
  
  //(*logString) += temperatureString;
  strCopy(temperatureString,&ls_index,logString);
  //temp = String(temperature);
  //(*logString) += temp;
  toString(temperature,&ls_index,logString);

  //(*logString) += ir_lightString;
  strCopy(ir_lightString,&ls_index,logString);
  //temp = String(ir_light);
  //(*logString) += temp;
  toString(ir_light,&ls_index,logString);
  
  //(*logString) += uv_lightString;
  strCopy(uv_lightString,&ls_index,logString);
  //temp = String(uv_light);
  //(*logString) += temp;
  toString(uv_light,&ls_index,logString);
  
  //(*logString) += white_lightString;
  strCopy(white_lightString,&ls_index,logString);
  //temp = String(white_light);
  //(*logString) += temp;
  toString(white_light,&ls_index,logString);

  //(*logString) += wind_directionString;
  strCopy(wind_directionString,&ls_index,logString);
  //temp = String(wind_direction);
  //(*logString) += temp;
  toString(wind_direction,&ls_index,logString);
  
  //(*logString) += wind_speedString;
  strCopy(wind_speedString,&ls_index,logString);
  //temp = String(wind_speed);
  //(*logString) += temp;
  toString(wind_speed,&ls_index,logString);

  logString[ls_index] = 0;
  Serial.println(ls_index);
  //Serial.println(logString);
}
