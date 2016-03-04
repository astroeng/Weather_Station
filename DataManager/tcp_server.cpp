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

using namespace std;

#include "tcp_server.h"
#include "system_error.h"


TCP_Server::TCP_Server(int portNumber, int connectionLimit)
{
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd < 0)
  {
    outputError("TCP_SERVER: Socket Error!");
    throw TCP_ServerSocketFailure;
  }
  
  bzero((char *) &serverAddress, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(portNumber);

  if (bind(socket_fd, (struct sockaddr *) &serverAddress, sizeof (serverAddress)) < 0)
  {
    outputError("TCP_SERVER: Bind Error!");
    throw TCP_ServerBindFailure;
  }

  if (listen(socket_fd, connectionLimit) != 0)
  {
    outputError("TCP_SERVER: Listen Error!");
    throw TCP_ServerListenFailure;
  }

}

TCP_Server::~TCP_Server()
{
  close(socket_fd);
}

Client_Interface TCP_Server::accept()
{
  return Client_Interface(socket_fd);
}
