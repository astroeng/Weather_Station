#include "io_support.h"
#include "software_i2c.h"
#include "timer_support.h"


void setup_tsl2561_sensor(I2C_State_Object_Type* sensorBus)
{
    int i2c_status;
    unsigned char data = 0x03;
    i2c_status = i2c_write(sensorBus, 0x39, 0x80, &data, 1);
    delay(100l);
    data = 0;
    i2c_status = i2c_write(sensorBus, 0x39, 0x86, &data, 1);
    delay(100l);
    i2c_status = i2c_write(sensorBus, 0x39, 0x81, &data, 1);
    delay(100l);
}

void read_tsl2561_data(I2C_State_Object_Type* sensorBus, unsigned int* broadLight, unsigned int* irLight)
{
    int i2c_status;
    unsigned char data[] = {0,0,0,0};
    i2c_status = i2c_read(sensorBus, 0x39, 0x8c, data, 4);
    
    *broadLight = (data[1] << 8) | data[0];
    *irLight = (data[3] << 8) | data[2];
}

#define TSL2561_PWR  12
#define TSL2561_GND  11
#define TSL2561_DATA 9
#define TSL2561_CLK  10

void init_tsl2561_sensor(I2C_State_Object_Type* i2cBus)
{
    digitalWrite(TSL2561_PWR,0);
    digitalWrite(TSL2561_GND,0);
    pinMode(TSL2561_PWR,OUTPUT);
    pinMode(TSL2561_GND,OUTPUT);
    digitalWrite(TSL2561_PWR,1);

    i2c_init(i2cBus,TSL2561_DATA,TSL2561_CLK,1);
    delay(2);
    setup_tsl2561_sensor(i2cBus);
    
}