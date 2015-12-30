#ifndef SPARKFUN_LOG_H
#define SPARKFUN_LOG_H

#include <Arduino.h>

#include "sparkfun_keys.h"

const char PROGMEM addressString[]          = "data.sparkfun.com\0";                     // 17
const char PROGMEM preamble[]               = "/input/\0";                               // 7
const char PROGMEM publicKey[]              = PUBLIC_KEY;                                // 20
const char PROGMEM privateKeyText[]         = "?private_key=\0";                         // 13
const char PROGMEM privateKey[]             = PRIVATE_KEY;                               // 20
const char PROGMEM intervalString[]         = "&i_ms=\0";                                // 6
const char PROGMEM humidityString[]         = "&h_ap=\0";                                // 6
const char PROGMEM pressureString[]         = "&p_in=\0";                                // 6
const char PROGMEM temperatureString[]      = "&t_df=\0";                                // 6
const char PROGMEM ir_lightString[]         = "&l_ir=\0";                                // 6
const char PROGMEM uv_lightString[]         = "&l_uv=\0";                                // 6
const char PROGMEM white_lightString[]      = "&l_wh=\0";                                // 6
const char PROGMEM wind_directionString[]   = "&wd_d=\0";                                // 6
const char PROGMEM wind_speedString[]       = "&ws_mph=\0";                              // 8
                                                                                     
                                                                                     // 133 (ish) Stringy Bits
                                                                                     // 36  (ish) Numerical (9 numerics - 4 digits each)

                                                                                     // 170 (ish) Total

void createLoggingString(long pressure, 
                         int humidity,    
                         int temperature, 
                         int uv_light, 
                         int ir_light,  
                         int white_light, 
                         int wind_speed,  
                         int wind_direction, 
                         char* logString);

#endif
