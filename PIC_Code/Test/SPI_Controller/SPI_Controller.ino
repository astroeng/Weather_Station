#include <SPI.h>

const int slaveSelectPin = 10;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin();
  Serial.begin(9600);
  delay(1000);
  go();
}

int values[] = {125,221,111,45};
int i;

void go() {}

void loop() {
  // put your main code here, to run repeatedly:
  writeSpiBytes(values);
  delay(100);
  for (i = 0; i < 4; i++)
  {
    values[i]++;  
  }
}

void writeSpiBytes(int* value) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin, LOW);
  delayMicroseconds(10);
  //  send in the address and value via SPI:
  Serial.println(SPI.transfer(value[0]));
  delayMicroseconds(10);
  Serial.println(SPI.transfer(value[1]));
  delayMicroseconds(10);
  Serial.println(SPI.transfer(value[2]));
  delayMicroseconds(10);
  Serial.println(SPI.transfer(value[3]));
  delayMicroseconds(10);
  Serial.println(SPI.transfer(0));
  // take the SS pin high to de-select the chip:
  delayMicroseconds(10);
  digitalWrite(slaveSelectPin, HIGH);
}
