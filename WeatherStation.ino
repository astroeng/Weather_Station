#include <software_i2c.h>
#include <software_spi.h>
#include <MPL115A1.h>
#include <bmp180.h>
#include <htu21d.h>
#include <primitive_scheduler.h>

#define I2C_CLOCK A4
#define I2C_DATA  A5

#define I2C_CLOCK_2 11
#define I2C_DATA_2  12

#define SPI_CLOCK    5
#define SPI_DATA_OUT 6
#define SPI_DATA_IN  7

#define MPL115A1_SELECT 8
#define MPL115A1_SLEEP  9

PrimitiveScheduler schedule(4);

Software_I2C i2c_bus(I2C_DATA, 
                     I2C_CLOCK, 
                     100);

Software_I2C i2c_bus_2(I2C_DATA_2, 
                       I2C_CLOCK_2, 
                       50);

Software_SPI spi_bus(SPI_DATA_IN,
                     SPI_DATA_OUT,
                     SPI_CLOCK,
                     MSB_FIRST,
                     500);
                     
BMP180 bmp180(&i2c_bus);           /* temperature and pressure sensor */
HTU21D htu21d(&i2c_bus_2);         /* temperature and humidity sensor */
MPL115A1 mpl115a1(&spi_bus,        /* temperature and pressure sensor */
                  MPL115A1_SELECT, 
                  MPL115A1_SLEEP);



void setup()
{
  Serial.begin(9600);
  delay(100);
  schedule.addTask(readSensors, 1000);
  schedule.addTask(outputData, 10000);
  
  bmp180.getCalibrationData();
  mpl115a1.begin();
}


void loop()
{
  schedule.run();
}

void readSensors( void )
{
  bmp180.getData();
  
  htu21d.run();
  mpl115a1.run();

}


void outputData ( void )
{
  long temperature[4];
  long pressure[2];
  long humidity[2];
  long uvLight = 0;
  long whiteLight = 0;
  long irLight = 0;
  
  temperature[0] = bmp180.getTemperature();
  temperature[1] = htu21d.getTemperature();
  temperature[2] = mpl115a1.getTemperature();
  temperature[3] = 0;
  
  pressure[0] = bmp180.getPressure();
  pressure[1] = mpl115a1.getPressure();
  
  humidity[0] = htu21d.getHumidity();
  humidity[1] = 0;
  
  Serial.print(" Bt,");
  Serial.print(temperature[0]);
  Serial.print(",Ht,");
  Serial.print(temperature[1]);
  Serial.print(",Mt,");
  Serial.print(temperature[2]);
  Serial.print(",At,");
  Serial.print((temperature[0] + temperature[1]) / 2);
  
  Serial.print(".Bp,");
  Serial.print(pressure[0]);
  Serial.print(",Mp,");
  Serial.print(pressure[1]);
  Serial.print(",Ap,");
  Serial.print((pressure[0] + pressure[1]) / 2);
  
  Serial.print(",Hh,");
  Serial.print(humidity[0]);
  
  Serial.print(",TET,");
  Serial.print(schedule.getTaskExecutionTime(0));
  Serial.print(",");
  Serial.println(schedule.getTaskExecutionTime(1));
}
