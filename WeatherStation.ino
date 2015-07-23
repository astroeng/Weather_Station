#include <software_i2c.h>
#include <software_spi.h>
#include <MPL115A1.h>
#include <bmp180.h>
#include <htu21d.h>
#include <primitive_scheduler.h>

#include "sparkfun_log.h"

#define BMP180_CLOCK A4
#define BMP180_DATA  A5

#define HTU21D_CLOCK 11
#define HTU21D_DATA  12

#define SPI_CLOCK    5
#define SPI_DATA_OUT 6
#define SPI_DATA_IN  7

#define MPL115A1_SELECT 8
#define MPL115A1_SLEEP  9


#define PASCAL_TO_PSI 0.145037738
#define PASCAL_TO_INHG 0.295333727
#define C_TO_F(x) ((9*x)/5)+3200

PrimitiveScheduler schedule(4);

Software_I2C bmp180_bus(BMP180_DATA, 
                        BMP180_CLOCK, 
                        100);

Software_I2C htu21d_bus(HTU21D_DATA, 
                        HTU21D_CLOCK, 
                        50);

Software_SPI spi_bus(SPI_DATA_IN,
                     SPI_DATA_OUT,
                     SPI_CLOCK,
                     MSB_FIRST,
                     500);
                     
BMP180 bmp180(&bmp180_bus);        /* temperature and pressure sensor */
HTU21D htu21d(&htu21d_bus);        /* temperature and humidity sensor */
MPL115A1 mpl115a1(&spi_bus,        /* temperature and pressure sensor */
                  MPL115A1_SELECT,
                  MPL115A1_SLEEP);

void setup()
{
  Serial.begin(9600);
  delay(200);

  schedule.addTask(readSensors, 1000); /* 1 second */
  schedule.addTask(outputData, 10000); /* 10 seconds */
  schedule.addTask(logData,   300000); /* 5 minutes */ 
  
  bmp180.begin();
  htu21d.begin();
  mpl115a1.begin();
}


void loop()
{
  schedule.run();
}

void readSensors( void )
{
  
  bmp180.run();
  htu21d.run();
  mpl115a1.run();

}

void logData()
{
  
  Serial.println(createLoggingString(bmp180.getPressure()*PASCAL_TO_INHG,
                                     htu21d.getHumidity(),
                                     C_TO_F(htu21d.getTemperature()),
                                     0,
                                     0,
                                     0,
                                     0,
                                     0));

}

void outputData ( void )
{
  long temperature[4];
  long pressure[2];
  long humidity[2];
  long uvLight = 0;
  long whiteLight = 0;
  long irLight = 0;
  
  long averagePressure;
  
  temperature[0] = bmp180.getTemperature();
  temperature[1] = htu21d.getTemperature();   /* This seems to have the best resolution. */
  temperature[2] = mpl115a1.getTemperature(); /* This is not even calibrated......!!! */
  temperature[3] = 0;
  
  pressure[0] = bmp180.getPressure();     /* This seems to have the best resolution. */
  pressure[1] = mpl115a1.getPressure();
  
  humidity[0] = htu21d.getHumidity();
  humidity[1] = 0;
  
  averagePressure = (pressure[0] + pressure[1]) / 2;
  
  Serial.print(" Bt,");
  Serial.print(C_TO_F(temperature[0]));
  Serial.print(",Ht,");
  Serial.print(C_TO_F(temperature[1]));
  Serial.print(",Mt,");
  Serial.print(C_TO_F(temperature[2]*10));
  Serial.print(",At,");
  Serial.print(C_TO_F((temperature[0] + temperature[1]) / 2));
  
  Serial.print(",Bp,");
  Serial.print(pressure[0] * PASCAL_TO_INHG);
  Serial.print(",Mp,");
  Serial.print(pressure[1] * PASCAL_TO_INHG);
  Serial.print(",Ap,");
  Serial.print(averagePressure * PASCAL_TO_INHG);
  
  Serial.print(",Hh,");
  Serial.print(humidity[0]);
  
  Serial.print(",TET,");
  Serial.print(schedule.getTaskExecutionTime(0));
  Serial.print(",");
  Serial.println(schedule.getTaskExecutionTime(1));
}
