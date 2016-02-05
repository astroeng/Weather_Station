/* Derek Schacht 
 *  2016 01 25 
 *  IO Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */
#include "xc.h"
#include "io_support.h"

void pinInit()
{
    ANALOG_PINS_nENABLE1 = 0xffff;
    ANALOG_PINS_nENABLE2 = 0xffff;
}

void pinMode(unsigned char pin, unsigned char function)
{
    char pullup = 0;
    if (function == INPUT_PULLUP)
    {
        pullup = 1;
        function = INPUT;
    }
    
    switch(pin)
    {
    case 0: PIN_0_TRIS = function; PIN_0_PULLUP = pullup; break;
    case 1: PIN_1_TRIS = function; PIN_1_PULLUP = pullup; break;
    case 2: PIN_2_TRIS = function; PIN_2_PULLUP = pullup; break;
    case 3: PIN_3_TRIS = function; PIN_3_PULLUP = pullup; break;
    case 4: PIN_4_TRIS = function; PIN_4_PULLUP = pullup; break;
    case 5: PIN_5_TRIS = function; PIN_5_PULLUP = pullup; break;
    case 6: PIN_6_TRIS = function; PIN_6_PULLUP = pullup; break;
    case 7: PIN_7_TRIS = function; PIN_7_PULLUP = pullup; break;
    case 8: PIN_8_TRIS = function; PIN_8_PULLUP = pullup; break;
    case 9: PIN_9_TRIS = function; PIN_9_PULLUP = pullup; break;
    case 10: PIN_10_TRIS = function; PIN_10_PULLUP = pullup; break;
    case 11: PIN_11_TRIS = function; PIN_11_PULLUP = pullup; break;
    case 12: PIN_12_TRIS = function; PIN_12_PULLUP = pullup; break;
    case 13: PIN_13_TRIS = function; PIN_13_PULLUP = pullup; break;
    case A0: PIN_A0_TRIS = function; PIN_A0_PULLUP = pullup; break;
    case A1: PIN_A1_TRIS = function; PIN_A1_PULLUP = pullup; break;
    case A2: PIN_A2_TRIS = function; PIN_A2_PULLUP = pullup; break;
    case A3: PIN_A3_TRIS = function; PIN_A3_PULLUP = pullup; break;
    case A4: PIN_A4_TRIS = function; PIN_A4_PULLUP = pullup; break;
    case A5: PIN_A5_TRIS = function; PIN_A5_PULLUP = pullup; break;
    case SCL: PIN_SCL_TRIS = function; PIN_SCL_PULLUP = pullup; break;
    case SDA: PIN_SDA_TRIS = function; PIN_SDA_PULLUP = pullup; break;
    }
    Nop();
}

void digitalWrite(unsigned char pin, unsigned char level)
{
    switch(pin)
    {
    case 0: PIN_0_LAT = level; break;
    case 1: PIN_1_LAT = level; break;
    case 2: PIN_2_LAT = level; break;
    case 3: PIN_3_LAT = level; break;
    case 4: PIN_4_LAT = level; break;
    case 5: PIN_5_LAT = level; break;
    case 6: PIN_6_LAT = level; break;
    case 7: PIN_7_LAT = level; break;
    case 8: PIN_8_LAT = level; break;
    case 9: PIN_9_LAT = level; break;
    case 10: PIN_10_LAT = level; break;
    case 11: PIN_11_LAT = level; break;
    case 12: PIN_12_LAT = level; break;
    case 13: PIN_13_LAT = level; break;
    case A0: PIN_A0_LAT = level; break;
    case A1: PIN_A1_LAT = level; break;
    case A2: PIN_A2_LAT = level; break;
    case A3: PIN_A3_LAT = level; break;
    case A4: PIN_A4_LAT = level; break;
    case A5: PIN_A5_LAT = level; break;
    case SCL: PIN_SCL_LAT = level; break;
    case SDA: PIN_SDA_LAT = level; break;
    }
    Nop();
}

unsigned char digitalRead(unsigned char pin)
{
    switch(pin)
    {
    case 0: return PIN_0_PORT;
    case 1: return PIN_1_PORT;
    case 2: return PIN_2_PORT;
    case 3: return PIN_3_PORT;
    case 4: return PIN_4_PORT;
    case 5: return PIN_5_PORT;
    case 6: return PIN_6_PORT;
    case 7: return PIN_7_PORT;
    case 8: return PIN_8_PORT;
    case 9: return PIN_9_PORT;
    case 10: return PIN_10_PORT;
    case 11: return PIN_11_PORT;
    case 12: return PIN_12_PORT;
    case 13: return PIN_13_PORT;
    case A0: return PIN_A0_PORT;
    case A1: return PIN_A1_PORT;
    case A2: return PIN_A2_PORT;
    case A3: return PIN_A3_PORT;
    case A4: return PIN_A4_PORT;
    case A5: return PIN_A5_PORT;
    case SCL: return PIN_SCL_PORT;
    case SDA: return PIN_SDA_PORT;
    }
    return -1;
}
