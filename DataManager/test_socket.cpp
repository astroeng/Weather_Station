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

using namespace std;

#include "tcp_client.h"
#include "string.h"

int main()
{

  char buffer[1025];
  bzero(buffer, 1025);

  char request[] = "GET /\0";

  char request2[] = "input/G2JQdM3d69hjr5Z2arQj?private_key=NWP87EK7bjHN6RZJ56kN&mc=825307441&t1a_ms=808464432&t1m_ms=842150450&t2a_ms=858993459&t2m_ms=875836468&t3a_ms=892679477&t3m_ms=909522485&t4a_ms=926365495&t4m_ms=943208504&bv_v=1234&upt_s=168649057\0";

  char request3[] = " HTTP/1.1\r\n\0";
  char request4[] = "Host: data.sparkfun.com\r\n\0";
  char request5[] = "User-Agent: arduino-ethernet\r\n\0";
  char request6[] = "Connection: keep-alive\r\n\r\n\0";

  TCP_Client sparkfun("data.sparkfun.com\0", 80);
  cout << sparkfun.sendData(request, strlen(request)) << endl;
  cout << sparkfun.sendData(request2, strlen(request2)) << endl;
  cout << sparkfun.sendData(request3, strlen(request3)) << endl;
  cout << sparkfun.sendData(request4, strlen(request4)) << endl;
  cout << sparkfun.sendData(request5, strlen(request5)) << endl;
  cout << sparkfun.sendData(request6, strlen(request6)) << endl;
  sparkfun.readData(buffer, 1024);


  return 0;

}
