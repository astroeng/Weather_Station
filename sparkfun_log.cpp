

#include "sparkfun_log.h"


unsigned long logTime = 0;

String createLoggingString(long pressure, int humidity,    int temperature, int uv_light, 
                           int ir_light,  int white_light, int wind_speed,  int wind_direction)
{

  unsigned long currentTime;
  String logString = "";
  String temp;

  logString += addressString;
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
  
  temp = "0.0";
  
  logString += ir_lightString;
  logString += temp;
  
  logString += uv_lightString;
  logString += temp;
  
  logString += white_lightString;
  logString += temp;
  
  logString += wind_directionString;
  logString += temp;
  
  logString += wind_speedString;
  logString += temp;
  
}
