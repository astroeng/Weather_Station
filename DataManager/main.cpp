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
  switch(messageType(buffer))
  {
  case WeatherMessage:
    cout << "Weather Header" << endl;
    status = processWeatherMessage(&client);

    break;
  case StatusMessage:
    cout << "Status Header" << endl;
    status = processStatusMessage(&client);

    break;
  case QuitMessage:
    cout << "Quit Header" << endl;
    serverRunning = 0;

    break;
  default:
    cout << "ERROR: Bad Header" << endl;
    break;
  }

  buffer[messageSize] = 0;
  cout << buffer << bytes << endl;

  cout << "TIME: " << micros() - start << endl;
  
  return status;
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
