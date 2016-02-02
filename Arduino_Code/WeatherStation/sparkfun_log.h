/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef SPARKFUN_LOG_H
#define SPARKFUN_LOG_H

#include <Arduino.h>

#include "sparkfun_keys_real.h"
#include "ethernet.h"

#define JSON_STRING_LENGTH 200

const char PROGMEM preamble[]                  = "/input/";                               // 7
const char PROGMEM privateKeyText[]            = "?private_key=";                         // 13
const char PROGMEM messageCountString[]        = "&mc=";


const char PROGMEM weatherPublicKey[]        = WEATHER_PUBLIC_KEY;   // 20
const char PROGMEM weatherPrivateKey[]       = WEATHER_PRIVATE_KEY;  // 20

const char PROGMEM intervalString[]          = "&i_ms=";           // 6
const char PROGMEM humidityString[]          = "&h_ap=";           // 6
const char PROGMEM pressureString[]          = "&p_in=";           // 6
const char PROGMEM temperatureString[]       = "&t_df=";           // 6
const char PROGMEM ir_lightString[]          = "&l_ir=";           // 6
const char PROGMEM uv_lightString[]          = "&l_uv=";           // 6
const char PROGMEM white_lightString[]       = "&l_wh=";           // 6
const char PROGMEM wind_directionString[]    = "&wd_d=";           // 6
const char PROGMEM wind_speedString[]        = "&ws_mph=";         // 8
const char PROGMEM wind_speed_stdevString[]  = "&ws_std=";         // 8
const char PROGMEM wind_speed_maxString[]    = "&ws_max=";         // 8
const char PROGMEM wind_speed_maxdirString[] = "&ws_dmx=";         // 8
const char PROGMEM rainfallString[]          = "&rf_in=";          // 8

/* The order of these values must match the data array in the
 * build function found in the main Weather Station file.
 */

static const char* weatherStrings[] = {intervalString,
                                       humidityString,
                                       pressureString,
                                       temperatureString,
                                       ir_lightString,
                                       uv_lightString,
                                       white_lightString,
                                       wind_directionString,
                                       wind_speedString,
                                       wind_speed_stdevString,
                                       wind_speed_maxString,
                                       wind_speed_maxdirString,
                                       rainfallString,
                                       messageCountString};


const char PROGMEM systemPublicKey[]          = SYSTEM_PUBLIC_KEY;                        // 20
const char PROGMEM systemPrivateKey[]         = SYSTEM_PRIVATE_KEY;                       // 20

const char PROGMEM task1_average_timeString[] = "&t1a_ms=";
const char PROGMEM task1_max_timeString[]     = "&t1m_ms=";
const char PROGMEM task2_average_timeString[] = "&t2a_ms=";
const char PROGMEM task2_max_timeString[]     = "&t2m_ms=";
const char PROGMEM task3_average_timeString[] = "&t3a_ms=";
const char PROGMEM task3_max_timeString[]     = "&t3m_ms=";
const char PROGMEM task4_average_timeString[] = "&t4a_ms=";
const char PROGMEM task4_max_timeString[]     = "&t4m_ms=";
const char PROGMEM batteryVoltageString[]     = "&bv_v=";
const char PROGMEM uptimeString[]             = "&upt_s=";

/* The order of these values must match the data array in the
 * build function found in the main Weather Station file.
 */

static const char* systemStrings[] = {task1_average_timeString,
                                      task1_max_timeString,
                                      task2_average_timeString,
                                      task2_max_timeString,
                                      task3_average_timeString,
                                      task3_max_timeString,
                                      task4_average_timeString,
                                      task4_max_timeString,
                                      batteryVoltageString,
                                      uptimeString,
                                      messageCountString};

void createLoggingString(const char* publicKey, const char* privateKey,
                         long* dataArray, const char** stringArray, char numValues);

char* getLogString();

char sendLoggingString(HTTP_Connection* device);

#endif
