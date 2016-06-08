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

#include "system_utilities.h"
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

Interface_Return_Type sendHttpMessage(stringstream &httpMessage, string server, int port)
{
  // Try to send the message to the sparkfun server up to 5 times.

  for (int i = 0; i < RETRY_LIMIT; i++)
  {
    // Try and send the message to the sparkfun server. The TCP_Client
    // class will throw exceptions if for some reason the socket cannot
    // be created, or the connection cannot be made. Just catch them 
    // and proceed.
    try 
    {
      TCP_Client sparkfun(server.c_str(), port);
      sparkfun.sendData(httpMessage.str().c_str(), httpMessage.str().length());

      getHttpResponse(&sparkfun);

      return Return_Success;
    }
    catch (...)
    { 
      // Report the error and then wait just a little bit before
      // allowing the loop to continue.
      cout << "ERROR: Send Failed Retry Count : " << i << endl;
      delay(100);
    }
  }
  return Return_Fail;
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

// This function will format and send a weather message to the sparkfun
// servers. This function assumes that the message kind is done by some
// other function. Other than only reading the size of the WeatherDataType
// there is no data validation.

Interface_Return_Type processWeatherMessage(Client_Interface* client)
{

  // Setup a couple of things that this function will use:
  
  //   The stringstream is used to build the get message based on data
  //   from the weather message and built in strings. Point of note
  //   since this functionality is now on what is basically a full up
  //   computer. A proper JSON implementation could probably be used.
  
  //   messageSize is created more for convenience than anything. It is
  //   used to create the message buffer and tell the client class how
  //   many bytes to read.
  
  //   weatherData is a typed structure that matches the type structure
  //   of the message sent by the client.
  
  //   buffer is the buffer that data is read into from the client.
  
  stringstream getRequest;
  const int messageSize = sizeof (WeatherDataType);
  int bytes;
  WeatherDataType weatherData;

  byte buffer[messageSize];
  bzero(buffer, messageSize);

  // Read the data from the connected client.
  bytes = client->readFrom(buffer, messageSize);

  // If the messageSize matches the number of read bytes copy the data
  // into a data structure and then create and send the GET message to
  // the sparkfun servers.
  if (bytes == messageSize)
  {

    memcpy(&weatherData, buffer, bytes);

    // Build the GET request that will be sent to the sparkfun server.
    // This uses the data from the client as well as strings that form
    // a JSON string. This is pretty crude and was adapted from code
    // designed to run on a tiny microcontroller. As stated above this
    // could potentially be upgraded to a full JSON implementation. But
    // since this is only ever creating JSON strings, it's not too bad.

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

    // Print the request message to the console.
    cout << string(getRequest.str()) << endl;

    return sendHttpMessage(getRequest, "data.sparkfun.com", 80);
  }
  
  return Return_Fail;
}


Interface_Return_Type processStatusMessage(Client_Interface* client)
{

  // This function is pretty similar to processWeatherMessage. Consult
  // that function for comments.

  stringstream getRequest;
  const int messageSize = sizeof (StatusDataType);
  int bytes;
  StatusDataType statusData;

  byte buffer[messageSize];
  bzero(buffer, messageSize);

  bytes = client->readFrom(buffer, messageSize);

  if (bytes == messageSize)
  {
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

    return sendHttpMessage(getRequest, "data.sparkfun.com", 80);
  }
  return Return_Fail;
}
