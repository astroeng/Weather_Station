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

#include <unistd.h>

using namespace std;

#include <netdb.h>

#include "tcp_client.h"
#include "system_error.h"

#define DEBUG(x) //x

/* Create a TCP Client for a given host and port.
 * This will setup the socket and make the connection.
 * If either of the steps fail an exception will be thrown.
 *
 * Point of discussion... I choose to have these failable methods here
 * since if a "createAndConnect" method was part of this class the
 * object could just be reused for multiple connections. Keeping this
 * in the constructor enforces the usecase of this class.
 */

TCP_Client::TCP_Client(const char* host, const char* port)
{
  struct addrinfo hostLookupHints = {};
  struct addrinfo *hostLookupResults;
  
  hostLookupHints.ai_family    = AF_INET;
  hostLookupHints.ai_socktype  = SOCK_STREAM;
  hostLookupHints.ai_flags     = 0;
  hostLookupHints.ai_protocol  = 0;
  hostLookupHints.ai_canonname = NULL;
  hostLookupHints.ai_addr      = NULL;
  hostLookupHints.ai_next      = NULL;

  socket_fd = socket(hostLookupHints.ai_family, hostLookupHints.ai_socktype, 0);
  
  if (socket_fd < 0)
  {
    outputError("TCP_CLIENT: Socket Error!");
    throw TCP_ClientSocketFailure;
  }

  DEBUG(cout << "TC: Created Socket" << endl);
  
  if (getaddrinfo(host, port, &hostLookupHints, &hostLookupResults) != 0)
  {
    char error[128] = {};
    sprintf(error, "TCP_CLIENT: getaddrinfo Failed %s:%s", host, port);
    outputError(error);

    throw TCP_ClientHostNameFailure;
  }
  
  DEBUG(cout << "TC: Got Host" << endl);
  
  // getaddrinfo returns a linked list of possible results. Loop through
  // them and attempt a connection, break on the first successful result

  for (struct addrinfo *results_i = hostLookupResults; results_i != NULL; results_i = results_i->ai_next)
  {
    DEBUG(cout << "TC: Address " << ((struct sockaddr_in*)results_i->ai_addr)->sin_addr.s_addr << endl);
  
    if (connect(socket_fd,results_i->ai_addr, results_i->ai_addrlen) == 0)
    {
      DEBUG(cout << "TC: Connected" << endl);

      // The addrinfo results are no longer needed.
      freeaddrinfo(hostLookupResults);
      return;
    }
    DEBUG(else
    {
      cout << "TC: Not Connected" << endl;
    })

  }

  // The addrinfo results are no longer needed.
  freeaddrinfo(hostLookupResults);

  // Getting here is an error, it means that the program wasnt able to
  // connect to any of the results from getaddrinfo 
  outputError("TCP_CLIENT: Connect Error!");
  throw TCP_ClientConnectFailure;

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
