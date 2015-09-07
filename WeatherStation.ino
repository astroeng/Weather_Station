#include <software_i2c.h>
#include <bmp180.h>
#include <htu21d.h>
#include <primitive_scheduler.h>

#include "ethernet.h"
#include "sparkfun_log.h"

/* These apparently HAVE to be included here for anything to work.... */
#include <SPI.h>
#include <Ethernet.h>

#define FILTER_CONSTANT_A 0.90
#define FILTER_CONSTANT_B 0.10


/* One of my bigest gripes with the Arduino hardware is that the pins
 * used by any given shield are poorly documented.
 * These are the pins used to support just the ethernet shield. In these
 * pins is buried an SPI interface... The documentation does not describe
 * which pin is used for clock, data in, or data out... Those details have
 * been gleened from the code.
 */
#define ETHERNET_SHIELD_PIN_A 10 /* Ethernet Select */
#define ETHERNET_SHIELD_PIN_B 11
#define ETHERNET_SHIELD_PIN_C 12
#define ETHERNET_SHIELD_PIN_D 13
#define ETHERNET_SHIELD_SD_SEL  4  /* SD Select */
#define ETHERNET_SHIELD_INTERRUPT 2

/* Sensor pins. */
#define BMP180_CLOCK A4
#define BMP180_DATA  A5

#define HTU21D_CLOCK A3
#define HTU21D_DATA  A2



#define PASCAL_TO_PSI 0.145037738
#define PASCAL_TO_INHG 0.295333727
#define C_TO_F(x) ((9*x)/5)+3200

const char server[] = "data.sparkfun.com";
char logString[250];

HTTP_Connection sparkfun_logger((char*)server, 80);

PrimitiveScheduler schedule(4);

Software_I2C bmp180_bus(BMP180_DATA, 
                        BMP180_CLOCK, 
                        100);

Software_I2C htu21d_bus(HTU21D_DATA, 
                        HTU21D_CLOCK, 
                        50);

BMP180 bmp180(&bmp180_bus); /* temperature and pressure sensor */
HTU21D htu21d(&htu21d_bus); /* temperature and humidity sensor */

long filtered_temperature; /* Degrees Celsius (Two decimal) */
long filtered_humidity;    /* Percent (Two decimal) */
long filtered_pressure;    /* Pascal */

void setup()
{ 
  Serial.begin(9600);
  delay(500);
  
  Serial.println("Startup....");
  pinMode(ETHERNET_SHIELD_SD_SEL,OUTPUT);
  digitalWrite(ETHERNET_SHIELD_SD_SEL,HIGH);

  /* Read the sensors once per second. Given that the longest sensor state-machine
   * is 6 states it will take 6 calls to cycle through all of the states in a 
   * sensor. With a sensor filter of 0.95 it will take 20 cycles to move through
   * a filter cycle. So the data should be logged about once every 2 minutes.
   */
  
  schedule.addTask(readSensors, 1000); /*  0.01 seconds */ 
  schedule.addTask(logData,   120000); /*  120.00 seconds */
  schedule.addTask(outputData, 6000);
  schedule.addTask(memCheck,   60000); /*  5.00 seconds */
  
  bmp180.begin();
  htu21d.begin();
  
  sparkfun_logger.begin();

  filtered_temperature = 0;
  filtered_humidity = 0;
  filtered_pressure = 0;

  Serial.println("Finished");

}

void loop()
{
  schedule.run();
}

long updateValue(long filtered_value, long working_value)
{
  /* If the filtered_value is within a certain tolerance of the working value then updated the
   * filtered value using the filter function. Else, overwrite the filtered value with the 
   * working value.
   */
  if (abs(filtered_value - working_value) < (working_value * FILTER_CONSTANT_B))
  {
    return FILTER_CONSTANT_A * filtered_value + FILTER_CONSTANT_B * working_value;
  }
  else
  {
    return working_value;
  }  
}

void readSensors( void )
{
  long working_var;
  char state;

  /* Run the pressure sensor, if the pressure sensor just read pressure then use it
   * in the filtered pressure reading.
   */
  
  state = bmp180.run();

  if (state == BMP180_Read_Pressure)
  {
    working_var = bmp180.getPressure();
    filtered_pressure = updateValue(filtered_pressure,working_var);
  }

  /* Run the humidity sensor, if the humidity sensor just read humidity or temperature
   * use the new value in the appropriate filter.
   */
  
  state = htu21d.run();

  if (state == HTU21D_RetrieveHumidity)
  {
    working_var = htu21d.getHumidity();
    filtered_humidity = updateValue(filtered_humidity,working_var);
  }
  else if (state == HTU21D_RetrieveTemperature)
  {
    working_var = htu21d.getTemperature();
    filtered_temperature = updateValue(filtered_temperature,working_var);
  }

}

void memCheck( void )
{
  
  char * value = new char;
  Serial.println((int)value,HEX);
  delete value;

  Serial.println(schedule.getTaskExecutionTime(0));
  Serial.println(schedule.getTaskExecutionTime(1));
  Serial.println(schedule.getTaskExecutionTime(2));
  Serial.println(schedule.getTaskExecutionTime(3));
}

void logData()
{
  char looper;
  char ethernetStatus;
  char ethernetClosed;

  ethernetClosed = sparkfun_logger.close();

  createLoggingString(filtered_pressure*PASCAL_TO_INHG,
                      filtered_humidity,
                      C_TO_F(filtered_temperature),
                      0,
                      0,
                      0,
                      0,
                      0,
                      logString);

  for (looper = 0; looper < 3; looper++)
  {
    ethernetStatus = sparkfun_logger.sendGetRequest(logString);
    if (ethernetStatus == ETHERNET_CONNECTION_SUCCESS)
    {
      break;
    }
  }
  
  Serial.print("Ethernet Status: ");
  Serial.print(ethernetStatus,HEX);
  Serial.print(" ");
  Serial.print(ethernetClosed,HEX);
  Serial.print(" ");
  Serial.println(looper,HEX);

}

void outputData ( void )
{
  Serial.print("Pressure   : ");
  Serial.println(filtered_pressure*PASCAL_TO_INHG);
  Serial.print("Humidity   : ");
  Serial.println(filtered_humidity);
  Serial.print("Temperature: ");
  Serial.println(C_TO_F(filtered_temperature));
}
