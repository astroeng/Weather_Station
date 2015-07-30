#ifndef SPARKFUN_LOG_H
#define SPARKFUN_LOG_H

#include <Arduino.h>

#include "sparkfun_keys.h"

/* Store these in the program memory instead of RAM. This will save about 256 bytes of RAM.
 */

const PROGMEM String addressString          = "data.sparkfun.com";
const PROGMEM String preamble_and_keyString = "/input/PUBLIC_KEY?private_key=PRIVATE_KEY";
const PROGMEM String intervalString         = "&interval=";
const PROGMEM String humidityString         = "&humidity=";
const PROGMEM String pressureString         = "&pressure=";
const PROGMEM String temperatureString      = "&temperature=";
const PROGMEM String ir_lightString         = "&ir_light=";
const PROGMEM String uv_lightString         = "&uv_light=";
const PROGMEM String white_lightString      = "&white_light=";
const PROGMEM String wind_directionString   = "&wind_direction=";
const PROGMEM String wind_speedString       = "&wind_speed=";

String* createLoggingString(long pressure, int humidity,    int temperature, int uv_light, 
                            int ir_light,  int white_light, int wind_speed,  int wind_direction);

#endif
