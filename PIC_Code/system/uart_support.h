/* Derek Schacht 
 *  2016 01 30 
 *  UART Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#ifndef UART_SUPPORT_H
#define UART_SUPPORT_H

#include <string.h>
#include "remappable_pin_support.h"

typedef enum
{
    UART_Device1 = 0,
    UART_Device2,
    UART_Device3,
    UART_Device4
} UartDeviceType;

typedef enum
{
    UART_Disabled = 0,
    UART_Enabled
} UartEnableType;

typedef enum
{
    UART_ContinueWhenCpuIdle    = 0,
    UART_DiscontinueWhenCpuIdle
} UartIdleModeType;

typedef enum
{
    UART_IrDADisabled = 0,
    UART_IrDAEnabled
} UartIrDAType;

typedef enum
{
    UART_RtsFlowControlMode = 0,
    UART_RtsSimplexMode
} UartRtsModeType;

typedef enum
{
    UART_Tx_Rx_Enabled = 0,
    UART_Tx_Rx_Rts_Enabled,
    UART_Tx_Rx_Cts_Rts_Enabled,
    UART_Tx_Rx_Bclk_Enabled
} UartPinsEnableType;

typedef enum
{
    UART_NoWake = 0,
    UART_InterruptOnFallingEdgeRx
} UartWakeType;

typedef enum
{
    UART_LoopbackDisabled = 0,
    UART_LoopbackEnabled
} UartLoopbackType;

typedef enum
{
    UART_RxNormallyHigh = 0,
    UART_RxNormallyLow
} UartRxPolarityType;

typedef enum
{
    UART_StandardBaudMode = 0,
    UART_HighBaudMode
} UartBaudRateModeType;

typedef enum
{
    UART_8BitNoParity = 0,
    UART_8BitEvenParity,
    UART_8BitOddParity,
    UART_9bitNoParity
} UartParityAndDataSelectionType;

typedef enum
{
    UART_OneStopBit = 0,
    UART_TwoStopBits
} UartStopBitSelectionType;

typedef enum
{
    Empty,
} UartTxInterruptModeType;

typedef enum
{
    Empty1,
} UartIrDaEncoderTxPolarityType;

/* Skipping "break" because it is a function of the UART and not a configuration.
 * There is a function to start a break sequence. 
 */


typedef enum
{
    Empty2,
} UartTxEnableType;

/* Tx Buffer Status and Tx Shift Register Empty bits are not a configuration.
 * There is a function for checking those bits.
 */

typedef enum
{
    Empty3,
} UartRxInterruptModeType;

typedef enum
{
    Empty4,
} UartAddressDetectModeType;

/* Rx idle, parity error, framing error, overrun error, rx data available are 
 * all status bits. There are functions for interactions with those registers. 
 */

typedef struct
{
    UartDeviceType                 device;
    UartEnableType                 uartEnable;
    UartIdleModeType               idleMode;
    UartIrDAType                   irDaMode;
    UartRtsModeType                rtsMode;
    UartPinsEnableType             pinsMode;
    UartWakeType                   idleWake;
    UartLoopbackType               loopback;
    UartRxPolarityType             rxPolarity;
    UartBaudRateModeType           baudMode;
    UartParityAndDataSelectionType parityAndDataBits;
    UartStopBitSelectionType       stopBits;
    UartTxInterruptModeType        txInterruptMode;
    UartIrDaEncoderTxPolarityType  irDaTxPolarity;
    UartTxEnableType               txEnable;
    UartRxInterruptModeType        rxInterruptMode;
    UartAddressDetectModeType      addressDetectMode;
    RemappablePinsType             rtsPin;
    RemappablePinsType             ctsPin;
    RemappablePinsType             rxPin;
    RemappablePinsType             txPin;
    unsigned int                   baudCount;
} UartConfigType;

void uart_init(UartConfigType*    config,
               UartDeviceType     device,
               unsigned long      baudRate,
               RemappablePinsType rtsPin,
               RemappablePinsType ctsPin,
               RemappablePinsType rxPin,
               RemappablePinsType txPin);

void uart_begin(UartConfigType* device);

void uart_sendBreak(UartConfigType* device);

void uart_sendData(UartConfigType* device, unsigned char* dataBuffer, size_t size);
void uart_getData(UartConfigType* device, unsigned char* dataBuffer, size_t size);
void uart_write(UartConfigType* device, unsigned char data);
unsigned int uart_dataAvailable(UartConfigType* device);

#endif