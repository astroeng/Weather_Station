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
#include <string>
#include <cstring>

using namespace std;

#include <stdio.h>

#include "phant_strings.h"
#include "station_interface.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "client_interface.h"
#include "system_utilities.h"
#include "system_error.h"
#include "numerical_statistics.h"

int processClient(TCP_Server* tcpServer)
{
  const int messageSize = sizeof (StationHeaderType);
  int bytes;
  time64_t start;

  int status = 0;

  byte buffer[messageSize];
  bzero(buffer, messageSize);

  Client_Interface client = tcpServer->accept();

  bytes = client.readFrom(buffer, messageSize);

  start = micros();
  switch(start)
  {
  case WeatherMessage:
    cout << "Weather Header" << endl;
    //status = processWeatherMessage(&client);

    break;
  case StatusMessage:
    cout << "Status Header" << endl;
    //status = processStatusMessage(&client);

    break;
  case QuitMessage:
    cout << "Quit Header" << endl;
    serverRunning = 0;

    break;
  default:
    outputError("MAIN: Bad Header");
    break;
  }

  buffer[messageSize] = 0;
  cout << buffer << bytes << " " << status << endl;

  cout << "TIME: " << micros() - start << endl;
  
  return status;
}

void *udpThread( void* )
{
  Client_Interface newClient("9876");
  
  const int bufferSize = 128;
  const int timeBufferSize = 80;
  
  time64_t start;

  int status = 0;
  
  Numerical_Statistics averageTime;
  
  while(serverRunning != 0)
  {
    char timeBuffer[timeBufferSize];
    byte buffer[bufferSize];
    
    bzero(buffer, bufferSize);
    
    int bytes = newClient.readFrom(buffer, bufferSize);
    
    cout << "Received Bytes: " << bytes << endl;
    cout << "Local Time: " << timeDateString(timeBuffer, timeBufferSize) << endl;
    
    StationHeaderType header;
    memcpy(&header, buffer, sizeof(StationHeaderType));

    start = micros();

    switch(messageType(header))
    {
    case WeatherMessage:
      cout << "Message Type: Weather Header @ " << 
              "Message Time: " << timeDateString(timeBuffer, timeBufferSize) << 
              "Message Size: " << bytes << endl;
  
      // weatherMessage is a typed structure that matches the type
      // structure of the message sent by the client.
      WeatherMessageType weatherMessage;

      // If the expected size matches the number of read bytes copy the 
      // data into a data structure and then process the received data.
      if (bytes == sizeof (WeatherMessageType))
      {
        memcpy(&weatherMessage, buffer, sizeof (WeatherMessageType));
        status = processWeatherMessage(weatherMessage.data);
      }
      else
      {
        outputError("Main: Bad Weather Message");
      }
      break;

    case StatusMessage:
      cout << "Message Type: Status Header @ " << 
              "Message Time: " << timeDateString(timeBuffer, timeBufferSize) << 
              "Message Size: " << bytes << endl;

      StatusMessageType statusMessage;

      if (bytes == sizeof (StatusMessageType))
      {
        memcpy(&statusMessage, buffer, sizeof (StatusMessageType));
        status = processStatusMessage(statusMessage.data);
      }
      else
      {
        outputError("Main: Bad Status Message");
      }

      break;
    case QuitMessage:
      cout << "Message Type: Quit Header @" << 
              "Message Time: " << timeDateString(timeBuffer, timeBufferSize) << 
              "Message Size: " << bytes << endl;

      serverRunning = 0;

      break;
    default:
      outputError("MAIN: Bad Header");
      break;
    }

    buffer[4] = 0;
    cout << buffer << " " << status << endl;

    time64_t sendTime = micros() - start;

    cout << "TIME: " << sendTime << endl;

    if (crossedMidnight())
    {
      averageTime.reset();
    }

    averageTime.includeValue(sendTime);

    cout << "ATIME: " << averageTime.getMean() << " " <<
                         averageTime.getMax() << " " << 
                         averageTime.getMin() << " " << 
                         averageTime.getStdev() << endl;
  }
}

int main()
{
  try
  {
    time_init();
    time64_t start = micros();
    serverRunning = 1;
    delay(1000);
    cout << micros() - start << endl;

    TCP_Server tcpServer(9876, 3);

    pthread_t udpThreadTask;
    pthread_create(&udpThreadTask, NULL, udpThread, NULL);

    while (serverRunning)
    {
      try
      {
        processClient(&tcpServer);
      }
      catch (...)
      {
        cout << "ERROR: The processClient call failed" << endl;
      }
    }
  }
  catch (...)
  {
    cout << "ERROR: Well thats the end" << endl;
  }

  return 0;
}
