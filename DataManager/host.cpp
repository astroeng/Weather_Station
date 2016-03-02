
#include <netdb.h>

#include <iostream>

using namespace std;

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
