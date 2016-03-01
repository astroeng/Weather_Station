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

#include "client_interface.h"

Client_Interface::Client_Interface(int server_fd)
{
  socklen_t remoteLength = sizeof (remoteAddress);
  client_fd = accept(server_fd, (struct sockaddr *) &remoteAddress, &remoteLength);
}


Client_Interface::~Client_Interface()
{
  close(client_fd);
}


int Client_Interface::readFrom(byte* buffer, int bytes)
{
  int bytesRead = 0;
  while (bytesRead < bytes)
  {
    bytesRead += read(client_fd, buffer, bytes-bytesRead);
  }
  return bytesRead;
}


int Client_Interface::replyTo(byte* buffer, int bytes)
{
  return write(client_fd, buffer, bytes);
}
