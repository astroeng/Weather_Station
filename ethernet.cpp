#include <Ethernet.h>
#include "ethernet.h"

#define TEST(x) 

HTTP_Connection::HTTP_Connection(char* serverURL, unsigned int connectionPort)
{
  server = serverURL;
  port   = connectionPort;
}


/* This will start the Ardunio Ethernet library for the first connection that
 * calls begin. Even though it is not needed every connection should call this 
 * function.
 */
int HTTP_Connection::begin()
{
  int statusReturn = ETHERNET_OK;
  
  client = new EthernetClient();
  
  if (ethernetRunning != true)
  {
    //Ethernet.begin(localMAC,ipAddress,networkDNS);
    /* This will use DHCP */
    Ethernet.begin(localMAC);
    //delay(1000);
    
    ethernetRunning = true;
  }
  
  return statusReturn;
}


/* Check to see if the client connection is active. If it is not then the server
 * has dropped the connection and the client should be stopped. 
 */
int HTTP_Connection::close()
{
  int statusReturn = ETHERNET_ERROR;
  
  if (!client->connected())
  {
    client->stop();
    statusReturn = ETHERNET_WAS_DISCONNECTED;
  }
  else
  {
    client->stop();
    statusReturn = ETHERNET_WAS_CONNECTED;
  }
  
  return statusReturn;
}


/* This will send a string as part of a formatted GET request.
 * The server will be instructed to close the connection since
 * this only needs the basic reply.
 */
int HTTP_Connection::sendGetRequest(char* requestString)
{
  /* Try to connect and send a GET request.
   */
  if (client->connect(server, port))
  {
    //String getRequest = "GET " + (*requestString) + " HTTP/1.1";

    /* Connected so send the request.
     */
    client->print("GET ");
    client->print(requestString);
    client->println(" HTTP/1.1");
    client->println(hostString);
    client->println(userAgent);
    client->println(connectionClose);
    client->println();
    
    TEST(Serial.print("GET "));
    TEST(Serial.print(requestString));
    TEST(Serial.println(" HTTP/1.1"));
    TEST(Serial.println(hostString));
    TEST(Serial.println(userAgent));
    TEST(Serial.println(connectionClose));
    TEST(Serial.println());
    
    return ETHERNET_CONNECTION_SUCCESS;
  }
  /* Since the connection failed stop the client and report an error.
   */
  client->stop();
  return ETHERNET_CONNECTION_FAILED;
}


/* This will read off a response to a request.
 */
char* HTTP_Connection::receiveResponse(char* output)
{
  while (client->available() > 0)
  {
    (*output) += client->read();
  }
  return output;
}
