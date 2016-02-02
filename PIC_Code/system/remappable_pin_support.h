/* Derek Schacht 
 *  2016 01 25 
 *  Remappable Pin Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#ifndef REMAPPABLE_PIN_SUPPORT_H
#define REMAPPABLE_PIN_SUPPORT_H

#include <xc.h>

typedef enum
{
    arduino_pin0   = 18,
    arduino_pin1   = 28,
    arduino_pin2   = 43,
    arduino_pin3   =  5,
    arduino_pin4   = 10,
    arduino_pin5   = 17,
    arduino_pin6   = 16,
    arduino_pin7   = 30,
    arduino_pin8   = 15,
    arduino_pin9   = 44,
    arduino_pin10  = 45,
    arduino_pin11  = -1,
    arduino_pin12  = -2,
    arduino_pin13  = -3,
    arduino_pinA0  =  6,
    arduino_pinA1  =  7,
    arduino_pinA2  =  8,
    arduino_pinA3  =  9,
    arduino_pinA4  = -4,
    arduino_pinA5  = -5,
    arduino_pinSDA = -6,
    arduino_pinSCL = -7,
            
} ArduinoNamesType;

typedef enum
{
    remappable_pin_io0 = 0,
    remappable_pin_io1,
    remappable_pin_io2,
    remappable_pin_io3,
    remappable_pin_io4,
    remappable_pin_io5,
    remappable_pin_io6,
    remappable_pin_io7,
    remappable_pin_io8,
    remappable_pin_io9,
    remappable_pin_io10,
    remappable_pin_io11,
    remappable_pin_io12,
    remappable_pin_io13,
    remappable_pin_io14,
    remappable_pin_io15,
    remappable_pin_io16,
    remappable_pin_io17,
    remappable_pin_io18,
    remappable_pin_io19,
    remappable_pin_io20,
    remappable_pin_io21,
    remappable_pin_io22,
    remappable_pin_io23,
    remappable_pin_io24,
    remappable_pin_io25,
    remappable_pin_io26,
    remappable_pin_io27,
    remappable_pin_io28,
    remappable_pin_io29,
    remappable_pin_io30,
    remappable_pin_io31,
    remappable_pin_i32,
    remappable_pin_i33,
    remappable_pin_i34,
    remappable_pin_i35,
    remappable_pin_i36,
    remappable_pin_i37,
    remappable_pin_i38,
    remappable_pin_i39,
    remappable_pin_i40,
    remappable_pin_i41,
    remappable_pin_i42,
    remappable_pin_i43,
    remappable_pin_i44,
    remappable_pin_i45,
    remappable_pin_nc
} RemappablePinsType;

typedef enum
{
    external_interrupt_1 = 0,
    external_interrupt_2,
    external_interrupt_3,
    external_interrupt_4,
    input_capture_1,
    input_capture_2,
    input_capture_3,
    input_capture_4,
    input_capture_5,
    input_capture_6,
    input_capture_7,
    input_capture_8,
    input_capture_9,
    output_compare_fault_a,
    output_compare_fault_b,
    spi_1_clock_input,
    spi_1_data_input,
    spi_1_slave_select_input,
    spi_2_clock_input,
    spi_2_data_input,
    spi_2_slave_select_input,
    spi_3_clock_input,
    spi_3_data_input,
    spi_3_slave_select_input,
    timer_2_external_clock,
    timer_3_external_clock,
    timer_4_external_clock,
    timer_5_external_clock,
    uart_1_clear_to_send,
    uart_1_receive,
    uart_2_clear_to_send,
    uart_2_receive,
    uart_3_clear_to_send,
    uart_3_receive,
    uart_4_clear_to_send,
    uart_4_receive, 
} RemappableInputFunctionType;

typedef enum
{
    comparator_1_output       = 1,
    comparator_2_output       = 2,
    comparator_3_output       = 36,
    uart_1_transmit           = 3,
    uart_1_request_to_send    = 4,
    uart_2_transmit           = 5,
    uart_2_request_to_send    = 6,
    uart_3_transmit           = 28,
    uart_3_request_to_send    = 29,
    uart_4_transmit           = 30,
    uart_4_request_to_send    = 31,
    spi_1_data_output         = 7,
    spi_1_clock_output        = 8,
    spi_1_slave_select_output = 9,
    spi_2_data_output         = 10,
    spi_2_clock_output        = 11,
    spi_2_slave_select_output = 12,
    spi_3_data_output         = 32,
    spi_3_clock_output        = 33,
    spi_3_slave_select_output = 34,
    output_compare_1          = 18,
    output_compare_2          = 19,
    output_compare_3          = 20,
    output_compare_4          = 21,
    output_compare_5          = 22,
    output_compare_6          = 23,
    output_compare_7          = 24,
    output_compare_8          = 25,
    output_compare_9          = 35,    
} RemappableOutputFunctionType;

/* Remappable component defines */

#define EXTERNAL_INTERRUPT_1 RPINR0bits.INT1R
#define EXTERNAL_INTERRUPT_2 RPINR1bits.INT2R
#define EXTERNAL_INTERRUPT_3 RPINR1bits.INT3R
#define EXTERNAL_INTERRUPT_4 RPINR2bits.INT4R

#define INPUT_CAPTURE_1 RPINR7bits.IC1R
#define INPUT_CAPTURE_2 RPINR7bits.IC2R
#define INPUT_CAPTURE_3 RPINR8bits.IC3R
#define INPUT_CAPTURE_4 RPINR8bits.IC4R
#define INPUT_CAPTURE_5 RPINR9bits.IC5R
#define INPUT_CAPTURE_6 RPINR9bits.IC6R
#define INPUT_CAPTURE_7 RPINR10bits.IC7R
#define INPUT_CAPTURE_8 RPINR10bits.IC8R
#define INPUT_CAPTURE_9 RPINR15bits.IC9R

#define OUTPUT_COMPARE_FAULT_A RPINR11bits.OCFAR
#define OUTPUT_COMPARE_FAULT_B RPINR11bits.OCFBR

#define SPI_1_CLOCK_INPUT         RPINR20bits.SCK1R
#define SPI_1_DATA_INPUT          RPINR20bits.SDI1R
#define SPI_1_SLAVE_SELECT_INPUT  RPINR21bits.SS1R


#define SPI_2_CLOCK_INPUT         RPINR22bits.SCK2R
#define SPI_2_DATA_INPUT          RPINR22bits.SDI2R
#define SPI_2_SLAVE_SELECT_INPUT  RPINR23bits.SS2R


#define SPI_3_CLOCK_INPUT         RPINR28bits.SCK3R
#define SPI_3_DATA_INPUT          RPINR28bits.SDI3R
#define SPI_3_SLAVE_SELECT_INPUT  RPINR29bits.SS3R


#define TIMER_2_EXTERNAL_CLOCK RPINR3bits.T2CKR
#define TIMER_3_EXTERNAL_CLOCK RPINR3bits.T3CKR
#define TIMER_4_EXTERNAL_CLOCK RPINR4bits.T4CKR
#define TIMER_5_EXTERNAL_CLOCK RPINR4bits.T5CKR

#define UART_1_CTS RPINR18bits.U1CTSR
#define UART_1_RX  RPINR18bits.U1RXR


#define UART_2_CTS RPINR19bits.U2CTSR
#define UART_2_RX  RPINR19bits.U2RXR


#define UART_3_CTS RPINR21bits.U3CTSR
#define UART_3_RX  RPINR17bits.U3RXR


#define UART_4_CTS RPINR27bits.U4CTSR
#define UART_4_RX  RPINR27bits.U4RXR


#define RP0_OUTPUT  RPOR0bits.RP0R
#define RP1_OUTPUT  RPOR0bits.RP1R
#define RP2_OUTPUT  RPOR1bits.RP2R
#define RP3_OUTPUT  RPOR1bits.RP3R
#define RP4_OUTPUT  RPOR2bits.RP4R
#define RP5_OUTPUT  RPOR2bits.RP5R
#define RP6_OUTPUT  RPOR3bits.RP6R
#define RP7_OUTPUT  RPOR3bits.RP7R
#define RP8_OUTPUT  RPOR4bits.RP8R
#define RP9_OUTPUT  RPOR4bits.RP9R
#define RP10_OUTPUT RPOR5bits.RP10R
#define RP11_OUTPUT RPOR5bits.RP11R
#define RP12_OUTPUT RPOR6bits.RP12R
#define RP13_OUTPUT RPOR6bits.RP13R
#define RP14_OUTPUT RPOR7bits.RP14R
#define RP15_OUTPUT RPOR7bits.RP15R
#define RP16_OUTPUT RPOR8bits.RP16R
#define RP17_OUTPUT RPOR8bits.RP17R
#define RP18_OUTPUT RPOR9bits.RP18R
#define RP19_OUTPUT RPOR9bits.RP19R
#define RP20_OUTPUT RPOR10bits.RP20R
#define RP21_OUTPUT RPOR10bits.RP21R
#define RP22_OUTPUT RPOR11bits.RP22R
#define RP23_OUTPUT RPOR11bits.RP23R
#define RP24_OUTPUT RPOR12bits.RP24R
#define RP25_OUTPUT RPOR12bits.RP25R
#define RP26_OUTPUT RPOR13bits.RP26R
#define RP27_OUTPUT RPOR13bits.RP27R
#define RP28_OUTPUT RPOR14bits.RP28R
#define RP29_OUTPUT RPOR14bits.RP29R
#define RP30_OUTPUT RPOR15bits.RP30R
#define RP31_OUTPUT RPOR15bits.RP31R


char setRemappableOutputPin (RemappablePinsType outputPin, RemappableOutputFunctionType function);

char setRemappableInputPin (RemappablePinsType inputPin, RemappableInputFunctionType function);


#endif