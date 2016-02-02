#include "software_i2c.h"
#include "io_support.h"
#include "timer_support.h"

#define TEST(x)

#define DRIVE_PIN_LOW(x) pinMode(x,OUTPUT)
#define FLOAT_PIN_HIGH(x) pinMode(x,INPUT)
#define READ_PIN(x) digitalRead(x)

/* Magic define statements to make this less painful. Classes would be so nice. 
 */

#define CLOCK_STRETCH_TIMEOUT i2c_object->delay * 10
#define CLOCK_STRETCH_DELAY() delayMicroseconds(1)

#define CLOCK_DELAY() delayMicroseconds(i2c_object->delay)

//#define _address           i2c_object->address
#define _data_pin          i2c_object->dataPin
#define _clock_pin         i2c_object->clockPin
#define _release_control() i2c_release(i2c_object)
#define _read_bit(x)       read_bit(i2c_object,x)
#define _write_bit(x)      write_bit(i2c_object,x)
#define _i2c_read(x,a)     i2c_read_byte(i2c_object,x,a)
#define _i2c_write(x)      i2c_write_byte(i2c_object,x)
#define _i2c_start()       i2c_start(i2c_object)
#define _i2c_stop()        i2c_stop(i2c_object)
#define _i2c_status()      i2c_status(i2c_object)

/* Hopefully this won't make too many eyes bleed. */

/* Output a single bit of information.
 */

int write_bit(I2C_State_Object_Type* i2c_object, unsigned char value)
{
  int stretch_timer = 0;
  
  if (value > 0)
  {
    FLOAT_PIN_HIGH(_data_pin);
    
    TEST(Serial.print("wb1"));
  }
  else 
  {
    DRIVE_PIN_LOW(_data_pin);
    
    TEST(Serial.print("wb0"));
  }

  CLOCK_DELAY();
  
  /* Give up control of the clock pin, this allows for the clock stretch
     check. It will also cause the rising edge of the clock signal.
   */
  
  FLOAT_PIN_HIGH(_clock_pin);
  
  while (READ_PIN(_clock_pin) == I2C_LOW)
  {
    CLOCK_STRETCH_DELAY();
    stretch_timer++;
    
    TEST(Serial.print("S"));
    
    if (stretch_timer > CLOCK_STRETCH_TIMEOUT)
    {
      _release_control();
      TEST(Serial.println("Stretch1"));
      return I2C_WRITE_CLOCK_PIN_TIMEOUT;
    }
  }
  
  /* Check to make sure bit and the data line actually match. This 
     ensures that the bus is being driven by this device and not
     someone else. This check can only be performed if this device
     was trying to output a HIGH (1).
   */
  
  if ((value == 1) && (READ_PIN(_data_pin) == 0))
  {
    _release_control();
    TEST(Serial.println("Arbitration_Lost"));
    return I2C_WRITE_ARBITRATION_LOST;
  }
  
  CLOCK_DELAY();
  
  DRIVE_PIN_LOW(_clock_pin);
  
  return I2C_NO_ERROR;
  
}

/* Read a bit of information from a slave device. The slave device will
   output the new data on the rising edge of the clock pulse. The slave
   device will hold the clock signal low until it is ready to output the
   data.
 */

unsigned char read_bit(I2C_State_Object_Type* i2c_object, char* error)
{
  char value = 0;
  int stretch_timer = 0;
  *error = I2C_NO_ERROR;
  
  /* Give up control of the data pin. */

  FLOAT_PIN_HIGH(_data_pin);

  CLOCK_DELAY();
  
  /* Give up control of the clock pin, this allows for the clock stretch
     check. It also causes the rising edge of the clock signal.
   */

  FLOAT_PIN_HIGH(_clock_pin);

  while (READ_PIN(_clock_pin) == I2C_LOW)
  {
    CLOCK_STRETCH_DELAY();
    stretch_timer++;
    
    if (stretch_timer > CLOCK_STRETCH_TIMEOUT)
    {
      *error = I2C_READ_CLOCK_TIMEOUT;
      _release_control();
      return 0;
    }
  }

  /* Since the clock is high the slave must have released it and the data
     is now valid. Read it and move on.
   */

  value = READ_PIN(_data_pin);
  
  if (value > 0) 
  {
    TEST(Serial.print("rb1"));
  }
  else 
  {
    TEST(Serial.print("rb0"));
  }

  CLOCK_DELAY();
  
  /* Reassume control of the clock pin and return the result from the remote
     device.
   */

  DRIVE_PIN_LOW(_clock_pin);
  
  return value;

}

int i2c_status(I2C_State_Object_Type* i2c_object)
{
  int stretch_timer = 0;
  
  _release_control();
  
  while (READ_PIN(_clock_pin) == I2C_LOW)
  {
    CLOCK_STRETCH_DELAY();
    stretch_timer++;
    
    if (stretch_timer > CLOCK_STRETCH_TIMEOUT)
    {
      return I2C_BUSY;
    }
  }
  
  stretch_timer = 0;
  
  while (READ_PIN(_data_pin) == I2C_LOW)
  {
    CLOCK_STRETCH_DELAY();
    stretch_timer++;
    
    if (stretch_timer > CLOCK_STRETCH_TIMEOUT)
    {
      return I2C_BUSY;
    }
  }
  
  return I2C_AVAILABLE;
}

/* Read a byte of information from a slave device and reply with the 
   appropriate acknowledgement.
 */

unsigned char i2c_read_byte(I2C_State_Object_Type* i2c_object, char* error, unsigned char ack)
{
  unsigned char input = 0;
  char input_bit = 0;
  int bit_looper;
  
  /* Loop through sending the bits of the byte. */

  TEST(Serial.print("r"));
  
  for (bit_looper = 0; bit_looper < 8; bit_looper++)
  {

    input_bit = _read_bit(error);
    
    if (*error != I2C_NO_ERROR)
    {
      _release_control();
      return 0;
    }

    input = input << 1;
    input = input | (input_bit & 0x1);

  }
  
  /* Send the Acknowledge */
  
  *error = _write_bit(ack);
  
  TEST(Serial.println());
  
  return input;
  
}

/* Output a byte of information.
 */

int i2c_write_byte(I2C_State_Object_Type* i2c_object, unsigned char output)
{
  char error;
  int bit_looper;
  char write_status;
  
  for (bit_looper = 0; bit_looper < 8; bit_looper++)
  {
    write_status = _write_bit(output & 0x80);
    if (write_status == 0)
    {
      output = output << 1;
    }
    else
    {
      _release_control();
      return write_status;
    }
  }
  
  FLOAT_PIN_HIGH(_data_pin);

  return _read_bit(&error);
}


/* Allow the clock pin to be high and if it is take the data pin low.
   This will signal to the slave devices that something is about to 
   start.
 */

int i2c_start(I2C_State_Object_Type* i2c_object)
{
  int stretch_timer = 0;

  _release_control();
  
  while (READ_PIN(_clock_pin) == I2C_LOW)
  {
    CLOCK_STRETCH_DELAY();
    stretch_timer++;
    
    if (stretch_timer > CLOCK_STRETCH_TIMEOUT)
    {
      return I2C_START_CLOCK_PIN_ERROR;
    }
  }
  
  stretch_timer = 0;
  
  while (READ_PIN(_data_pin) == I2C_LOW)
  {
    CLOCK_STRETCH_DELAY();
    stretch_timer++;
    
    if (stretch_timer > CLOCK_STRETCH_TIMEOUT)
    {
      return I2C_START_DATA_PIN_ERROR;
    }
  }
  
  DRIVE_PIN_LOW(_data_pin);
  CLOCK_DELAY();
  DRIVE_PIN_LOW(_clock_pin);
  
  return I2C_NO_ERROR;
}

/* Release both the data pin and the clock pin. This allows another device
   to then control the bus.
 */

int i2c_stop(I2C_State_Object_Type* i2c_object)
{
  int stretch_timer = 0;
  
  FLOAT_PIN_HIGH(_clock_pin);
  
  while (READ_PIN(_clock_pin) == I2C_LOW)
  {
    CLOCK_STRETCH_DELAY();
    stretch_timer++;
    
    if (stretch_timer > CLOCK_STRETCH_TIMEOUT)
    {
      return I2C_STOP_CLOCK_ERROR;
    }
  }
  
  CLOCK_DELAY();
  FLOAT_PIN_HIGH(_data_pin);
  
  stretch_timer = 0;
  
  while (READ_PIN(_data_pin) == I2C_LOW)
  {
    CLOCK_STRETCH_DELAY();
    stretch_timer++;
    
    if (stretch_timer > CLOCK_STRETCH_TIMEOUT)
    {
      return I2C_STOP_DATA_ERROR;
    }
  }
  
  return I2C_NO_ERROR;
}


void i2c_release(I2C_State_Object_Type* i2c_object)
{
  FLOAT_PIN_HIGH(_clock_pin);
  FLOAT_PIN_HIGH(_data_pin);
}

void i2c_init(I2C_State_Object_Type* i2c_object, 
              unsigned char data_pin, 
              unsigned char clock_pin,
              unsigned int delay)
{
  /* Initialize the object with the parameters passed. */

  i2c_object->clockPin = clock_pin;
  i2c_object->dataPin = data_pin;
  i2c_object->delay = delay;
  
  _release_control();
  
  /* Set the outputs to low so that when the mode is changed to output
     the value on the wire will be low.
   */
  digitalWrite(_data_pin, I2C_LOW);
  digitalWrite(_clock_pin, I2C_LOW);
}

char i2c_write(I2C_State_Object_Type* i2c_object, unsigned char address, unsigned char command, unsigned char* data, unsigned int elements)
{
  int i = 0;
  
  address = address << 1;
  
  _i2c_start();
  _i2c_write(address | I2C_WRITE);
  _i2c_write(command);
  
  for (i = 0; i < elements; i++)
  {
	  _i2c_write(data[i]);
  }

  _i2c_stop();

  return _i2c_status();
}

char i2c_read(I2C_State_Object_Type* i2c_object, unsigned char address, unsigned char command, unsigned char* data, unsigned int elements)
{
  int i = 0;
  char error;
  
  address = address << 1;
  
  _i2c_start();
  _i2c_write(address | I2C_WRITE);
  _i2c_write(command);
  _i2c_start();
  _i2c_write(address | I2C_READ);
  
  for (i = 0; i < elements - 1; i++)
  {
	data[i] = _i2c_read(&error,I2C_ACK);
  }

  data[i] = _i2c_read(&error, I2C_NACK);

  _i2c_stop();

  return _i2c_status();
}
