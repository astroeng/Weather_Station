/* Derek Schacht
 *  2016 02 29
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */


#ifndef CLIENT_INTERFACE_H
#define CLIENT_INTERFACE_H

#include "system_types.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Client_Interface
{
  public:
    Client_Interface(int server_fd);
    ~Client_Interface();
    int readFrom(byte* buffer, int bytes);
    int replyTo(byte* buffer, int bytes);

  private:
    int client_fd;
    struct sockaddr_in remoteAddress;
};

#endif
