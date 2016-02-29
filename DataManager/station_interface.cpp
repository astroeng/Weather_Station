
#include <iostream>

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "station_interface.h"

u_int32 readMessage(byte* buffer, size_t bytes)
{


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
    default:
      return InvalidMessage;
  }
}


int parseWeatherMessage(byte* buffer)
{
  WeatherMessageType message;
  memcpy(&message, buffer, sizeof(WeatherMessageType));
}

int destroyReceiver(int receiver_fd)
{
  close(receiver_fd);
}

int destroyClient(int client_fd)
{
  close(client_fd);
}

int waitForClient(int server_fd)
{
  struct sockaddr_in remoteAddress;
  socklen_t remoteLength = sizeof (remoteAddress);
  return accept(server_fd, (struct sockaddr *) &remoteAddress, &remoteLength);
}

int readFromClient(int client_fd, char* buffer, int bytes)
{
  bzero(buffer,bytes+1);
  if (read(client_fd, buffer, bytes) >= 0)
    cout << buffer << endl;
}

int createReceiver(int portNumber)
{
  
  int socket_fd, remote_fd, connectionLimit;
  
  socklen_t remoteLength;

  struct sockaddr_in serverAddress, remoteAddress;

  connectionLimit = 3;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd < 0)
    cout << "Error opening socket" << endl;
  
  bzero((char *) &serverAddress, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(portNumber);

  if (bind(socket_fd, (struct sockaddr *) &serverAddress, sizeof (serverAddress)) < 0)
    cout << "Error on bind" << endl;

  listen(socket_fd, connectionLimit);

  return socket_fd;

  //remoteLength = sizeof (remoteAddress);

  //remote_fd = accept(socket_fd, (struct sockaddr *) &remoteAddress, &remoteLength);

  //if (remote_fd < 0)
  //  cout << "Error on accept" << endl;

  //bzero(buffer,256);
  //if (read(remote_fd, buffer, 255) >= 0)
  //  cout << buffer << endl;
  //else
  //  cout << "Error on read" << endl;

  //close(remote_fd);
  //close(socket_fd);

}




