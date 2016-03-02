
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <sys/time.h>

using namespace std;

#include "phant_interface.h"
#include "phant_strings.h"
#include "station_interface.h"

#include "tcp_server.h"
#include "tcp_client.h"
#include "client_interface.h"

typedef long long time64_t;

timespec boot;

void time_init()
{

  cout << "CLOCK SETUP: " << endl;

  clock_gettime(CLOCK_REALTIME, &boot);

}

time64_t micros()
{
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (ts.tv_sec - boot.tv_sec) * 1000000 + ts.tv_nsec / 1000;
}

void delay(time64_t micros)
{
  usleep(micros);
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

  getRequest << " HTTP/1.1" << endl;
  getRequest << "Host: data.sparkfun.com" << endl;
  getRequest << "User-Agent: ddwrt-weather" << endl;
  getRequest << "Connection: keep-alive" << endl;
  getRequest << endl;

  cout << string(getRequest.str()) << endl;

  TCP_Client sparkfun("data.sparkfun.com",80);
  sparkfun.sendData(getRequest.str().c_str(), getRequest.str().length());
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
  getRequest << batteryVoltageString     << 1234;
  getRequest << uptimeString             << statusData.uptime;

  getRequest << " HTTP/1.1" << endl;
  getRequest << "Host: data.sparkfun.com" << endl;
  getRequest << "User-Agent: arduino-ethernet" << endl;
  getRequest << "Connection: close" << endl;
  getRequest << endl;

  cout << getRequest.str() << endl;

  TCP_Client sparkfun("data.sparkfun.com", 80);
  sparkfun.sendData(getRequest.str().c_str(), getRequest.str().length());

  char buffer2[256];
  bzero (buffer2, 256);

  sparkfun.readData(buffer2, 255);

  cout << "RESPONSE " << buffer2 << endl;

}

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
  default:
    cout << "Bad Header" << endl;
    break;
  }

  cout << buffer << bytes << endl;

  cout << "TIME: " << micros() - start << endl;
}

int main()
{
  time_init();
  time64_t start = micros();
  delay(1000);
  cout << micros() - start << endl;

  //long values[] = {1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE};

  //createLoggingString(weatherPublicKey,weatherPrivateKey,values,weatherStrings,14);

  //sendLoggingString((int*)values);

  TCP_Server tcpServer(9876,3);

  for (int i = 0; i < 2; i++)
  {
    processClient(&tcpServer);
  }
  return 0;

}
