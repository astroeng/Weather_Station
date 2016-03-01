/* Derek Schacht
 *  2016 02 29
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class TCP_Client
{
public:
  TCP_Client(char* host, int port);
  ~TCP_Client();
  int sendTo(char* buffer, int bytes);
  int readFrom(char* buffer, int bytes);
private:
  int socket_fd;
  int port_no;
  struct sockaddr_in serverAddress;
  struct hostent *server;
};

#endif
