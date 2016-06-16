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
  
  int messageSize = 128;
  
  time64_t start;

  int status = 0;
  
  while(1)
  {
    byte buffer[messageSize];
    bzero(buffer, messageSize);
    int bytes = newClient.readFrom(buffer, messageSize);

    cout << "BYTES: " << bytes << endl;

    StationHeaderType header;
    memcpy(&header, buffer, sizeof(StationHeaderType));

    start = micros();
    switch(messageType(header))
    {
    case WeatherMessage:
      cout << "Weather Header" << endl;
      
      //   messageSize is created more for convenience than anything. It is
      //   used to create the message buffer and tell the client class how
      //   many bytes to read.
  
      //   weatherMessage is a typed structure that matches the type structure
      //   of the message sent by the client.

      messageSize = sizeof (WeatherMessageType);
      WeatherMessageType weatherMessage;

      // If the messageSize matches the number of read bytes copy the data
      // into a data structure and then create and send the GET message to
      // the sparkfun servers.
      // if (bytes == messageSize)

      memcpy(&weatherMessage, buffer, messageSize);
      
      status = processWeatherMessage(weatherMessage.data);

      break;
    case StatusMessage:
      cout << "Status Header" << endl;

      messageSize = sizeof (StatusMessageType);
      StatusMessageType statusMessage;

      memcpy(&statusMessage, buffer, messageSize);
      
      status = processStatusMessage(statusMessage.data);

      break;
    case QuitMessage:
      cout << "Quit Header" << endl;
      serverRunning = 0;

      break;
    default:
      outputError("MAIN: Bad Header");
      break;
    }

    buffer[4] = 0;
    cout << buffer << " " << status << endl;

    cout << "TIME: " << micros() - start << endl;
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
