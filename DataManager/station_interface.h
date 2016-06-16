/* Derek Schacht
 *  2016 02 28
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef STATION_INTERFACE_H
#define STATION_INTERFACE_H

#include "client_interface.h"
#include "system_types.h"
#include "station_types.h"

#define RETRY_LIMIT 5

typedef enum
{
    Return_Success = 0,
    Return_Fail
} Interface_Return_Type;

MessageKindType messageType(StationHeaderType headerData);

Interface_Return_Type processWeatherMessage(WeatherDataType weatherData);

Interface_Return_Type processStatusMessage(StatusDataType statusData);

#endif
