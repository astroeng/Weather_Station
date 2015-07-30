

#include "sparkfun_log.h"


unsigned long logTime = 0;

/* Hold a string object in global scope. A pointer to this object will 
 * be returned by the createLoggingString function. This helps with 
 * execution speed and RAM efficency. This cannot go into PROGMEM since
 * it needs to be writeable.
 */

String logString = "";

/* This function will build the string that is sent after the URL to
 * sparkfun. The string constains the keys as well as all of the data
 * that is expected by sparkfun. This is JSON in it's lightest possible
 * implementation.
 */

String* createLoggingString(long pressure, int humidity,    int temperature, int uv_light, 
                            int ir_light,  int white_light, int wind_speed,  int wind_direction)
{

  unsigned long currentTime;
  String temp;
  
  logString = "";

  //logString += addressString; /* Not actually needed to make the GET request to the sparkfun server. */
  logString += preamble_and_keyString;
  
  logString += intervalString;
  currentTime = millis();
  temp = String(currentTime - logTime);
  logTime = currentTime;
  logString += temp;
  
  logString += humidityString;
  temp = String(humidity);
  logString += temp;
  
  logString += pressureString;
  temp = String(pressure);
  logString += temp;
  
  logString += temperatureString;
  temp = String(temperature);
  logString += temp;

  logString += ir_lightString;
  temp = String(ir_light);
  logString += temp;
  
  logString += uv_lightString;
  temp = String(uv_light);
  logString += temp;
  
  logString += white_lightString;
  temp = String(white_light);
  logString += temp;
  
  logString += wind_directionString;
  temp = String(wind_direction);
  logString += temp;
  
  logString += wind_speedString;
  temp = String(wind_speed);
  logString += temp;
  
  return &logString;
  
}
