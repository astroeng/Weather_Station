/* Derek Schacht 
 *  2016 01 25 
 *  SPI Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#include <xc.h>
#include <string.h>
#include "picduino.h"
#include "spi_support.h"

/* State Data */

volatile unsigned int* spiBuffers[] = 
  {&SPI_1_BUFFER,
   &SPI_2_BUFFER,
   &SPI_3_BUFFER};

volatile SpiStatusType* spiStatus[] = 
  {(SpiStatusType*)&SPI_1_STATUS,
   (SpiStatusType*)&SPI_2_STATUS,
   (SpiStatusType*)&SPI_3_STATUS};

volatile SpiControl1Type* spiControl_1[] = 
  {(SpiControl1Type*)&SPI_1_CONTROL_1,
   (SpiControl1Type*)&SPI_2_CONTROL_1,
   (SpiControl1Type*)&SPI_3_CONTROL_1};

volatile SpiControl2Type* spiControl_2[] = 
  {(SpiControl2Type*)&SPI_1_CONTROL_2,
   (SpiControl2Type*)&SPI_2_CONTROL_2,
   (SpiControl2Type*)&SPI_3_CONTROL_2};

/* Internal driver state and mode buffer */
unsigned int driverMode[SPI_DEVICES];

#define INTERRUPT_MODE 0x8000
#define MASTER_MODE    0x4000

/* These parts are only really used in interrupt mode. */
#define SPI_BUFFER_SIZE 64u

volatile unsigned int txBuffer[SPI_DEVICES][SPI_BUFFER_SIZE];
volatile unsigned int rxBuffer[SPI_DEVICES][SPI_BUFFER_SIZE];
volatile unsigned char rxTransfers[SPI_DEVICES];
volatile unsigned char txTransfers[SPI_DEVICES];

/* Internal Functions */

void isrRxTx(SpiDeviceType spiDevice)
{
    rxBuffer[spiDevice][rxTransfers[spiDevice]] = (*spiBuffers[spiDevice]);
    rxTransfers[spiDevice]++;
    if (rxTransfers[spiDevice] < txTransfers[spiDevice])
    {
        (*spiBuffers[spiDevice]) = txBuffer[spiDevice][rxTransfers[spiDevice]];
    }

    rxTransfers[spiDevice] %= SPI_BUFFER_SIZE;
}

void _USER_ISR _SPI1Interrupt ()
{
    SPI_1_INTERRUPT_FLAG_RESET();

    isrRxTx(SPI_Device1);
}

void _USER_ISR _SPI2Interrupt ()
{
    SPI_2_INTERRUPT_FLAG_RESET();

    isrRxTx(SPI_Device2);
}

void _USER_ISR _SPI3Interrupt ()
{
    SPI_3_INTERRUPT_FLAG_RESET();

    isrRxTx(SPI_Device3);
}

void spi_interruptEnable(SpiConfigType* config, char state, char priority)
{
    switch (config->spiDevice)
    {
        case SPI_Device1:
            SPI_1_INTERRUPT_ENABLE = state;
            Nop();
            SPI_1_INTERRUPT_PRIORITY = priority;
            break;
        case SPI_Device2:
            SPI_2_INTERRUPT_ENABLE = state;
            Nop();
            SPI_2_INTERRUPT_PRIORITY = priority;
            break;
        case SPI_Device3:
            SPI_3_INTERRUPT_ENABLE = state;
            Nop();
            SPI_3_INTERRUPT_PRIORITY = priority;
            break;
    }
    
    /* If interrupts are enabled the driver needs to know. */
    driverMode[config->spiDevice] &= ~INTERRUPT_MODE;
    if (state > 0)
    {
        driverMode[config->spiDevice] |= INTERRUPT_MODE;
    }
}

/* External Functions */

/* Setup for a basic SPI configuration. This is provided so that users do not
 * need to specify every single parameter. Just the ones they wish to change.
 */
void spi_init(SpiConfigType* config, 
              RemappablePinsType dataInPin, 
              RemappablePinsType dataOutPin, 
              RemappablePinsType clockPin, 
              RemappablePinsType selectPin,
              SpiDeviceType      spiDevice,
              unsigned char      useInterrupts,
              unsigned char      interruptPriority)
{   
    config->clockEdgeConfig          = SPI_DataTransitionOnActiveToIdleClock;
    config->clockPinConfig           = SPI_ClockEnable;
    config->dataOutPinConfig         = SPI_DataOutEnable;
    config->dataSampleConfig         = SPI_MiddleOfDataOut;
    config->enable                   = SPI_Enable;
    config->enhancedModeConfig       = SPI_EnhancedDisabled;
    config->frameSupportConfig       = SPI_FrameSupportDisabled;
    config->frameSyncEdgeConfig      = SPI_FrameSyncConcurrentWithFirstClock;
    config->frameSyncModeConfig      = SPI_FrameSyncMaster;
    config->frameSyncPolarityConfig  = SPI_FrameSyncActiveLow;
    config->idleClockConfig          = SPI_IdleStateLow;
    config->idleConfig               = SPI_ContinueWhenCpuIdle;
    config->interruptConfig          = SPI_InterruptWhenByteTransmitComplete;
    config->modeConfig               = SPI_MasterMode;
    config->primaryPrescalerConfig   = 3;
    config->secondaryPrescalerConfig = 5;
    config->slaveSelectConfig        = SPI_SlaveSelectNotUsed;
    config->spiClockPin              = clockPin;
    config->spiDataInPin             = dataInPin;
    config->spiDataOutPin            = dataOutPin;
    config->spiDevice                = spiDevice;
    config->spiSlaveSelectPin        = selectPin;
    config->wordConfig               = SPI_Word8;
    config->useInterrupts            = useInterrupts;
    config->interruptPriority        = interruptPriority;
}

void spi_selectSlave(SpiConfigType* config)
{
    digitalWrite(config->spiSlaveSelectPin, 0);
}

void spi_releaseSlave(SpiConfigType* config)
{
    digitalWrite(config->spiSlaveSelectPin, 1);
}

/* Function to return the number of bytes in the receive buffer.
 * NOTE: This function should only be used in interrupt mode.
 *       This will always return 0 in non interrupt mode.
 */
unsigned char spi_getRxBufferState(SpiConfigType* config)
{
    return rxTransfers[config->spiDevice];
}

/* Function to return whether or not the SPI module has received a new byte of
 * data.
 * NOTE: This function should only be used in non-interrupt mode.
 *       This will always return 0 in interrupt mode.
 */
unsigned char spi_dataAvailable(SpiConfigType* config)
{
    /* Check the SPIRBF bit in the status register to see if unread data has
     * been received.
     */
    return (spiStatus[config->spiDevice]->SPIRBF);
}

/* Return the state of the SPI device. This function will consider the mode of
 * the driver and correctly return busy if either the module is busy or the 
 * driver is waiting on more data from a previously started SPI transaction. */
unsigned char spi_busy(SpiConfigType* config)
{
    unsigned char spiBusy = txTransfers[config->spiDevice] > rxTransfers[config->spiDevice];
    if ((driverMode[config->spiDevice] & INTERRUPT_MODE) > 0)
    {
        if (!spiBusy)
        {
            spi_releaseSlave(config);
        }            
        return spiBusy;
    }
    /* Check the SPITBF bit in the status register to see if there is room for
     * more data.
     */
    return (spiStatus[config->spiDevice]->SPITBF);
}

/* This will setup a given SPI module with the specified configuration. */
void spi_begin(SpiConfigType* config)
{
    RemappableInputFunctionType spiClockInPin[SPI_DEVICES]    = {spi_1_clock_input,         spi_2_clock_input,         spi_3_clock_input};
    RemappableInputFunctionType spiSelectInPin[SPI_DEVICES]   = {spi_1_slave_select_input,  spi_2_slave_select_input,  spi_2_slave_select_input};
    RemappableInputFunctionType spiDataInPin[SPI_DEVICES]     = {spi_1_data_input,          spi_2_data_input,          spi_3_data_input};
    RemappableOutputFunctionType spiClockOutPin[SPI_DEVICES]  = {spi_1_clock_output,        spi_2_clock_output,        spi_3_clock_output};
    //RemappableOutputFunctionType spiSelectOutPin[SPI_DEVICES] = {spi_1_slave_select_output, spi_2_slave_select_output, spi_3_slave_select_output};
    RemappableOutputFunctionType spiDataOutPin[SPI_DEVICES]   = {spi_1_data_output,         spi_2_data_output,         spi_3_data_output};

    /* Turn the device off before configuring. */
    spiStatus[config->spiDevice]->SPIEN = SPI_Disable;    
    
    setRemappableInputPin(config->spiDataInPin,   spiDataInPin[config->spiDevice]);
    setRemappableOutputPin(config->spiDataOutPin, spiDataOutPin[config->spiDevice]);
    
    
    if (config->modeConfig == SPI_SlaveMode)
    {
        setRemappableInputPin(config->spiSlaveSelectPin, spiSelectInPin[config->spiDevice]);
        setRemappableInputPin(config->spiClockPin,       spiClockInPin[config->spiDevice]);
    }
    else
    {
        setRemappableOutputPin(config->spiClockPin,       spiClockOutPin[config->spiDevice]);
        
        /* This pin is apparently supposed to be software controlled when in master mode. DUMB */
        //setRemappableOutputPin(config->spiSlaveSelectPin, spiSelectOutPin[spiDevice]);
        pinMode(config->spiSlaveSelectPin, OUTPUT);
        digitalWrite(config->spiSlaveSelectPin, 1);
    }
    
    rxTransfers[config->spiDevice] = 0;
    txTransfers[config->spiDevice] = 0;
    
    if (config->useInterrupts == 1 && config->enable == SPI_Enable)
    {
        spi_interruptEnable(config, 1, config->interruptPriority);
    }
    else
    {
        spi_interruptEnable(config, 0, 0);
    }    
    
    if (config->enable == SPI_Enable)
    {
        spiStatus[config->spiDevice]->SPISIDL   = config->idleConfig;
        spiStatus[config->spiDevice]->SISEL     = config->interruptConfig;
    
        spiControl_1[config->spiDevice]->DISSCK = config->clockPinConfig;
        spiControl_1[config->spiDevice]->DISSDO = config->dataOutPinConfig;
        spiControl_1[config->spiDevice]->MODE16 = config->wordConfig;
        spiControl_1[config->spiDevice]->SMP    = config->dataSampleConfig;
        spiControl_1[config->spiDevice]->CKE    = config->clockEdgeConfig;
        spiControl_1[config->spiDevice]->SSEN   = config->slaveSelectConfig;
        spiControl_1[config->spiDevice]->CKP    = config->idleClockConfig;
        spiControl_1[config->spiDevice]->MSTEN  = config->modeConfig;
        spiControl_1[config->spiDevice]->SPRE   = config->secondaryPrescalerConfig;
        spiControl_1[config->spiDevice]->PPRE   = config->primaryPrescalerConfig;
    
        spiControl_2[config->spiDevice]->FRMEN   = config->frameSupportConfig;
        spiControl_2[config->spiDevice]->SPIFSD  = config->frameSyncModeConfig;
        spiControl_2[config->spiDevice]->SPIFPOL = config->frameSyncPolarityConfig;
        spiControl_2[config->spiDevice]->SPIFE   = config->frameSyncEdgeConfig;
        spiControl_2[config->spiDevice]->SPIBEN  = config->enhancedModeConfig;
    
        spiStatus[config->spiDevice]->SPIEN = config->enable;

    }
}

/* If interrupts are not enabled this function will send a byte of data on the 
 * SPI bus and return the received byte, blocking if necessary to wait for the
 * SPI module. If interrupts are enable this function will add the data to the
 * txBuffer. Received data can then be retrieved via the getData function.
 */
unsigned char spi_transfer(SpiConfigType* config, unsigned int data)
{
    unsigned char output = 0;
    
    if ((driverMode[config->spiDevice] & INTERRUPT_MODE) > 1)
    {
        txBuffer[config->spiDevice][txTransfers[config->spiDevice]] = data;

        /* If this is the first byte to be send or the SPI module has caught up
         * with requested transmissions then kick the module by putting the next
         * byte into the txBuffer.
         */
        if (txTransfers[config->spiDevice] == 0 || (txTransfers[config->spiDevice] == rxTransfers[config->spiDevice]))
        {
            (*spiBuffers[config->spiDevice]) = data;
        }

        txTransfers[config->spiDevice]++;
    }
    else 
    {
        while(spi_busy(config) > 0) {}
        (*spiBuffers[config->spiDevice]) = data;
        while(spi_busy(config) > 0) {}
        output = (*spiBuffers[config->spiDevice]);
    }
    
    return output;
}

/* Returns the byte count copied to the transmit buffer. The data in the
 * provided buffer is copied into the transmit buffer. This function returns 
 * the number of SPI transactions that will be performed for this send.
 * NOTE: This function is only useful in interrupt mode.
 */
unsigned int spi_sendData(SpiConfigType* config, unsigned int* data, size_t size)
{
    spi_selectSlave(config);
    
    memcpy((void*)txBuffer[config->spiDevice], (void*)data, size);
    
    rxTransfers[config->spiDevice] = 0;
    txTransfers[config->spiDevice] = size / sizeof(int);
    
    /* Kick the SPI module to start the transfer. */
    (*spiBuffers[config->spiDevice]) = txBuffer[config->spiDevice][0];
    
    return txTransfers[config->spiDevice];
}


/* Returns the byte count contained in the receive buffer. The data in the
 * receive buffer is copied into the pointer specified as the second parameter.
 * This function returns the number of SPI transactions since the last time
 * getData was called.
 * NOTE: This function is only useful in interrupt mode.
 */
unsigned int spi_getData(SpiConfigType* config, unsigned int* data, size_t size)
{
    unsigned int bytesReceived = 0;

    memcpy((void*)data, (void*)rxBuffer[config->spiDevice], size);

    bytesReceived = rxTransfers[config->spiDevice];

    rxTransfers[config->spiDevice] = 0;
    txTransfers[config->spiDevice] = 0;

    return bytesReceived;
}

void spi_disable(SpiConfigType* config)
{
    spiStatus[config->spiDevice]->SPIEN = 0;
    Nop();
}
