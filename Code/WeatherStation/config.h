/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_SPEED 9600
#define I2C_PERIOD 1000

/* One of my biggest gripes with the Arduino hardware is that the pins
 * used by any given shield are poorly documented.
 * These are the pins used to support just the Ethernet shield. In these
 * pins is buried an SPI interface... The documentation does not describe
 * which pin is used for clock, data in, or data out... Those details have
 * been gleaned from the code.
 */
#define ETHERNET_SHIELD_PIN_A    10 /* Ethernet Select */
#define ETHERNET_SHIELD_PIN_B    11
#define ETHERNET_SHIELD_PIN_C    12
#define ETHERNET_SHIELD_PIN_D    13
#define ETHERNET_SHIELD_SD_SEL    4  /* SD Select */
#define ETHERNET_SHIELD_INTERRUPT 2

/* Sensor pins. */
/* I2C is used for the BMP180 pressure sensor and the HTU21D humidity sensor. */
#define I2C_CLOCK A5
#define I2C_DATA  A4

/* Analog Pins */
#define BATTERY_PIN    A1
#define WIND_DIRECTION A2

/* Discrete Pins */
#define WIND_SPEED 2
#define RAIN_FALL  3

#define NUMBER_OF_TASKS 6

#endif
