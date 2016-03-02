/* Derek Schacht
 *  2016 02 29
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

#include <netdb.h>

#include "tcp_client.h"

TCP_Client::TCP_Client(char* host, unsigned short port)
{
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
  {
    cout << "Error with socket" << endl;
  }

  port_no = port;
  server = gethostbyname(host);

  bzero((char*)&serverAddress, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;

  bcopy((char*)server->h_addr, (char*)&serverAddress.sin_addr.s_addr,server->h_length);

  //serverAddress.sin_addr.s_addr = htons(serverAddress.sin_addr.s_addr);
  serverAddress.sin_port = htons(port_no);

  int connected = connect(socket_fd,(struct sockaddr *) &serverAddress, sizeof (serverAddress));

  if (connected < 0)
  {
    char buffer[100];
    bzero(buffer, 100);
    cout << "Error connecting" << endl;
    perror(buffer);
    cout << buffer << endl;
  }

  //cout << "SOCKET: " <<  socket_fd  << " CONNECTED: " << connected << endl;
}

TCP_Client::~TCP_Client()
{
  close(socket_fd);
}

int TCP_Client::sendData(const char* buffer, int bytes)
{
  //cout << socket_fd << " " << bytes << endl;
  return send(socket_fd, buffer, bytes , 0);
}

int TCP_Client::readData(char* buffer, int bytes)
{
  int bytesRead = 0;
  while (bytesRead < bytes)
  {
    int currentRead = recv(socket_fd, &buffer[bytesRead], bytes-bytesRead, 0);
    bytesRead += currentRead;

    if (currentRead == 0)
    {
      break;
    }

  }
  return bytesRead;
}
