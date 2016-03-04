/* Derek Schacht
 *  2016 02 29
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */


#ifndef TCP_INTERFACE_H
#define TCP_INTERFACE_H

#include <netinet/in.h>

#include "client_interface.h"

typedef enum
{
  TCP_ServerSocketFailure,
  TCP_ServerBindFailure,
  TCP_ServerListenFailure,
} TCP_ServerExceptionsType;

class TCP_Server
{
public:
  TCP_Server(int portNumber, int connectionLimit);
  ~TCP_Server();
  Client_Interface accept();
private:
  int socket_fd;
  struct sockaddr_in serverAddress;
};



#endif
