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
 * a whole lot of reserve space. This code with the serial stuff not included is about 73% of the program
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

#include <avr/wdt.h>

/* These apparently two HAVE to be included HERE for anything to work.... */
#include <SPI.h>
#include <Ethernet.h>

#include <primitive_scheduler.h>

#include "discrete_task.h"
#include "digital_task.h"
#include "weather_log_task.h"
#include "system_log_task.h"
#include "global_data.h"

#define TEST(x)

/* http://data.sparkfun.com/highlands_weather */


/* Schedule definition. */
const TaskDataType PROGMEM taskData[] = {{readDiscreteSensors, DISCRETE_INTERVAL},
                                         {readDigitalSensors,  DIGITAL_INTERVAL},
                                         {logData,             WEATHER_LOG_INTERVAL},
                                         {logSystem,           SYSTEM_LOG_INTERVAL}};



/* Classic Arduino setup function. The WDT is started at the beginning so that if an
 * initialization step hangs the code will try again after the WDT resets the device.
 */

void setup()
{
  int looper;
  wdt_enable(WDTO_8S);

  TEST(Serial.begin(SERIAL_SPEED));
  delay(500);
  TEST(Serial.print("S...."));

  wdt_reset();

  /* Initialize the IO pins. */
  pinMode(ETHERNET_SHIELD_SD_SEL,OUTPUT);
  digitalWrite(ETHERNET_SHIELD_SD_SEL,HIGH);

  wdt_reset();

  /* Initialize the sensors. */
  setupDigitalSensors();
  setupDiscreteSensors();

  wdt_reset();

  /* Initialize the logging framework. */
  sparkfun_logger.begin();

  wdt_reset();

  /* Setup the tasks. The weather station will read all sensors once per second. This gives an update
   * rate of 1hz for the discrete sensors and about 1hz for the digital sensors. The digital sensors
   * are read as part of a state machine that takes 6 calls to cycle once. So 166ms * 6 is about 1
   * second. This makes the discrete and digital sensors have a similar data update rate.
   */

  for (looper = 0; looper < task_name_length; looper++)
  {
    func_ptr entryPoint;
    unsigned long taskInterval;

    memcpy_P((void *)&entryPoint,   (void *)&taskData[looper].entryPoint,   sizeof(func_ptr));
    memcpy_P((void *)&taskInterval, (void *)&taskData[looper].taskInterval, sizeof(taskInterval));

    schedule.addTask(entryPoint, taskInterval);
  }

  TEST(schedule.addTask(outputData, 15000));
  TEST(schedule.addTask(memCheck,   15000)); /*  60.0 seconds */

  wdt_reset();

  TEST(Serial.println("F"));

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

/* Test next functions are only included in TEST mode. */

TEST(
void outputData ( void )
{
  DigitalDataType* digital = getDigitalData();
  DiscreteDataType* discrete = getDiscreteData();

  Serial.println();
  Serial.print("P  : ");
  Serial.println((unsigned int)(digital->collectedData[station_air_pressure].getMean()));
  Serial.print("Pd : ");
  Serial.println((unsigned int)(digital->collectedData[station_air_pressure].getStdev()));
  Serial.print("H  : ");
  Serial.println(digital->collectedData[station_air_humidity].getMean());
  Serial.print("Hd : ");
  Serial.println(digital->collectedData[station_air_humidity].getStdev());
  Serial.print("T  : ");
  Serial.println(digital->collectedData[station_air_temperature].getMean());
  Serial.print("Td : ");
  Serial.println(digital->collectedData[station_air_temperature].getStdev());
  Serial.print("WSa: ");
  Serial.println(discrete->collectedData[station_wind_speed].getMean());
  Serial.print("WSd: ");
  Serial.println(discrete->collectedData[station_wind_speed].getStdev());
  Serial.print("WSM: ");
  Serial.println(discrete->collectedData[station_wind_speed].getMax());
  Serial.print("WSm: ");
  Serial.println(discrete->collectedData[station_wind_speed].getMin());

  Serial.print("WD : ");
  Serial.println(getWindDirection());
  Serial.print("MWD: ");
  Serial.println(discrete->windDirectionForMaxSpeed);
}
)

TEST(
void memCheck( void )
{
  int i;
  char * value = new char;
  Serial.println((int)value,HEX);
  delete value;

  for (i = 0; i < schedule.getTaskCount(); i++)
  {
    Serial.print(i);
    Serial.print(" : ");
    Serial.print(schedule.getTaskExecutionTime(i));
    Serial.print(" : ");
    Serial.println(schedule.getTaskSkipped(i));

  }

  Serial.print("SCH_TME\t");
  Serial.println(schedule.getIntervalTime());
}
)
