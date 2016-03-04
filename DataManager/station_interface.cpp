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
#include <sstream>

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "station_interface.h"
#include "phant_strings.h"
#include "tcp_client.h"

/* Function to get the server response and print it to the terminal.
 */
void getHttpResponse(TCP_Client* client)
{
  char buffer2[1000];
  bzero (buffer2, 1000);

  client->readData(buffer2, 999);

  cout << "RESPONSE " << buffer2 << endl;
}


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
    case QuitMessage:
      return (MessageKindType)header.messageKind;
    default:
      return InvalidMessage;
  }
}


int processWeatherMessage(Client_Interface* client)
{
  stringstream getRequest;
  const int messageSize = sizeof (WeatherDataType);
  int bytes;
  WeatherDataType weatherData;

  byte buffer[messageSize];
  bzero(buffer, messageSize);

  bytes = client->readFrom(buffer, messageSize);

  memcpy(&weatherData, buffer, bytes);

  getRequest << "GET ";
  getRequest << preamble << weatherPublicKey << privateKeyText << weatherPrivateKey;

  getRequest << messageCountString      << weatherData.messageCount;
  getRequest << intervalString          << weatherData.intervalTime;
  getRequest << humidityString          << weatherData.humidity;
  getRequest << pressureString          << weatherData.pressure;
  getRequest << temperatureString       << weatherData.temperature;
  getRequest << ir_lightString          << weatherData.irLight;
  getRequest << uv_lightString          << weatherData.uvLight;
  getRequest << white_lightString       << weatherData.whiteLight;
  getRequest << wind_directionString    << weatherData.windDirection;
  getRequest << wind_speedString        << weatherData.windSpeed;
  getRequest << wind_speed_stdevString  << weatherData.windSpeedStdev;
  getRequest << wind_speed_maxString    << weatherData.maxWindSpeed;
  getRequest << wind_speed_maxdirString << weatherData.windDirectionAtMaxWindSpeed;
  getRequest << rainfallString          << weatherData.messageCount;

  getRequest << " HTTP/1.1"                 << endl;
  getRequest << "Host: data.sparkfun.com"   << endl;
  getRequest << "User-Agent: ddwrt-weather" << endl;
  getRequest << "Connection: close"         << endl;
  getRequest << endl; /* Blank line to end the transfer. VERY IMPORTANT */

  cout << string(getRequest.str()) << endl;

  TCP_Client sparkfun("data.sparkfun.com", 80);
  sparkfun.sendData(getRequest.str().c_str(), getRequest.str().length());

  getHttpResponse(&sparkfun);

  return 0;

}


int processStatusMessage(Client_Interface* client)
{

  stringstream getRequest;
  const int messageSize = sizeof (StatusDataType);
  int bytes;
  StatusDataType statusData;

  byte buffer[messageSize];
  bzero(buffer, messageSize);

  bytes = client->readFrom(buffer, messageSize);

  memcpy(&statusData, buffer, bytes);

  getRequest << "GET ";
  getRequest << preamble << systemPublicKey << privateKeyText << systemPrivateKey;

  getRequest << messageCountString       << statusData.messageCount;
  getRequest << task1_average_timeString << statusData.task1_average_execution_time;
  getRequest << task1_max_timeString     << statusData.task1_max_execution_time;
  getRequest << task2_average_timeString << statusData.task2_average_execution_time;
  getRequest << task2_max_timeString     << statusData.task2_max_execution_time;
  getRequest << task3_average_timeString << statusData.task3_average_execution_time;
  getRequest << task3_max_timeString     << statusData.task3_max_execution_time;
  getRequest << task4_average_timeString << statusData.task4_average_execution_time;
  getRequest << task4_max_timeString     << statusData.task4_max_execution_time;
  getRequest << batteryVoltageString     << statusData.batteryVoltage;
  getRequest << uptimeString             << statusData.uptime;

  getRequest << " HTTP/1.1"                 << endl;
  getRequest << "Host: data.sparkfun.com"   << endl;
  getRequest << "User-Agent: ddwrt-weather" << endl;
  getRequest << "Connection: close"         << endl;
  getRequest << endl; /* Blank line to end the transfer. VERY IMPORTANT */

  cout << getRequest.str() << endl;

  TCP_Client sparkfun("data.sparkfun.com", 80);
  sparkfun.sendData(getRequest.str().c_str(), getRequest.str().length());

  getHttpResponse(&sparkfun);

  return 0;

}





