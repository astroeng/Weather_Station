
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <sys/time.h>

using namespace std;

#include <stdio.h>

#include "phant_strings.h"
#include "station_interface.h"

#include "tcp_server.h"
#include "tcp_client.h"
#include "client_interface.h"

#include "system_utilities.h"


int processClient(TCP_Server* tcpServer)
{
  const int messageSize = sizeof (StationHeaderType);
  int bytes;
  time64_t start;

  byte buffer[messageSize];
  bzero(buffer, messageSize);
  
  Client_Interface client = tcpServer->accept();

  bytes = client.readFrom(buffer, messageSize);

  start = micros();
  switch(messageType(buffer))
  {
  case WeatherMessage:
    cout << "Weather Header" << endl;
    processWeatherMessage(&client);

    break;
  case StatusMessage:
    cout << "Status Header" << endl;
    processStatusMessage(&client);

    break;
  case QuitMessage:
    cout << "Quit Header" << endl;
    serverRunning = 0;

    break;
  default:
    cout << "Bad Header" << endl;
    break;
  }

  buffer[messageSize] = 0;
  cout << buffer << bytes << endl;

  cout << "TIME: " << micros() - start << endl;
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

    while (serverRunning)
    {
      processClient(&tcpServer);
    }

  }
  catch (int ex)
  {
    char buffer[100];
    bzero(buffer,100);
    perror(buffer);
    cout << "Ex " << ex << endl << buffer << endl;
  }

  return 0;
}
