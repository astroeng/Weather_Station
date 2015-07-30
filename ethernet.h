#ifndef ETHERNET_H
#define ETHERNET_H

#include <Arduino.h>
#include <Ethernet.h>

#define ETHERNET_CONNECTION_SUCCESS 1
#define ETHERNET_OK 0
#define ETHERNET_CONNECTION_FAILED -1
#define ETHERNET_ERROR -2

/* Setup the network parameters */
static byte localMAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD};
static IPAddress ipAddress(192,168,123,123);
static IPAddress networkDNS(192,168,123,5);

static boolean ethernetRunning = false;

class HTTP_Connection
{
public:
  HTTP_Connection(char* serverURL, unsigned int connectionPort);
  int begin();
  int close();
  int sendGetRequest(String* requestString);
private:
  char* server;
  unsigned int port;
  EthernetClient* client;
};

#endif
