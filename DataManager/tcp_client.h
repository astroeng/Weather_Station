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

typedef enum
{
  TCP_ClientSocketFailure,
  TCP_ClientConnectFailure,
} TCP_ClientExceptionsType;

class TCP_Client
{
public:
  TCP_Client(char* host, unsigned short port);
  ~TCP_Client();
  int sendData(const char* buffer, int bytes);
  int readData(char* buffer, int bytes);
private:
  int socket_fd;
  unsigned short port_no;
  struct sockaddr_in serverAddress;
  struct hostent *server;
};

#endif
