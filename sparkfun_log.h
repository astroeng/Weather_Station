#ifndef SPARKFUN_LOG_H
#define SPARKFUN_LOG_H

#include <Arduino.h>

#include "sparkfun_keys.h"

const char PROGMEM addressString[]          = "data.sparkfun.com\0";                         // 17
const char PROGMEM preamble_and_keyString[] = "/input/1nXZMd60WgfxnN7oWl6q?private_key=0mAl5Rjkp7T9nyvB540r\0"; // 60
const char PROGMEM intervalString[]         = "&interval=\0";                                // 10
const char PROGMEM humidityString[]         = "&humidity=\0";                                // 10
const char PROGMEM pressureString[]         = "&pressure=\0";                                // 10
const char PROGMEM temperatureString[]      = "&temperature=\0";                             // 13
const char PROGMEM ir_lightString[]         = "&ir_light=\0";                                // 10
const char PROGMEM uv_lightString[]         = "&uv_light=\0";                                // 10
const char PROGMEM white_lightString[]      = "&white_light=\0";                             // 13
const char PROGMEM wind_directionString[]   = "&wind_direction=\0";                          // 16
const char PROGMEM wind_speedString[]       = "&wind_speed=\0";                              // 12
                                                                                     
                                                                                     // 164 Stringy Bits
                                                                                     // 36  Numerical (9 numerics - 4 digits each)

                                                                                     // 200 Total

void createLoggingString(long pressure, int humidity,    int temperature, int uv_light, 
                         int ir_light,  int white_light, int wind_speed,  int wind_direction, char* logString);

#endif
