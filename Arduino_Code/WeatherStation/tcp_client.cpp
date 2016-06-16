/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <utility/w5100.h>

#include "tcp_client.h"
#include "utilities.h"

#define TEST(x)

/* Setup the network parameters */
byte localMAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD};

//const IPAddress ipAddress(192,168,2,100);
//const IPAddress networkDNS(192,168,2,1);

boolean ethernetRunning = false;

TCP_Client::TCP_Client(IPAddress serverIP,  unsigned int connectionPort)
{
  server = serverIP;
  port   = connectionPort;
}


/* This will start the Ardunio Ethernet library for the first connection that
 * calls begin. Even though it is not needed every connection should call this
 * function.
 */
int TCP_Client::begin()
{
  int statusReturn = ETHERNET_OK;

  client = new EthernetClient();

  if (ethernetRunning != true)
  {

    /* Static address configuration */
    //Ethernet.begin(localMAC,ipAddress,networkDNS);

    /* This will use DHCP, and use a lot more memory */
    Ethernet.begin(localMAC);
    Udp.begin(1234);

    ethernetRunning = true;
    TEST(Serial.println("Ethernet UP"));
  }
  /* Set some tuning parameters since the default settings are
   * way too slow.
   */
  client->setTimeout(1000) ;
  W5100.setRetransmissionTime(2000);
  W5100.setRetransmissionCount(2);

  return statusReturn;
}


/* Check to see if the client connection is active. If it is not then the server
 * has dropped the connection and the client should be stopped.
 */
int TCP_Client::close()
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
int TCP_Client::sendData(char* dataArray, int length)
{


  Udp.beginPacket(server,port);
  Udp.write(dataArray, length);
  Udp.endPacket();

  /* Try to connect and send a GET request. */
  //if (client->connect(server, port))
  //{
  //  client->write(dataArray, length);

  //  delay(10);

  //  client->stop();

  //  return ETHERNET_CONNECTION_SUCCESS;
  //}
  //else
  //{
  //  TEST(Serial.println(F("Ethernet Connection Failed")));
  //  TEST(Serial.println());
  //  TEST(Serial.flush());
  //}

  /* Since the connection failed stop the client and report an error. */
  //client->stop();

  //return ETHERNET_CONNECTION_FAILED;
}


/* This will read off a response to a request. */
char* TCP_Client::receiveResponse(char* output)
{
  while (client->available() > 0)
  {
    (*output) += client->read();
  }
  return output;
}
