/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef ETHERNET_H
#define ETHERNET_H

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>


#define ETHERNET_WAS_DISCONNECTED 3
#define ETHERNET_WAS_CONNECTED 2
#define ETHERNET_CONNECTION_SUCCESS 1
#define ETHERNET_OK 0
#define ETHERNET_CONNECTION_FAILED -1
#define ETHERNET_ERROR -2


class TCP_Client
{
public:
  TCP_Client(IPAddress serverIP, unsigned int connectionPort);
  int begin();
  int close();
  int sendData(char* dataArray, int length);
  char* receiveResponse(char* output);

private:
  IPAddress server;
  unsigned int port;
  EthernetClient* client;
  EthernetUDP Udp;
};

#endif
