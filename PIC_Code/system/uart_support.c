/* Derek Schacht 
 *  2016 01 30
 *  UART Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#include <xc.h>
#include "uart_support.h"
#include "timer_support.h"

#define UART_BUFFER_SIZE 39
#define UART_COUNT       4

typedef struct 
{
    unsigned URXDA:1;
    unsigned OERR:1;
    unsigned FERR:1;
    unsigned PERR:1;
    unsigned RIDLE:1;
    unsigned ADDEN:1;
    unsigned URXISEL:2;
    unsigned TRMT:1;
    unsigned UTXBF:1;
    unsigned UTXEN:1;
    unsigned UTXBRK:1;
    unsigned :1;
    unsigned UTXISEL0:1;
    unsigned UTXINV:1;
    unsigned UTXISEL1:1;
} UartStatusType;

typedef struct 
{
    unsigned STSEL:1;
    unsigned PDSEL:2;
    unsigned BRGH:1;
    unsigned RXINV:1;
    unsigned ABAUD:1;
    unsigned LPBACK:1;
    unsigned WAKE:1;
    unsigned UEN:2;
    unsigned :1;
    unsigned RTSMD:1;
    unsigned IREN:1;
    unsigned USIDL:1;
    unsigned :1;
    unsigned UARTEN:1;
} UartModeType;

volatile  UartStatusType* uartStatus[] = {(UartStatusType*)&U1STAbits,
                                          (UartStatusType*)&U2STAbits,
                                          (UartStatusType*)&U3STAbits,
                                          (UartStatusType*)&U4STAbits};

volatile  UartModeType* uartMode[] = {(UartModeType*)&U1MODEbits,
                                      (UartModeType*)&U1MODEbits,
                                      (UartModeType*)&U1MODEbits,
                                      (UartModeType*)&U1MODEbits};

volatile unsigned int* uartBaud[] = {&U1BRG,
                                     &U2BRG,
                                     &U3BRG,
                                     &U4BRG};

volatile unsigned int* uartTx[] = {&U1TXREG,
                                   &U2TXREG,
                                   &U3TXREG,
                                   &U4TXREG};

volatile unsigned int* uartRx[] = {&U1RXREG,
                                   &U2RXREG,
                                   &U3RXREG,
                                   &U4RXREG};

volatile unsigned char txBuffers[UART_COUNT][UART_BUFFER_SIZE];
volatile unsigned char rxBuffers[UART_COUNT][UART_BUFFER_SIZE];

volatile unsigned int txBufferIndex[UART_COUNT];
volatile unsigned int txBufferState[UART_COUNT];
volatile unsigned int rxBufferIndex[UART_COUNT];

#define isr_busy(x) txBufferIndex[x] < txBufferState[x]
#define uart_busy(x) uartStatus[x]->UTXBF == 1

#define UART1_RX_INTERRUPT_STATE(x) IEC0bits.U1RXIE = x
#define UART2_RX_INTERRUPT_STATE(x) IEC1bits.U2RXIE = x
#define UART3_RX_INTERRUPT_STATE(x) IEC5bits.U3RXIE = x
#define UART4_RX_INTERRUPT_STATE(x) IEC5bits.U4RXIE = x

#define UART1_RX_INTERRUPT_ACK() IFS0bits.U1RXIF = 0
#define UART2_RX_INTERRUPT_ACK() IFS1bits.U2RXIF = 0
#define UART3_RX_INTERRUPT_ACK() IFS5bits.U3RXIF = 0
#define UART4_RX_INTERRUPT_ACK() IFS5bits.U4RXIF = 0

#define UART1_TX_INTERRUPT_STATE(x) IEC0bits.U1TXIE = x
#define UART2_TX_INTERRUPT_STATE(x) IEC1bits.U2TXIE = x
#define UART3_TX_INTERRUPT_STATE(x) IEC5bits.U3TXIE = x
#define UART4_TX_INTERRUPT_STATE(x) IEC5bits.U4TXIE = x

#define UART1_TX_INTERRUPT_ACK() IFS0bits.U1TXIF = 0
#define UART2_TX_INTERRUPT_ACK() IFS1bits.U2TXIF = 0
#define UART3_TX_INTERRUPT_ACK() IFS5bits.U3TXIF = 0
#define UART4_TX_INTERRUPT_ACK() IFS5bits.U4TXIF = 0

void isrRx(UartDeviceType device)
{
    rxBuffers[device][rxBufferIndex[device]] = (*uartRx[device]);
    rxBufferIndex[device]++;
}

void isrTx(UartDeviceType device)
{
    if (isr_busy(device))
    {
        (*uartTx[device]) = txBuffers[device][txBufferIndex[device]];
        txBufferIndex[device]++;
    }
}

void _SYSTEM_ISR _U1RXInterrupt()
{
    UART1_RX_INTERRUPT_ACK();
    isrRx(UART_Device1);
}

void _SYSTEM_ISR _U1TXInterrupt()
{
    UART1_TX_INTERRUPT_ACK();
    isrTx(UART_Device1);
}

void _SYSTEM_ISR _U2RXInterrupt()
{
    UART2_RX_INTERRUPT_ACK();
    isrRx(UART_Device2);
}

void _SYSTEM_ISR _U2TXInterrupt()
{
    UART2_TX_INTERRUPT_ACK();
    isrTx(UART_Device2);
}

void _SYSTEM_ISR _U3RXInterrupt()
{
    UART3_RX_INTERRUPT_ACK();
    isrRx(UART_Device3);
}

void _SYSTEM_ISR _U3TXInterrupt()
{
    UART3_TX_INTERRUPT_ACK();
    isrTx(UART_Device3);
}

void _SYSTEM_ISR _U4RXInterrupt()
{
    UART4_RX_INTERRUPT_ACK();
    isrRx(UART_Device4);
}

void _SYSTEM_ISR _U4TXInterrupt()
{
    UART4_TX_INTERRUPT_ACK();
    isrTx(UART_Device4);
}

void uart_init(UartConfigType*    config,
               UartDeviceType     device,
               unsigned long      baudRate,
               RemappablePinsType rtsPin,
               RemappablePinsType ctsPin,
               RemappablePinsType rxPin,
               RemappablePinsType txPin)
{

    config->addressDetectMode = 0;
    config->baudMode          = UART_StandardBaudMode;
    config->baudCount         = 0xffff;
    config->device            = device;
    config->idleMode          = UART_ContinueWhenCpuIdle;
    config->idleWake          = UART_NoWake;
    config->irDaMode          = UART_IrDADisabled;
    config->irDaTxPolarity    = 0;
    config->loopback          = UART_LoopbackDisabled;
    config->parityAndDataBits = UART_8BitNoParity;
    config->pinsMode          = UART_Tx_Rx_Enabled;
    config->rtsMode           = UART_RtsFlowControlMode;
    config->rxInterruptMode   = 0;
    config->rxPolarity        = UART_RxNormallyHigh;
    config->stopBits          = UART_OneStopBit;
    config->txEnable          = 1;
    config->txInterruptMode   = 0;
    config->uartEnable        = 1;
    
    config->rtsPin = rtsPin;
    config->ctsPin = ctsPin;
    config->rxPin  = rxPin;
    config->txPin  = txPin;
    
    if (baudRate > 100000)
    {
        config->baudMode = UART_HighBaudMode;
        config->baudCount = (CORE_CLOCK_HZ / ( 4UL * baudRate )) - 1;
    }
    else
    {
        config->baudCount = (CORE_CLOCK_HZ / ( 16UL * baudRate )) - 1;
    }

}

void uart_begin(UartConfigType* config)
{
    unsigned int index = config->device;
    
    uartMode[index]->UARTEN = UART_Disabled;
    
    setRemappableOutputPin(config->txPin, uart_1_transmit);
    setRemappableInputPin(config->rxPin,  uart_1_receive);
    
    uartMode[index]->ABAUD  = 0;
    uartMode[index]->BRGH   = config->baudMode;
    uartMode[index]->IREN   = config->irDaMode;
    uartMode[index]->LPBACK = config->loopback;
    uartMode[index]->PDSEL  = config->parityAndDataBits;
    uartMode[index]->RTSMD  = config->rtsMode;
    uartMode[index]->RXINV  = config->rxPolarity;
    uartMode[index]->STSEL  = config->stopBits;
    uartMode[index]->UEN    = config->pinsMode;
    uartMode[index]->USIDL  = config->idleMode;
    uartMode[index]->WAKE   = config->idleWake;
    
    (*uartBaud[index]) = config->baudCount;
    
    uartStatus[index]->ADDEN    = config->addressDetectMode;
    uartStatus[index]->URXISEL  = 1;
    uartStatus[index]->UTXINV   = config->irDaTxPolarity;
    uartStatus[index]->UTXISEL0 = 0;
    uartStatus[index]->UTXISEL1 = 0;

    UART1_RX_INTERRUPT_STATE(1);
    UART1_TX_INTERRUPT_STATE(1);
    
    uartMode[index]->UARTEN = UART_Enabled;

    uartStatus[index]->UTXEN = config->txEnable;
}

/* This will send a buffer of pretty much any length.. if the buffer is smaller
 * than the TX buffer for the UART this function will return on a single pass.
 * If the buffer is longer than the TX buffer this function will block until the
 * last bit of data has been copied to the UART TX buffer. If a second transfer
 * is started before the first on has been completely sent, this will block.
 */
void uart_sendData(UartConfigType* config, unsigned char* dataBuffer, size_t size)
{
    if (size == 0)
    {
        return;
    }
    
    unsigned int dataSent = 0;
    unsigned int deviceIndex = config->device;
    size_t dataToSend;
    
    do
    {
        /* Check to make sure the ISR and the UART are ready to do some work. */
        while (isr_busy(deviceIndex)) {}
        while (uart_busy(deviceIndex)) {}
        
        dataToSend = min(size, UART_BUFFER_SIZE);

        memcpy((void*)txBuffers[deviceIndex], (void*)&dataBuffer[dataSent], dataToSend);
        
        dataSent += dataToSend;
        size     -= dataToSend;
    
        txBufferState[deviceIndex] = dataToSend;
        /* Start at byte 1 since byte 0 is sent below. */
        txBufferIndex[deviceIndex] = 1;
        /* Kick the UART module to start the transfer. */
        (*uartTx[deviceIndex]) = txBuffers[deviceIndex][0];
        
    } while (size > 0);
}

void uart_getData(UartConfigType* device, unsigned char* dataBuffer, size_t size)
{
    memcpy((void*)dataBuffer, (void*)rxBuffers[device->device], size);
    rxBufferIndex[device->device] = 0;
}

void uart_write(UartConfigType* device, unsigned char data)
{
    while (uart_busy(deviceIndex)) {}
    (*uartTx[device->device]) = data;
}

unsigned int uart_dataAvailable(UartConfigType* device)
{
    return rxBufferIndex[device->device];
}
