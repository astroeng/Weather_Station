#ifndef SOFTWARE_I2C
#define SOFTWARE_I2C

/* On the PIC24 only certain pins can drive an i2c bus. These pins are mapped
 * to the SDA and SCL pins on the Arduino headers. For any other pair of pins
 * to be used for an i2c bus, the bus behavior must be implemented in software.
 * This software i2c bus is only capable of being the bus master.
 */

#define I2C_ACK  0
#define I2C_NACK 1

#define I2C_HIGH  1
#define I2C_LOW   0

#define I2C_WRITE 0x00
#define I2C_READ  0x01

#define I2C_AVAILABLE               10
#define I2C_BUSY                    11

/* Errors */
#define I2C_NO_ERROR                 0
#define I2C_START_CLOCK_PIN_ERROR   -1
#define I2C_START_DATA_PIN_ERROR    -2
#define I2C_WRITE_CLOCK_PIN_TIMEOUT -3
#define I2C_WRITE_ARBITRATION_LOST  -4
#define I2C_READ_CLOCK_TIMEOUT      -5
#define I2C_STOP_CLOCK_ERROR        -6
#define I2C_STOP_DATA_ERROR         -7

typedef struct
{
    int dataPin;
    int clockPin;
    int delay;
    unsigned char address;
} I2C_State_Object_Type;

/* These are the three functions that should be used by consumers of this utility. */

void i2c_init(I2C_State_Object_Type* i2c_object, 
              unsigned char data_pin, 
              unsigned char clock_pin,
              unsigned int delay);

char i2c_write(I2C_State_Object_Type* i2c_object, unsigned char address, unsigned char command, unsigned char* data, unsigned int elements);
char i2c_read(I2C_State_Object_Type* i2c_object, unsigned char address, unsigned char command, unsigned char* data, unsigned int elements);

/* The rest of the functions are exposed to allow debugging. */
int i2c_status(I2C_State_Object_Type* i2c_object);
void i2c_release(I2C_State_Object_Type* i2c_object);

int i2c_start(I2C_State_Object_Type* i2c_object);
int i2c_stop(I2C_State_Object_Type* i2c_object);

int i2c_write_byte(I2C_State_Object_Type* i2c_object, unsigned char output);
unsigned char i2c_read_byte(I2C_State_Object_Type* i2c_object, char* error, unsigned char ack);

#endif