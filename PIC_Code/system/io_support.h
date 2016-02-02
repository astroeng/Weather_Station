/* Derek Schacht 
 *  2016 01 25 
 *  IO Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#ifndef IO_SUPPORT_H
#define	IO_SUPPORT_H

/* Arduino Pin | PIC IOREG 
 *          D0 | RB5
 *          D1 | RB4
 *          D2 | RD14
 *          D3 | RD15
 *          D4 | RF4
 *          D5 | RF5
 *          D6 | RF3
 *          D7 | RF2
 *          D8 | RF8
 *          D9 | RF7
 *         D10 | RF6
 *         D11 | RA2
 *         D12 | RA3
 *         D13 | RA4
 *          A0 | RB6  | AN6 
 *          A1 | RB7  | AN7 
 *          A2 | RB8  | AN8 
 *          A3 | RB9  | AN9 
 *          A4 | RB10 | AN10
 *          A5 | RB11 | AN11
 *         SCL | RG2
 *         SDA | RG3
 */

/* These defines are provided to allow direct access to the pins. Care should
 * be taken when using these define statements. 
 */

#define PIN_0_PULLUP CNPU1bits.CN7PUE
#define PIN_1_PULLUP CNPU1bits.CN6PUE
#define PIN_2_PULLUP CNPU2bits.CN20PUE
#define PIN_3_PULLUP CNPU2bits.CN21PUE
#define PIN_4_PULLUP CNPU2bits.CN17PUE
#define PIN_5_PULLUP CNPU2bits.CN18PUE
#define PIN_6_PULLUP CNPU5bits.CN71PUE
#define PIN_7_PULLUP CNPU5bits.CN70PUE
#define PIN_8_PULLUP CNPU5bits.CN74PUE
#define PIN_9_PULLUP CNPU5bits.CN73PUE
#define PIN_10_PULLUP CNPU5bits.CN72PUE
#define PIN_11_PULLUP CNPU3bits.CN35PUE
#define PIN_12_PULLUP CNPU3bits.CN36PUE
#define PIN_13_PULLUP CNPU3bits.CN37PUE
#define PIN_A0_PULLUP CNPU2bits.CN24PUE
#define PIN_A1_PULLUP CNPU2bits.CN25PUE
#define PIN_A2_PULLUP CNPU2bits.CN26PUE
#define PIN_A3_PULLUP CNPU2bits.CN27PUE
#define PIN_A4_PULLUP CNPU2bits.CN28PUE
#define PIN_A5_PULLUP CNPU2bits.CN29PUE
#define PIN_SCL_PULLUP CNPU6bits.CN83PUE
#define PIN_SDA_PULLUP CNPU6bits.CN84PUE


#define PIN_0_TRIS TRISBbits.TRISB5
#define PIN_1_TRIS TRISBbits.TRISB4
#define PIN_2_TRIS TRISDbits.TRISD14
#define PIN_3_TRIS TRISDbits.TRISD15
#define PIN_4_TRIS TRISFbits.TRISF4
#define PIN_5_TRIS TRISFbits.TRISF5
#define PIN_6_TRIS TRISFbits.TRISF3
#define PIN_7_TRIS TRISFbits.TRISF2
#define PIN_8_TRIS TRISFbits.TRISF8
#define PIN_9_TRIS TRISFbits.TRISF7
#define PIN_10_TRIS TRISFbits.TRISF6
#define PIN_11_TRIS TRISAbits.TRISA2
#define PIN_12_TRIS TRISAbits.TRISA3
#define PIN_13_TRIS TRISAbits.TRISA4
#define PIN_A0_TRIS TRISBbits.TRISB6
#define PIN_A1_TRIS TRISBbits.TRISB7
#define PIN_A2_TRIS TRISBbits.TRISB8
#define PIN_A3_TRIS TRISBbits.TRISB9
#define PIN_A4_TRIS TRISBbits.TRISB10
#define PIN_A5_TRIS TRISBbits.TRISB11
#define PIN_SCL_TRIS TRISGbits.TRISG2
#define PIN_SDA_TRIS TRISGbits.TRISG3

#define PIN_0_LAT LATBbits.LATB5
#define PIN_1_LAT LATBbits.LATB4
#define PIN_2_LAT LATDbits.LATD14
#define PIN_3_LAT LATDbits.LATD15
#define PIN_4_LAT LATFbits.LATF4
#define PIN_5_LAT LATFbits.LATF5
#define PIN_6_LAT LATFbits.LATF3
#define PIN_7_LAT LATFbits.LATF2
#define PIN_8_LAT LATFbits.LATF8
#define PIN_9_LAT LATFbits.LATF7
#define PIN_10_LAT LATFbits.LATF6
#define PIN_11_LAT LATAbits.LATA2
#define PIN_12_LAT LATAbits.LATA3
#define PIN_13_LAT LATAbits.LATA4
#define PIN_A0_LAT LATBbits.LATB6
#define PIN_A1_LAT LATBbits.LATB7
#define PIN_A2_LAT LATBbits.LATB8
#define PIN_A3_LAT LATBbits.LATB9
#define PIN_A4_LAT LATBbits.LATB10
#define PIN_A5_LAT LATBbits.LATB11
#define PIN_SCL_LAT LATGbits.LATG2
#define PIN_SDA_LAT LATGbits.LATG3

#define PIN_0_PORT PORTBbits.RB5
#define PIN_1_PORT PORTBbits.RB4
#define PIN_2_PORT PORTDbits.RD14
#define PIN_3_PORT PORTDbits.RD15
#define PIN_4_PORT PORTFbits.RF4
#define PIN_5_PORT PORTFbits.RF5
#define PIN_6_PORT PORTFbits.RF3
#define PIN_7_PORT PORTFbits.RF2
#define PIN_8_PORT PORTFbits.RF8
#define PIN_9_PORT PORTFbits.RF7
#define PIN_10_PORT PORTFbits.RF6
#define PIN_11_PORT PORTAbits.RA2
#define PIN_12_PORT PORTAbits.RA3
#define PIN_13_PORT PORTAbits.RA4
#define PIN_A0_PORT PORTBbits.RB6
#define PIN_A1_PORT PORTBbits.RB7
#define PIN_A2_PORT PORTBbits.RB8
#define PIN_A3_PORT PORTBbits.RB9
#define PIN_A4_PORT PORTBbits.RB10
#define PIN_A5_PORT PORTBbits.RB11
#define PIN_SCL_PORT PORTGbits.RG2
#define PIN_SDA_PORT PORTGbits.RG3

#define ANALOG_PINS_nENABLE1 AD1PCFGL
#define ANALOG_PINS_nENABLE2 AD1PCFGH

/* 0 thru 13 are just numbers, then there are:*/
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define SCL 20
#define SDA 21
#define LAST_IO 22

#define OUTPUT 0
#define INPUT  1
#define INPUT_PULLUP 2

#define HIGH 1
#define LOW  0


void pinInit();
void digitalWrite(unsigned char pin, unsigned char level);
void pinMode(unsigned char pin, unsigned char function);
unsigned char digitalRead(unsigned char pin);
unsigned int analogRead(unsigned char pin);

#endif

