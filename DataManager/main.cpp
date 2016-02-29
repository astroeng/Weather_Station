
#include <iostream>

using namespace std;

#include "phant_interface.h"
#include "phant_strings.h"
#include "station_interface.h"

int main()
{
  char buffer[256];
  long values[] = {1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE};

  createLoggingString(weatherPublicKey,weatherPrivateKey,values,weatherStrings,14);

  sendLoggingString((int*)values);

  readFromClient(waitForClient(createReceiver(9876)),buffer,50);

  cout << buffer << endl;

  return 0;

}
