
#include <iostream>

using namespace std;

#include "phant_interface.h"
#include "phant_strings.h"
#include "station_interface.h"

#include "tcp_server.h"
#include "client_interface.h"

int processWeatherMessage(Client_Interface* client)
{
  const int messageSize = sizeof (WeatherMessageType) + 1;
  int bytes;
  WeatherDataType weatherData;

  byte buffer[messageSize];
  bzero(buffer, messageSize);

  bytes = client->readFrom(buffer, messageSize - 1);

  memcpy(&weatherData, buffer, bytes);

  cout << weatherData.humidity << endl;
}

int processStatusMessage(Client_Interface* client)
{
  const int messageSize = sizeof (StatusDataType) + 1;
  int bytes;
  StatusDataType statusData;

  byte buffer[messageSize];
  bzero(buffer, messageSize);

  bytes = client->readFrom(buffer, messageSize - 1);

  memcpy(&statusData, buffer, bytes);

  cout << statusData.uptime << endl;
}

int processClient(TCP_Server* tcpServer)
{
  const int messageSize = sizeof (StationHeaderType) + 1;
  int bytes;
  byte buffer[messageSize];

  bzero(buffer, messageSize);
  
  Client_Interface client(tcpServer->getSocketFd());

  bytes = client.readFrom(buffer, messageSize - 1);

  switch(messageType(buffer))
  {
  case WeatherMessage:
    processWeatherMessage(&client);
    cout << "Weather Header" << endl;
    break;
  case StatusMessage:
    processStatusMessage(&client);
    cout << "Status Header" << endl;
    break;
  default:
    cout << "Bad Header" << endl;
    break;
  }

  cout << buffer << bytes << endl;
}

int main()
{

  long values[] = {1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE};


  createLoggingString(weatherPublicKey,weatherPrivateKey,values,weatherStrings,14);

  sendLoggingString((int*)values);

  TCP_Server tcpServer(9876,3);

  for (int i = 0; i < 2; i++)
  {
    processClient(&tcpServer);
  }
  return 0;

}
