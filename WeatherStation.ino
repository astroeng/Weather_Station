/* Home Brew Weather Station */
/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due. 
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find 
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *  
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */
/* This weather station will upload the current data to a phant server every 2 minutes. This is done via a
 * simplified JSON post (full JSON is a bit much for the Arduino. I did try it as there are libraries, but
 * ended up needing the space for other things. As it is this code does work on an Uno R3 but there is not
 * a whole lot of reserve space. This code with the serial stuff not included is about 75% of the program 
 * space on an Uno.. 80% with the serial stuff. 
 * 
 * Overall the design is broken into several tasks that perform the data collection and then publishing to 
 * the phant server hosted by sparkfun. I wrote several support libraries in the effort to build this Weather
 * Station. They are listed below in the includes. Basically, each sensor has a library and I chose a software
 * I2C library so that I was not forced to use certain pins for I2C. I also wanted to have separate I2C buses
 * for the different sensors, but some ended up sharing anyway. I was not concerned with bus performance 
 * since the update rate for the weather station is quite slow. As well as the fact that some of the sensors
 * require milliseconds worth of wait time before their data is valid after a conversion request.
 * (https://github.com/astroeng/SoftwareI2C)
 * 
 * The peripheral architecture is basically the following.
 * 
 * Arduino
 *   Pin 2     - Wind Speed       (pulse)
 *   Pin 3     - Rain Fall        (pulse)
 *   pin A2    - Wind Direction   (analog)
 *   A4 & A5   - HTU21D, BMP180   (i2c)
 *   4 & 10-13 - Ethernet Shield  (spi)
 *   TODO:     - UV Light         (analog)
 *   TODO:     - IR & White Light (i2c)
 *   
 * The task architecture made possible by a library that I wrote called Primitive Scheduler. It
 * basically is initialized with function pointers and an interval on which to run said function. 
 * It makes doing things on regular intervals very easy. More information can be found in the 
 * Scheduler implementation. (https://github.com/astroeng/PrimitiveScheduler)  
 * The tasks are setup like this:
 * Main
 *   Discrete Inputs
 *   I2C Inputs
 *   Data Log
 *   Output Task (Debug)
 *   System Check (Debug)
 *
 * More information about the tasks can be found below in the function descriptions.
 */
/* TODO: List (other than inline TODOs)
 *  Add support for logging to a SD card. There is an uSD card slot on the Ethernet shield that could be
 *  used for the purpose. I worry that the addition of that library might put this over the Uno edge. I
 *  could probably restructure the whole thing to make it fit, but I am sorta attached to this architecture.
 *  
 *  Add support for the RF24 device. This would replace the Ethernet shield and would need to be paired 
 *  with another device that can receive the transmission.
 */
/* Sensors
 *  HTU21D       [ Humidity and Temperature ]  (https://github.com/astroeng/HTU21D)
 *  BMP180       [ Pressure and Temperature ]  (https://github.com/astroeng/BMP180)
 *  Anemometer   [ Wind Speed ]                (https://github.com/astroeng/Argent_80422)
 *  Windvane     [ Wind Direction ]            (same as above)
 *  Rain         [ Rain Fall ]                 (same as above)
 *  IR Light
 *  UV Light
 *  Light
 *  
 * Reported Data
 *  uvLight            TODO:
 *  whiteLight         TODO:
 *  irLight            TODO:
 *  airHumidity        Humidity         (rel%*100)    [ 2 minute average 120 samples ]
 *  airPressure        Pressure         (inHg*1000)   [ 2 minute average 120 samples ]
 *  airTemperature     Temperature      (F*100)       [ 2 minute average 240 samples ]
 *  windSpeed          Wind Speed       (mph*100)     [ 2 minute average 120 samples ]
 *  windDirection      Wind Direction   (bearing*10)  [ 2 minute average 120 samples ]
 *  windSpeed.getStdev Wind Speed Stdev (mph*100)     [ 2 minute Standard Deviation of wind. This is really 4 decimal places, 
 *                                                      (wind speed * 100) * (wind speed * 100) but it is divided by 100 before 
 *                                                      being sent to the logging library.]
 *  windSpeed.getMax   Max Wind Speed   (mph*100)     [ Maximum value observed during 2 minute data collection. Resets with 
 *                                                      every data report. ]
 *  max_windSpeedDir                    (bearing*10)  [ Wind direction observed concurrently with max wind speed ]
 */

#include <software_i2c.h>
#include <bmp180.h>
#include <htu21d.h>
#include <argent_80422.h>
#include <primitive_scheduler.h>
#include <numerical_statistics.h>


#include "ethernet.h"
#include "sparkfun_log.h"
#include <avr/wdt.h>

/* These apparently HAVE to be included HERE for anything to work.... */
#include <SPI.h>
#include <Ethernet.h>


#define SERIAL_T(x)

#define TEST(x)


/* http://data.sparkfun.com/highlands_weather */

/* One of my biggest gripes with the Arduino hardware is that the pins
 * used by any given shield are poorly documented.
 * These are the pins used to support just the Ethernet shield. In these
 * pins is buried an SPI interface... The documentation does not describe
 * which pin is used for clock, data in, or data out... Those details have
 * been gleaned from the code.
 */
#define ETHERNET_SHIELD_PIN_A 10 /* Ethernet Select */
#define ETHERNET_SHIELD_PIN_B 11
#define ETHERNET_SHIELD_PIN_C 12
#define ETHERNET_SHIELD_PIN_D 13
#define ETHERNET_SHIELD_SD_SEL  4  /* SD Select */
#define ETHERNET_SHIELD_INTERRUPT 2

/* Sensor pins. */
/* I2C is used for the BMP180 pressure sensor and the HTU21D humidity sensor. */
#define I2C_CLOCK A5
#define I2C_DATA  A4

/* Analog Pins */
#define BATTERY_PIN A1
#define WIND_DIRECTION A2

/* Discrete Pins */
#define WIND_SPEED 2
#define RAIN_FALL 3

/* Some conversion constants */
#define PASCAL_TO_PSI 0.145037738
#define PASCAL_TO_INHG 0.295333727
#define C_TO_F(x) ((9*x)/5)+3200

#define NUMBER_OF_TASKS 6

/* Setup for the logging task */

char logString[JSON_STRING_LENGTH];

HTTP_Connection sparkfun_logger("data.sparkfun.com", 80);

/* Declare a scheduler */
PrimitiveScheduler schedule(NUMBER_OF_TASKS);

/* Setup a bus and pass it to the units that will use it. This should
 * allow for a seamless transition to a hardware bus in the future. 
 * Provided the hardware bus library implements the same API.
 */
Software_I2C i2c_bus(I2C_DATA, 
                     I2C_CLOCK, 
                     1000);

/* Setup some i2c sensors */
BMP180 bmp180(&i2c_bus); /* temperature and pressure sensor */
HTU21D htu21d(&i2c_bus); /* temperature and humidity sensor */

/* Setup the weatherVane and other gear on it. */
Argent_80422 weatherVane(WIND_DIRECTION, 
                         WIND_SPEED, 
                         RAIN_FALL);


/* Variables for saving the fun numbers. */
typedef enum
{
  station_wind_direction = 0,
  station_wind_speed,
  station_air_pressure,
  station_air_temperature,
  station_air_humidity,
  station_uv_light,
  station_ir_light,
  station_white_light,
  station_collected_data_size
} CollectedDataType;

/* This will be the direction of the wind vane when the maximum wind speed is observed. */
unsigned int max_windSpeedDir;

Numerical_Statistics collectedData[station_collected_data_size];

/* Some system statistics classes. The task run time for the core tasks is recorded and
 * uploaded to it's own datastream independent of the weather data stream. The battery 
 * voltage is also included in the system information.
 */
Numerical_Statistics taskRunTime[4];
Numerical_Statistics batteryVoltage;
unsigned long systemMessageCount = 0;
unsigned long weatherMessageCount = 0;
unsigned long weatherLogTime = 0;

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

/* Classic Arduino setup function. The WDT is started at the beginning so that if an
 * initialization step hangs the code will try again after the WDT resets the device.
 */
void setup()
{ 
  wdt_enable(WDTO_8S);
  
  SERIAL_T(Serial.begin(9600));
  delay(500);
  
  SERIAL_T(Serial.print("S...."));

  wdt_reset();

  /* Initialize the IO pins. */
  /* TODO: A0 is only used as a pulldown for the prototype. */
  
  pinMode(A0,OUTPUT);
  pinMode(BATTERY_PIN,INPUT);
  pinMode(ETHERNET_SHIELD_SD_SEL,OUTPUT);
  digitalWrite(A0,HIGH);
  digitalWrite(ETHERNET_SHIELD_SD_SEL,HIGH);

  wdt_reset();

  /* Setup the tasks. The weather station will read all sensors once per second. This gives an update 
   * rate of 1hz for the discrete sensors and about 1hz for the digital sensors. The digital sensors
   * are read as part of a state machine that takes 6 calls to cycle once. So 166ms * 6 is about 1 
   * second. This makes the discrete and digital sensors have a similar data update rate.
   */

  schedule.addTask(readDiscreteSensors,   1000); //, "R_SENS\0"); /*  1.0 seconds */
  schedule.addTask(readI2CSensors,         166); //, "D_SENS\0"); /*  166 milliseconds */ 
  schedule.addTask(logData,             120000); //, "SK_LOG\0"); /*  120.00 seconds */
  schedule.addTask(outputData,           15000); //, "TRM_LOG\0");
  schedule.addTask(logSystem,           600000);                   /* 600.0 seconds */
  TEST(schedule.addTask(memCheck,        60000)); //, "DBG_LOG\0")); /*  60.00 seconds */

  wdt_reset();

  /* Initialize the sensors. */
  weatherVane.begin();
  bmp180.begin();
  htu21d.begin();

  wdt_reset();

  /* Initialize the logging framework. */
  sparkfun_logger.begin();


  /* Attach the interrupts this is for the weatherVane. */
  /* TODO: move this to the WeatherVane.begin code... maybe */
  attachInterrupt(digitalPinToInterrupt(2), localWindSpeed_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(3), localRainFall_ISR, RISING);

  wdt_reset();
  
  SERIAL_T(Serial.println("F"));

}

/* And the classic Arduino loop function. The scheduler takes care of
 * running all of the tasks when needed so all that needs to be done
 * here is call the scheduler.
 */
void loop()
{
  schedule.run();
  wdt_reset();
}

/* Circular values are special the next two functions wrap the circle stuff up.
 * Use the Mitsuta Method (ish) for circular averages. This still isn't perfect but
 * will suite the needs of the weather station.
 */

void updateValueCircle(long working_value)
{
  long delta = collectedData[station_wind_direction].getMean() - working_value;

  if (delta < -1800)
  {
    working_value -= 3600;
  }
  else if (delta > 1800)
  {
    working_value += 3600;
  }

  collectedData[station_wind_direction].includeValue(working_value);

}

long getValueCircle()
{
  long working_value;
  
  working_value = collectedData[station_wind_direction].getMean();
  
  if (working_value >= 3600)
  {
    working_value -= 3600;  
  }
  else if(working_value < 0)
  {
    working_value += 3600;  
  }
  
  return working_value;
}

/* This function reads the sensors that are attached directly to the Arduino.
 * The wind speed, wind direction, and rain fall sensors fall into this function.
 * NOTE: Rain processing is done in the logging task.
 * 
 * In the future one of the light sensors will be here as well. 
 * TODO: Add UV light sensor.
 */

void readDiscreteSensors()
{
  long currentWindspeed;
  long currentWindDirection;

  currentWindDirection = weatherVane.getWindDirection();
  updateValueCircle(currentWindDirection);

  currentWindspeed = weatherVane.getWindSpeed();
  collectedData[station_wind_speed].includeValue(currentWindspeed);

  if (currentWindspeed == collectedData[station_wind_speed].getMax())
  {
    max_windSpeedDir = currentWindDirection;
  }

  batteryVoltage.includeValue(((long)analogRead(BATTERY_PIN) * 500l) / 1023l);

  taskRunTime[0].includeValue(schedule.getTaskExecutionTime(0));
}

/* Read the digital I2C sensors here. This function concludes the sensor
 * tasks. The Humidity, and Pressure sensors are read here. In the future
 * IR and White light will be added to this function.
 * TODO: add support for IR/White light sensor.
 */
void readI2CSensors( void )
{
  /* Run the pressure sensor, if the pressure sensor just read pressure then use it
   * in the filtered pressure reading.
   */

  char state = bmp180.run();

  if (state == BMP180_Read_Pressure)
  {
    collectedData[station_air_pressure].includeValue(bmp180.getPressure());
  }
  else if (state == BMP180_Read_Temperature)
  {
    collectedData[station_air_temperature].includeValue(bmp180.getTemperature());
  }

  /* Run the humidity sensor, if the humidity sensor just read humidity or temperature
   * use the new value in the appropriate filter.
   */
  
  state = htu21d.run();

  if (state == HTU21D_RetrieveHumidity)
  {
    collectedData[station_air_humidity].includeValue(htu21d.getHumidity());
  }
  else if (state == HTU21D_RetrieveTemperature)
  {
    collectedData[station_air_temperature].includeValue(htu21d.getTemperature());
  }

  taskRunTime[1].includeValue(schedule.getTaskExecutionTime(1));
}

/* Function that will transmit the text contained in logString. */

void sendLoggingString()
{
  char looper;
  char ethernetStatus;
  char ethernetClosed;
  
  ethernetClosed = sparkfun_logger.close();

  for (looper = 0; looper < 3; looper++)
  {
    wdt_reset();
    ethernetStatus = sparkfun_logger.sendGetRequest(logString);
    if (ethernetStatus == ETHERNET_CONNECTION_SUCCESS)
    {
      break;
    }
  }

  SERIAL_T(Serial.print("ES: "));
  SERIAL_T(Serial.print(ethernetStatus,HEX));
  SERIAL_T(Serial.print(" "));
  SERIAL_T(Serial.print(ethernetClosed,HEX));
  SERIAL_T(Serial.print(" "));
  SERIAL_T(Serial.println(looper,HEX));
}

/* Function to populate logString with the system data. */

void buildSystemString()
{
  systemMessageCount++;

  /* The entries in dataArray must match the order of the entries in 
   * systemStrings defined in sparkfun_log.h
   */  
  
  long dataArray[] = {taskRunTime[0].getMean(),
                      taskRunTime[0].getMax(),
                      taskRunTime[1].getMean(),
                      taskRunTime[1].getMax(),
                      taskRunTime[2].getMean(),
                      taskRunTime[2].getMax(),
                      taskRunTime[3].getMean(),
                      taskRunTime[3].getMax(),
                      batteryVoltage.getMean(),
                      millis()/1000,
                      systemMessageCount};

  createLoggingString(systemPublicKey, systemPrivateKey, 
                      dataArray, systemStrings, 11,
                      logString);
                
}

/* Function to populate logString with the weather data. */

void buildWeatherString()
{
  unsigned int rainFall = weatherVane.getRainFall();
  weatherVane.resetRainFall();

  weatherMessageCount++;
                                  
  unsigned long currentTime = millis();
  
  /* The entries in dataArray must match the order of the entries in 
   * weatherStrings defined in sparkfun_log.h
   */
  
  long dataArray[] = {currentTime - weatherLogTime,
                      collectedData[station_air_humidity].getMean(),
                      collectedData[station_air_pressure].getMean()*PASCAL_TO_INHG,
                      C_TO_F(collectedData[station_air_temperature].getMean()),
                      collectedData[station_ir_light].getMean(),
                      collectedData[station_uv_light].getMean(),
                      collectedData[station_white_light].getMean(),
                      getValueCircle(),
                      collectedData[station_wind_speed].getMean(),
                      collectedData[station_wind_speed].getStdev()/100l,
                      collectedData[station_wind_speed].getMax(),
                      max_windSpeedDir,
                      rainFall,
                      weatherMessageCount};

  weatherLogTime = currentTime;

  createLoggingString(weatherPublicKey, weatherPrivateKey, 
                      dataArray, weatherStrings, 14,
                      logString);
}

/* Perform the steps to provide the remote server with weather data. */

void logData()
{
  char looper;

  buildWeatherString();

  sendLoggingString();

  /* Reset all of the statistics classes to get ready for the next 2 minutes. */

  for (looper = 0; looper < station_collected_data_size; looper++)
  {
    collectedData[looper].reset();
  }
  
  taskRunTime[2].includeValue(schedule.getTaskExecutionTime(2));

}

void outputData ( void )
{
  SERIAL_T(Serial.println());
  SERIAL_T(Serial.print("P  : "));
  SERIAL_T(Serial.println((unsigned int)(collectedData[station_air_pressure].getMean()*PASCAL_TO_INHG)));
  SERIAL_T(Serial.print("Pd : "));
  SERIAL_T(Serial.println((unsigned int)(collectedData[station_air_pressure].getStdev()*PASCAL_TO_INHG)));
  SERIAL_T(Serial.print("H  : "));
  SERIAL_T(Serial.println(collectedData[station_air_humidity].getMean()));
  SERIAL_T(Serial.print("Hd : "));
  SERIAL_T(Serial.println(collectedData[station_air_humidity].getStdev()));
  SERIAL_T(Serial.print("T  : "));
  SERIAL_T(Serial.println(C_TO_F(collectedData[station_air_temperature].getMean())));
  SERIAL_T(Serial.print("Td : "));
  SERIAL_T(Serial.println(C_TO_F(collectedData[station_air_temperature].getStdev())));
  SERIAL_T(Serial.print("WSa: "));
  SERIAL_T(Serial.println(collectedData[station_wind_speed].getMean()));
  SERIAL_T(Serial.print("WSd: "));
  SERIAL_T(Serial.println(collectedData[station_wind_speed].getStdev()));
  SERIAL_T(Serial.print("WSM: "));
  SERIAL_T(Serial.println(collectedData[station_wind_speed].getMax()));
  SERIAL_T(Serial.print("WSm: "));
  SERIAL_T(Serial.println(collectedData[station_wind_speed].getMin()));
  
  SERIAL_T(Serial.print("WD : "));
  SERIAL_T(Serial.println(getValueCircle()));
  SERIAL_T(Serial.print("MWD: "));
  SERIAL_T(Serial.println(max_windSpeedDir));
}

/* Perform the steps to provide the remote server with system data. */

void logSystem()
{
  buildSystemString();

  sendLoggingString();
  
  batteryVoltage.reset();
  
  taskRunTime[3].includeValue(schedule.getTaskExecutionTime(4));
}

TEST(
void memCheck( void )
{
  int i;
  char * value = new char;
  Serial.println((int)value,HEX);
  delete value;

  for (i = 0; i < schedule.getTaskCount(); i++)
  {
    Serial.println(schedule.getTaskExecutionTime(i));
  }

  Serial.print("SCH_TME\t");
  Serial.println(schedule.getIntervalTime());
}
)
