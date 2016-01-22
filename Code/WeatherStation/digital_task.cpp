/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include <software_i2c.h>
#include <bmp180.h>
#include <htu21d.h>

#include "digital_task.h"
#include "global_data.h"
#include "config.h"

/* Setup a bus and pass it to the units that will use it. This should
 * allow for a seamless transition to a hardware bus in the future.
 * Provided the hardware bus library implements the same API.
 */
Software_I2C i2c_bus(I2C_DATA,
                     I2C_CLOCK,
                     I2C_PERIOD);

/* Setup some i2c sensors */
BMP180 bmp180(&i2c_bus); /* temperature and pressure sensor */
HTU21D htu21d(&i2c_bus); /* temperature and humidity sensor */


DigitalDataType digitalDataBuffer;

void setupDigitalSensors( void )
{
  bmp180.begin();
  htu21d.begin();
}

/* Read the digital I2C sensors here. This function concludes the sensor
 * tasks. The Humidity, and Pressure sensors are read here. In the future
 * IR and White light will be added to this function.
 * TODO: add support for IR/White light sensor.
 */
void readDigitalSensors( void )
{
  /* Run the pressure sensor, if the pressure sensor just read pressure then use it
   * in the filtered pressure reading.
   */

  char state = bmp180.run();

  if (state == BMP180_Read_Pressure)
  {
    digitalDataBuffer.collectedData[station_air_pressure].includeValue(bmp180.getPressure());
  }
  else if (state == BMP180_Read_Temperature)
  {
    digitalDataBuffer.collectedData[station_air_temperature].includeValue(bmp180.getTemperature());
  }

  /* Run the humidity sensor, if the humidity sensor just read humidity or temperature
   * use the new value in the appropriate filter.
   */

  state = htu21d.run();

  if (state == HTU21D_RetrieveHumidity)
  {
    digitalDataBuffer.collectedData[station_air_humidity].includeValue(htu21d.getHumidity());
  }
  else if (state == HTU21D_RetrieveTemperature)
  {
    digitalDataBuffer.collectedData[station_air_temperature].includeValue(htu21d.getTemperature());
  }

  taskRunTime[task_digital_read].includeValue(schedule.getTaskExecutionTime(task_digital_read));
}

DigitalDataType* getDigitalData( void )
{
  return &digitalDataBuffer;
}

void resetDigitalData()
{
  int i;
  for (i = 0; i < station_digital_data_size; i++)
  {
    digitalDataBuffer.collectedData[i].reset();
  }
}
