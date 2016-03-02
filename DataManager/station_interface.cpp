/* Derek Schacht
 *  2016 02 28
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include <iostream>
#include <cstring>

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "station_interface.h"


MessageKindType messageType(byte* buffer)
{
  StationHeaderType header;
  memcpy(&header, buffer, sizeof(StationHeaderType));

  switch(header.messageKind)
  {
    case WeatherMessage:
      return (MessageKindType)header.messageKind;
    case StatusMessage:
      return (MessageKindType)header.messageKind;
    default:
      return InvalidMessage;
  }
}






