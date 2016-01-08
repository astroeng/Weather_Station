/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due. 
 *  Disclamer : I try and site code that I find on the internet but I am not perfect. If you find 
 *              something that should be sited let me know and I will update my code.
 *  Warranty : Absolutly None
 *  
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef SPARKFUN_LOG_H
#define SPARKFUN_LOG_H

#include <Arduino.h>

#include "sparkfun_keys_real.h"

#define JSON_STRING_LENGTH 190

//const char PROGMEM addressString[]             = "data.sparkfun.com\0";                     // 17
const char PROGMEM preamble[]                  = "/input/\0";                               // 7
const char PROGMEM publicKey[]                 = PUBLIC_KEY;                                // 20
const char PROGMEM privateKeyText[]            = "?private_key=\0";                         // 13
const char PROGMEM privateKey[]                = PRIVATE_KEY;                               // 20
const char PROGMEM intervalString[]            = "&i_ms=\0";                                // 6
const char PROGMEM humidityString[]            = "&h_ap=\0";                                // 6
const char PROGMEM pressureString[]            = "&p_in=\0";                                // 6
const char PROGMEM temperatureString[]         = "&t_df=\0";                                // 6
const char PROGMEM ir_lightString[]            = "&l_ir=\0";                                // 6
const char PROGMEM uv_lightString[]            = "&l_uv=\0";                                // 6
const char PROGMEM white_lightString[]         = "&l_wh=\0";                                // 6
const char PROGMEM wind_directionString[]      = "&wd_d=\0";                                // 6
const char PROGMEM wind_speedString[]          = "&ws_mph=\0";                              // 8
const char PROGMEM wind_speed_stdevString[]    = "&ws_std=\0";                              // 8
const char PROGMEM wind_speed_maxString[]      = "&ws_max=\0";                              // 8
const char PROGMEM wind_speed_maxdirString[]   = "&ws_dmx=\0";                              // 8
                                                                                     
                                                                                     // 157 (ish) Stringy Bits
                                                                                     // 36  (ish) Numerical (9 numerics - 4 digits each)

                                                                                     // 193 (ish) Total


void createLoggingString(long pressure, 
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
                         char* logString);

#endif
