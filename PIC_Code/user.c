/* Derek Schacht 
 *  2016 01 25 
 *  Timer Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */


#include "xc.h"
#include "picduino.h"
#include "mpl115a1.h"
#include "software_i2c.h"
#include "string_utilities.h"

#define TOGGLE_PIN 10

#define UART_TX remappable_pin_io4
#define UART_RX remappable_pin_io12 /* 12 WORKS */
#define UART_GND RD12

#define MPL115A1_CLOCK remappable_pin_io5
#define MPL115A1_SDI   remappable_pin_io10
#define MPL115A1_SDO   remappable_pin_io17
#define MPL115A1_CS    6 //remappable_pin_io16

#define MPL115A1_GND    2
#define MPL115A1_PWR    1
#define MPL115A1_ACTIVE 7

#define PASCAL_TO_INHG 0.295333727
#define C_TO_F(x) ((9*x)/5)+320

SpiConfigType mpl115a1;
MPL115A1CalibrationDataType pressureSensorCal;
MPL115A1DataType mpl115a1Data;
UartConfigType uart1;
I2C_State_Object_Type i2cBus;

void setup(void) 
{
    int i;

    TRISDbits.TRISD12 = 0;
    LATDbits.LATD12 = 0;
    
    pinMode(TOGGLE_PIN, OUTPUT);
    digitalWrite(TOGGLE_PIN, 0);
    
    pinMode(MPL115A1_GND, OUTPUT);
    digitalWrite(MPL115A1_GND, 0);
    
    pinMode(MPL115A1_PWR, OUTPUT);
    digitalWrite(MPL115A1_PWR, 1);
    
    pinMode(MPL115A1_ACTIVE, OUTPUT);
    digitalWrite(MPL115A1_ACTIVE, 1);
    
    uart_init(&uart1,
              UART_Device1,
              1000000,
              remappable_pin_nc,
              remappable_pin_nc,
              UART_RX,
              UART_TX);
    
    uart_begin(&uart1);
    
    spi_init(&mpl115a1,
             MPL115A1_SDO,
             MPL115A1_SDI,
             MPL115A1_CLOCK,
             MPL115A1_CS,
             SPI_Device1,
             1,
             2);
    
    spi_begin(&mpl115a1);
    
    init_tsl2561_sensor(&i2cBus);
    
    /* Give the SPI device a swift kick in the pants. If this is not done the
     * calibration sequence must be done a second time to actually get data.
     */
    spi_selectSlave(&mpl115a1);
    for (i = 0; i < 32; i++)
    {
        spi_transfer(&mpl115a1, 0x00);
    }
    
    while(spi_busy(&mpl115a1) == 1) {}
    
    /* read the data and throw it away. */
    spi_getData(&mpl115a1, 0, 0);
    
    /* Now get the calibration data */
    mpl115a1_getCalibration(&mpl115a1, &pressureSensorCal);
    
}

unsigned char string[128];

unsigned long lastSample = 0;
unsigned long time;
unsigned char value;

unsigned long start;

unsigned int broadLight;
unsigned int irLight;

unsigned char title[] = " ptime   ltime    rate   press   temp   wlight   ilight  stime\r\n\0";

void loop()
{
    lastSample = time;

    do
    {
        time = micros();
    } while ((time - lastSample) < 999998);
    uart_sendData(&uart1, &formFeed, 1);
    uart_sendData(&uart1, &title, strlen(title));
    /* This delay call synchronizes the program execution to the system tick.
     * For testing this makes the printed timestamps a bit more stable. 
     */
    delay(1);
    
    start = micros();
    mpl115a1_run(&mpl115a1, &pressureSensorCal, &mpl115a1Data);
    uart_sendData(&uart1, toString(micros()-start), 9);
    
    start = micros();
    read_tsl2561_data(&i2cBus, &broadLight, &irLight);
    uart_sendData(&uart1, toString(micros()-start), 9);
    
    /* Send over the sensor data. */
    start = micros();
    uart_sendData(&uart1, toString(time - lastSample), 9);
    uart_sendData(&uart1, toString(mpl115a1Data.pressure_pa*PASCAL_TO_INHG), 9);
    uart_sendData(&uart1, toString(C_TO_F(mpl115a1Data.temperature_c)), 9);
    uart_sendData(&uart1, toString(broadLight), 9);
    uart_sendData(&uart1, toString(irLight), 9);
    uart_sendData(&uart1, toString(micros()-start), 9);
    

    value = uart_dataAvailable(&uart1);
    uart_getData(&uart1, string, value);
    uart_sendData(&uart1, string, value);
 
}