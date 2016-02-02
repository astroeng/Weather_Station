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


#define ETHERNET_WAS_DISCONNECTED 3
#define ETHERNET_WAS_CONNECTED 2
#define ETHERNET_CONNECTION_SUCCESS 1
#define ETHERNET_OK 0
#define ETHERNET_CONNECTION_FAILED -1
#define ETHERNET_ERROR -2


const char PROGMEM hostString[]      = "Host: data.sparkfun.com";
const char PROGMEM userAgent[]       = "User-Agent: arduino-ethernet";
const char PROGMEM connectionClose[] = "Connection: close";

/* Test for GET request (replace xxxx's and yyyyy's with real keys

"telnet data.sparkfun.com 80" (then pass the following lines as all at the same time.

GET /input/xxxxxxxxxxxxx?private_key=yyyyyyyyyyyy&interval=90&humidity=80&pressure=70&temperature=66&ir_light=50&uv_light=40&white_light=30&wind_direction=20&wind_speed=10 HTTP/1.1
Host: data.sparkfun.com
User-Agent: arduino-ethernet
Connection: close

*/

class HTTP_Connection
{
public:
  HTTP_Connection(char* serverURL, unsigned int connectionPort);
  int begin();
  int close();
  int sendGetRequest(char* requestString);
  char* receiveResponse(char* output);

private:
  char* server;
  unsigned int port;
  EthernetClient* client;
};

#endif
