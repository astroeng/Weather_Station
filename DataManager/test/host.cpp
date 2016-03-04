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

#include <netdb.h>

int main(int argc, char * argv[])
{
  struct hostent *server;

  server = gethostbyname(argv[1]);

  cout << server->h_length << endl;
  cout << ((unsigned int)server->h_addr[0] & 0xff) << endl;
  cout << ((unsigned int)server->h_addr[1] & 0xff) << endl;
  cout << ((unsigned int)server->h_addr[2] & 0xff) << endl;
  cout << ((unsigned int)server->h_addr[3] & 0xff) << endl;


}
