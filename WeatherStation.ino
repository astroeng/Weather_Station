#include <software_i2c.h>
#include <bmp180.h>
#include <htu21d.h>
#include <argent_80422.h>
#include <primitive_scheduler.h>

#include "ethernet.h"
#include "sparkfun_log.h"

/* These apparently HAVE to be included here for anything to work.... */
#include <SPI.h>
#include <Ethernet.h>

#define FILTER_CONSTANT_A 0.90
#define FILTER_CONSTANT_B 0.10

#define TEST(x)
//#define TEST(x) x

/* http://data.sparkfun.com/highlands_weather */

/* One of my biggest gripes with the Arduino hardware is that the pins
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
/* I2C is used for the BMP180 pressure sensor as well as the HTU21D humidity sensor. */
#define I2C_CLOCK A5
#define I2C_DATA  A4

#define PASCAL_TO_PSI 0.145037738
#define PASCAL_TO_INHG 0.295333727
#define C_TO_F(x) ((9*x)/5)+3200

const char server[] = "data.sparkfun.com";
char logString[160];

HTTP_Connection sparkfun_logger((char*)server, 80);

PrimitiveScheduler schedule(6);

Software_I2C i2c_bus(I2C_DATA, 
                     I2C_CLOCK, 
                     1000);

BMP180 bmp180(&i2c_bus); /* temperature and pressure sensor */
HTU21D htu21d(&i2c_bus); /* temperature and humidity sensor */

Argent_80422 weatherVane(A2,2,3); /* wind speed, wind direction, and rain fall sensor */

int filtered_temperature;            /* Degrees Celsius (Two decimal) */
unsigned int filtered_humidity;      /* Percent         (Two decimal) */
long filtered_pressure;              /* Pascal */
unsigned int filtered_windSpeed;     /* MPH             (Two decimal) */
unsigned int max_windSpeed;          /* MPH             (Two decimal) */
unsigned int filtered_windDirection; /* Degrees         (Two decimal) */

/* ISR declaration, it is just way easier to put these
 * in the top level code file.
 */
void localWindSpeed_ISR()
{
  weatherVane.windSpeed_ISR();
}
void localRainFall_ISR()
{
  weatherVane.rainFall_ISR();
}

void setup()
{ 
  //Serial.begin(9600);
  delay(500);
  
  //Serial.print("S....");
  pinMode(A0,OUTPUT);
  pinMode(ETHERNET_SHIELD_SD_SEL,OUTPUT);
  digitalWrite(A0,HIGH);
  digitalWrite(ETHERNET_SHIELD_SD_SEL,HIGH);

  /* Read the sensors once per second. Given that the longest sensor state-machine
   * is 6 states it will take 6 calls to cycle through all of the states in a 
   * sensor. With a sensor filter of 0.95 it will take 20 cycles to move through
   * a filter cycle. So the data should be logged about once every 2 minutes.
   */

  schedule.addTask(readDiscreteSensors, 10000); /*  10.0 seconds */
  schedule.addTask(readI2CSensors,       1000); /*  1.0 seconds */ 
  schedule.addTask(logData,            120000); /*  120.00 seconds */
  schedule.addTask(outputData,           5000);
  TEST(schedule.addTask(memCheck,       60000)); /*  60.00 seconds */
  
  bmp180.begin();
  htu21d.begin();
  
  sparkfun_logger.begin();

  weatherVane.begin();

  filtered_temperature = 0;
  filtered_humidity = 0;
  filtered_pressure = 0;
  filtered_windSpeed = 0;
  max_windSpeed = 0;

  attachInterrupt(digitalPinToInterrupt(2), localWindSpeed_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), localRainFall_ISR, FALLING);
  
  //Serial.println("F");

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

void readDiscreteSensors()
{
  long working_var;
  
  working_var        = weatherVane.getWindSpeed();
  /* TODO: Implement something that works with circular values... */
  filtered_windSpeed = updateValue(filtered_windSpeed, working_var);
  max_windSpeed      = max(max_windSpeed,working_var);

  working_var            = weatherVane.getWindDirection();
  filtered_windDirection = updateValue(filtered_windDirection, working_var);
  
}

void readI2CSensors( void )
{
  long working_var;
  char state;
  
  /* Run the pressure sensor, if the pressure sensor just read pressure then use it
   * in the filtered pressure reading.
   */
  
  state = bmp180.run();

  if (state == BMP180_Read_Pressure)
  {
    working_var       = bmp180.getPressure();
    filtered_pressure = updateValue(filtered_pressure, working_var);
  }
  else if (state == BMP180_Read_Temperature)
  {
    working_var          = bmp180.getTemperature();
    filtered_temperature = updateValue(filtered_temperature,working_var);
  }

  /* Run the humidity sensor, if the humidity sensor just read humidity or temperature
   * use the new value in the appropriate filter.
   */
  
  state = htu21d.run();

  if (state == HTU21D_RetrieveHumidity)
  {
    working_var       = htu21d.getHumidity();
    filtered_humidity = updateValue(filtered_humidity,working_var);
  }
  else if (state == HTU21D_RetrieveTemperature)
  {
    working_var          = htu21d.getTemperature();
    filtered_temperature = updateValue(filtered_temperature,working_var);
  }

}
TEST(
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
);
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
                      filtered_windSpeed,
                      filtered_windDirection,
                      logString);

  for (looper = 0; looper < 3; looper++)
  {
    ethernetStatus = sparkfun_logger.sendGetRequest(logString);
    if (ethernetStatus == ETHERNET_CONNECTION_SUCCESS)
    {
      break;
    }
  }

  max_windSpeed = 0;
  
  //Serial.print("ES: ");
  //Serial.print(ethernetStatus,HEX);
  //Serial.print(" ");
  //Serial.print(ethernetClosed,HEX);
  //Serial.print(" ");
  //Serial.println(looper,HEX);

}

void outputData ( void )
{
//  Serial.print("P  : ");
//  Serial.println((unsigned int)(filtered_pressure*PASCAL_TO_INHG));
//  Serial.print("H  : ");
//  Serial.println(filtered_humidity);
//  Serial.print("T  : ");
//  Serial.println(C_TO_F(filtered_temperature));
//  Serial.print("WS : ");
//  Serial.println(filtered_windSpeed);
//  Serial.print("MWS: ");
//  Serial.println(max_windSpeed);
//  Serial.print("WDD: ");
//  Serial.println(filtered_windDirection);
}
