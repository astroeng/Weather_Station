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
#include "system_error.h"

/* Create a TCP Client for a given host and port.
 * This will setup the socket and make the connection.
 * If either of the steps fail an exception will be thrown.
 *
 * Point of discussion... I choose to have these failable methods here
 * since if a "createAndConnect" method was part of this class the
 * object could just be reused for multiple connections. Keeping this
 * in the constructor enforces the use of this class.
 */

TCP_Client::TCP_Client(const char* host, unsigned short port)
{
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
  {
    outputError("TCP_CLIENT: Socket Error!");
    throw TCP_ClientSocketFailure;
  }

  bzero((char*)&serverAddress, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;

  server = gethostbyname(host);
  bcopy((char*)server->h_addr, (char*)&serverAddress.sin_addr.s_addr,server->h_length);

  port_no = port;
  serverAddress.sin_port = htons(port_no);

  int connected = connect(socket_fd,(struct sockaddr *) &serverAddress, sizeof (serverAddress));
  if (connected != 0)
  {
    outputError("TCP_CLIENT: Connect Error!");
    throw TCP_ClientConnectFailure;
  }

}

/* Perform the needs of object destruction.
 * Basically just close the socket.
 */

TCP_Client::~TCP_Client()
{
  close(socket_fd);
}

/* Send the data to the server.
 */
int TCP_Client::sendData(const char* buffer, int bytes)
{
  return send(socket_fd, buffer, bytes , 0);
}

/* Read data from the server. This will return the requested amount
 * of data unless the internal recv timeout happens.
 */
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
