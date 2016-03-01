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

using namespace std;

#include <netdb.h>

#include "tcp_client.h"

TCP_Client::TCP_Client(char* host, int port)
{
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  port_no = port;
  server = gethostbyname(host);

  bzero((char*)&serverAddress, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;

  bcopy((char*)server->h_addr, (char*)&serverAddress.sin_addr.s_addr,server->h_length);

  serverAddress.sin_port = htons(port_no);

  connect(socket_fd,(struct sockaddr *) &serverAddress, sizeof (serverAddress));
}

TCP_Client::~TCP_Client()
{
  close(socket_fd);
}

int TCP_Client::sendTo(char* buffer, int bytes)
{
  return write(socket_fd, buffer, bytes);
}

int TCP_Client::readFrom(char* buffer, int bytes)
{
  int bytesRead = 0;
  while (bytesRead < bytes)
  {
    bytesRead += read(socket_fd, buffer, bytes-bytesRead);
  }
  return bytesRead;
}
