#include <Ethernet.h>
#include "ethernet.h"

HTTP_Connection::HTTP_Connection(char* serverURL, unsigned int connectionPort)
{
  server = serverURL;
  port = connectionPort;
  client = new EthernetClient();

}

int HTTP_Connection::begin()
{
  int statusReturn = ETHERNET_OK;
  
  if (ethernetRunning != true)
  {
    Ethernet.begin(localMAC,ipAddress,networkDNS);
    delay(1000);
    
    ethernetRunning = true;
  }
  
  return ETHERNET_OK;
}

const PROGMEM String hostString = "Host: data.sparkfun.com";
const PROGMEM String userAgent = "User-Agent: arduino-ethernet";
const PROGMEM String connectionClose = "Connection: close";

int HTTP_Connection::sendGetRequest(String* requestString)
{
  if (client->connect(server, port))
  {
    String getRequest = "GET " + *requestString + " HTTP/1.1";

    // send the HTTP PUT request:
    client->println(getRequest);
    client->println(hostString);
    client->println(userAgent);
    client->println(connectionClose);
    client->println();
    
    return ETHERNET_CONNECTION_SUCCESS;
  }
  
  return ETHERNET_CONNECTION_FAILED;
}
