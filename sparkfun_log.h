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

#include "sparkfun_keys.h"

#define JSON_STRING_LENGTH 200

typedef enum 
{
	log_common_preamble = 0,
	log_common_private_key,
	log_common_message_count,
	log_common_length
} CommonStringsType;

const char PROGMEM preamble[]                  = "/input/\0";                               // 7
const char PROGMEM privateKeyText[]            = "?private_key=\0";                         // 13
const char PROGMEM messageCountString[]        = "&mc=\0";

const char* const PROGMEM commonStrings[] = {preamble,
                                             privateKeyText,
											 messageCountString};

typedef enum 
{
	log_weather_interval,
	log_weather_humidity,
	log_weather_pressure,
	log_weather_temperature,
	log_weather_ir_light,
	log_weather_uv_light,
	log_weather_white_light,
	log_weather_wind_direction,
	log_weather_wind_speed,
	log_weather_max_wind_speed,
	log_weather_max_wind_speed_direction,
	log_weather_rainfall,
	log_weather_message_count,
	log_weather_size
} WeatherStringsType;

const char PROGMEM weatherPublicKey[]        = WEATHER_PUBLIC_KEY;   // 20
const char PROGMEM weatherPrivateKey[]       = WEATHER_PRIVATE_KEY;  // 20
const char PROGMEM intervalString[]          = "&i_ms=\0";           // 6
const char PROGMEM humidityString[]          = "&h_ap=\0";           // 6
const char PROGMEM pressureString[]          = "&p_in=\0";           // 6
const char PROGMEM temperatureString[]       = "&t_df=\0";           // 6
const char PROGMEM ir_lightString[]          = "&l_ir=\0";           // 6
const char PROGMEM uv_lightString[]          = "&l_uv=\0";           // 6
const char PROGMEM white_lightString[]       = "&l_wh=\0";           // 6
const char PROGMEM wind_directionString[]    = "&wd_d=\0";           // 6
const char PROGMEM wind_speedString[]        = "&ws_mph=\0";         // 8
const char PROGMEM wind_speed_stdevString[]  = "&ws_std=\0";         // 8
const char PROGMEM wind_speed_maxString[]    = "&ws_max=\0";         // 8
const char PROGMEM wind_speed_maxdirString[] = "&ws_dmx=\0";         // 8
const char PROGMEM rainfallString[]          = "&rf_in=\0";          // 8

/* This has to match the order of the data array in the weather log function. */

const char* const PROGMEM weatherStrings[] = {intervalString,
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

typedef enum 
{
	log_system_t1_average_interval,
	log_system_t1_max_interval,
	log_system_t2_average_interval,
	log_system_t2_max_interval,
	log_system_t3_average_interval,
	log_system_t3_max_interval,
	log_system_t4_average_interval,
	log_system_t4_max_interval,
	log_system_battery_voltage,
	log_system_uptime,
	log_system_message_count,
	log_system_size
} SystemStringsType;																				 
																					 
const char PROGMEM systemPublicKey[]          = SYSTEM_PUBLIC_KEY;                        // 20
const char PROGMEM systemPrivateKey[]         = SYSTEM_PRIVATE_KEY;                       // 20
const char PROGMEM task1_average_timeString[] = "&t1a_ms=\0";
const char PROGMEM task1_max_timeString[]     = "&t1m_ms=\0";
const char PROGMEM task2_average_timeString[] = "&t2a_ms=\0";
const char PROGMEM task2_max_timeString[]     = "&t2m_ms=\0";
const char PROGMEM task3_average_timeString[] = "&t3a_ms=\0";
const char PROGMEM task3_max_timeString[]     = "&t3m_ms=\0";
const char PROGMEM task4_average_timeString[] = "&t4a_ms=\0";
const char PROGMEM task4_max_timeString[]     = "&t4m_ms=\0";
const char PROGMEM batteryVoltageString[]     = "&bv_v=\0";
const char PROGMEM uptimeString[]             = "&upt_s=\0";

/* This has to match the order of the data array in the system log function. */

const char* const PROGMEM systemStrings[] = {task1_average_timeString,
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
                         char* logString);

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
                        char* logString);

#endif
