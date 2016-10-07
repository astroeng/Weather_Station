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
#include <cstdio>

#include <unistd.h>

using namespace std;

#include <netdb.h>

#include "client_interface.h"
#include "system_error.h"

#define DEBUG(x) //x

// This constructor establishes a UDP connection on a given port.

Client_Interface::Client_Interface(const char* port)
{
  struct addrinfo hostLookupHints = {};
  struct addrinfo *hostLookupResults;
  
  hostLookupHints.ai_family    = AF_INET;
  hostLookupHints.ai_socktype  = SOCK_DGRAM;
  hostLookupHints.ai_flags     = AI_PASSIVE;
  hostLookupHints.ai_protocol  = IPPROTO_UDP;
  hostLookupHints.ai_canonname = NULL;
  hostLookupHints.ai_addr      = NULL;
  hostLookupHints.ai_next      = NULL;
  
  client_fd = socket(hostLookupHints.ai_family, 
                     hostLookupHints.ai_socktype, 
                     hostLookupHints.ai_protocol);
  
  if (getaddrinfo(NULL, port, &hostLookupHints, &hostLookupResults) != 0)
  {
    char error[128] = {};
    sprintf(error, "CLIENT_INTERFACE: getaddrinfo Failed %s:%s", "ANY", port);
    outputError(error);
    throw Client_InterfaceAddressError;
  }
  
  // getaddrinfo returns a linked list of possible results. Loop through
  // them and attempt a connection, break on the first successful result

  for (struct addrinfo *results_i = hostLookupResults; results_i != NULL; results_i = results_i->ai_next)
  {
    DEBUG(cout << "CI: Address " << ((struct sockaddr_in*)results_i->ai_addr)->sin_addr.s_addr << endl);
  
    if (bind(client_fd,results_i->ai_addr, results_i->ai_addrlen) == 0)
    {
      DEBUG(cout << "CI: Connected" << endl);
      // The addrinfo results are no longer needed.
      freeaddrinfo(hostLookupResults);
      return;
    }
  }

  // If the code reaches this point it means that bind did not succeed. Free
  // up any memory and report the issue.

  freeaddrinfo(hostLookupResults);
  outputError("CLIENT_INTERFACE: UDP Bind Failed");
  throw Client_InterfaceBindError;

}


// This constructor is intended to be used to work with TCP connections. Via 
// the TCP Server class.

Client_Interface::Client_Interface(int server_fd)
{
  socklen_t remoteLength = sizeof (remoteAddress);

  client_fd = accept(server_fd, (struct sockaddr *) &remoteAddress, &remoteLength);

  if (client_fd < 0)
  {
    outputError("CLIENT_INTERFACE: Accept Error!");
    throw Client_InterfaceAcceptError;
  }
}


Client_Interface::~Client_Interface()
{
  close(client_fd);
}


int Client_Interface::readFrom(byte* buffer, int bytes)
{
  return read(client_fd, buffer, bytes);
}


int Client_Interface::replyTo(byte* buffer, int bytes)
{
  return write(client_fd, buffer, bytes);
}
