/* Derek Schacht 
 *  2016 01 25 
 *  Remappable Pin Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#include "xc.h"
#include "remappable_pin_support.h"

char setRemappableOutputPin (RemappablePinsType outputPin, RemappableOutputFunctionType function)
{
    char output = 0;
    switch (outputPin)
    {
    case remappable_pin_io0:  RP0_OUTPUT  = function; break;
    case remappable_pin_io1:  RP1_OUTPUT  = function; break;
    case remappable_pin_io2:  RP2_OUTPUT  = function; break;
    case remappable_pin_io3:  RP3_OUTPUT  = function; break;
    case remappable_pin_io4:  RP4_OUTPUT  = function; break;
    case remappable_pin_io5:  RP5_OUTPUT  = function; break;
    case remappable_pin_io6:  RP6_OUTPUT  = function; break;
    case remappable_pin_io7:  RP7_OUTPUT  = function; break;
    case remappable_pin_io8:  RP8_OUTPUT  = function; break;
    case remappable_pin_io9:  RP9_OUTPUT  = function; break;
    case remappable_pin_io10: RP10_OUTPUT = function; break;
    case remappable_pin_io11: RP11_OUTPUT = function; break;
    case remappable_pin_io12: RP12_OUTPUT = function; break;
    case remappable_pin_io13: RP13_OUTPUT = function; break;
    case remappable_pin_io14: RP14_OUTPUT = function; break;
    case remappable_pin_io15: RP15_OUTPUT = function; break;
    case remappable_pin_io16: RP16_OUTPUT = function; break;
    case remappable_pin_io17: RP17_OUTPUT = function; break;
    case remappable_pin_io18: RP18_OUTPUT = function; break;
    case remappable_pin_io19: RP19_OUTPUT = function; break;
    case remappable_pin_io20: RP20_OUTPUT = function; break;
    case remappable_pin_io21: RP21_OUTPUT = function; break;
    case remappable_pin_io22: RP22_OUTPUT = function; break;
    case remappable_pin_io23: RP23_OUTPUT = function; break;
    case remappable_pin_io24: RP24_OUTPUT = function; break;
    case remappable_pin_io25: RP25_OUTPUT = function; break;
    case remappable_pin_io26: RP26_OUTPUT = function; break;
    case remappable_pin_io27: RP27_OUTPUT = function; break;
    case remappable_pin_io28: RP28_OUTPUT = function; break;
    case remappable_pin_io29: RP29_OUTPUT = function; break;
    case remappable_pin_io30: RP30_OUTPUT = function; break;
    case remappable_pin_io31: RP31_OUTPUT = function; break;
    default: output = 1; break;

    }
    
    return output;
}


char setRemappableInputPin (RemappablePinsType inputPin, RemappableInputFunctionType function)
{
    char output = 0;
    switch (function)
    {
    case external_interrupt_1:     EXTERNAL_INTERRUPT_1     = inputPin; break;
    case external_interrupt_2:     EXTERNAL_INTERRUPT_2     = inputPin; break;
    case external_interrupt_3:     EXTERNAL_INTERRUPT_3     = inputPin; break;
    case external_interrupt_4:     EXTERNAL_INTERRUPT_4     = inputPin; break;
    case input_capture_1:          INPUT_CAPTURE_1          = inputPin; break;
    case input_capture_2:          INPUT_CAPTURE_2          = inputPin; break;
    case input_capture_3:          INPUT_CAPTURE_3          = inputPin; break;
    case input_capture_4:          INPUT_CAPTURE_4          = inputPin; break;
    case input_capture_5:          INPUT_CAPTURE_5          = inputPin; break;
    case input_capture_6:          INPUT_CAPTURE_6          = inputPin; break;
    case input_capture_7:          INPUT_CAPTURE_7          = inputPin; break;
    case input_capture_8:          INPUT_CAPTURE_8          = inputPin; break;
    case input_capture_9:          INPUT_CAPTURE_9          = inputPin; break;
    case output_compare_fault_a:   OUTPUT_COMPARE_FAULT_A   = inputPin; break;
    case output_compare_fault_b:   OUTPUT_COMPARE_FAULT_B   = inputPin; break;
    case spi_1_clock_input:        SPI_1_CLOCK_INPUT        = inputPin; break;
    case spi_1_data_input:         SPI_1_DATA_INPUT         = inputPin; break;
    case spi_1_slave_select_input: SPI_1_SLAVE_SELECT_INPUT = inputPin; break;
    case spi_2_clock_input:        SPI_2_CLOCK_INPUT        = inputPin; break;
    case spi_2_data_input:         SPI_2_DATA_INPUT         = inputPin; break;
    case spi_2_slave_select_input: SPI_2_SLAVE_SELECT_INPUT = inputPin; break;
    case spi_3_clock_input:        SPI_3_CLOCK_INPUT        = inputPin; break;
    case spi_3_data_input:         SPI_3_DATA_INPUT         = inputPin; break;
    case spi_3_slave_select_input: SPI_3_SLAVE_SELECT_INPUT = inputPin; break;
    case timer_2_external_clock:   TIMER_2_EXTERNAL_CLOCK   = inputPin; break;
    case timer_3_external_clock:   TIMER_3_EXTERNAL_CLOCK   = inputPin; break;
    case timer_4_external_clock:   TIMER_4_EXTERNAL_CLOCK   = inputPin; break;
    case timer_5_external_clock:   TIMER_5_EXTERNAL_CLOCK   = inputPin; break;
    case uart_1_clear_to_send:     UART_1_CTS               = inputPin; break;
    case uart_1_receive:           UART_1_RX                = inputPin; break;
    case uart_2_clear_to_send:     UART_2_CTS               = inputPin; break;
    case uart_2_receive:           UART_2_RX                = inputPin; break;
    case uart_3_clear_to_send:     UART_3_CTS               = inputPin; break;
    case uart_3_receive:           UART_3_RX                = inputPin; break;
    case uart_4_clear_to_send:     UART_4_CTS               = inputPin; break;
    case uart_4_receive:           UART_4_RX                = inputPin; break;
    default: output = 1; break;
    }

    return output;
}