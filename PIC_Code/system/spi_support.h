/* Derek Schacht 
 *  2016 01 25 
 *  SPI Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#ifndef SPI_SUPPORT_H
#define SPI_SUPPORT_H

#include "string.h"
#include "remappable_pin_support.h"

/* This driver has two modes of operation [Normal or Interrupt] mode. In Normal
 * mode the user must manage each transaction on the SPI bus word by word. The
 * user actions are described in Diagram A below. In Interrupt mode the drive
 * deals with the SPI data transactions and far less user interaction The user
 * actions for Interrupt mode are described in Diagram B & C. All of the 
 * diagrams assume the SPI module is already setup via the 'spi_begin' call.
 * 
 * Diagram A : (Normal Mode)
 * 
 *   spi_selectSlave
 *   spi_transfer<0> ...... spi_transfer<n-1>
 *   spi_releaseSlave
 * 
 * Diagram B : (Interrupt Mode)
 * 
 *   spi_sendData
 * 
 *   * Other code not related to the now busy SPI module may be done here while 
 *   * the SPI module is doing the work in the background. Say maybe starting
 *   * another SPI transfer on a different SPI module.
 * 
 *   while(spi_busy) {}
 *   spi_getData
 *   
 * Diagram C : (Interrupt Mode)
 * 
 *   spi_selectSlave
 *   spi_transfer<0> ...... spi_transfer<n-1>
 * 
 *   * Other code not related to the now busy SPI module may be done here while 
 *   * the SPI module is doing the work in the background. Say maybe starting
 *   * another SPI transfer on a different SPI module.
 * 
 *   while(spi_busy) {}
 *   spi_getData
 */

/* There are 3 SPI modules on the PIC24. PIC24 allows these modules to be
 * routed to any number of IO pins via the Remappable Pin Feature. 
 */

#define SPI_DEVICES 3

#define SPI_1_INTERRUPT_FLAG_RESET() IFS0bits.SPI1IF = 0
#define SPI_1_INTERRUPT_ENABLE       IEC0bits.SPI1IE
#define SPI_1_INTERRUPT_PRIORITY     IPC2bits.SPI1IP
#define SPI_1_CONTROL_1              SPI1CON1bits
#define SPI_1_CONTROL_2              SPI1CON2bits
#define SPI_1_STATUS                 SPI1STATbits
#define SPI_1_BUFFER                 SPI1BUF

#define SPI_2_INTERRUPT_FLAG_RESET() IFS2bits.SPI2IF = 0
#define SPI_2_INTERRUPT_ENABLE       IEC2bits.SPI2IE
#define SPI_2_INTERRUPT_PRIORITY     IPC8bits.SPI2IP
#define SPI_2_CONTROL_1              SPI2CON1bits
#define SPI_2_CONTROL_2              SPI2CON2bits
#define SPI_2_STATUS                 SPI2STATbits
#define SPI_2_BUFFER                 SPI2BUF

#define SPI_3_INTERRUPT_FLAG_RESET() IFS5bits.SPI3IF = 0
#define SPI_3_INTERRUPT_ENABLE       IEC5bits.SPI3IE
#define SPI_3_INTERRUPT_PRIORITY     IPC22bits.SPI3IP
#define SPI_3_CONTROL_1              SPI3CON1bits
#define SPI_3_CONTROL_2              SPI3CON2bits
#define SPI_3_STATUS                 SPI3STATbits
#define SPI_3_BUFFER                 SPI3BUF

typedef struct 
{
    unsigned SPIRBF:1;
    unsigned SPITBF:1;
    unsigned SISEL:3;
    unsigned SRXMPT:1;
    unsigned SPIROV:1;
    unsigned SRMPT:1;
    unsigned SPIBEC:3;
    unsigned :2;
    unsigned SPISIDL:1;
    unsigned :1;
    unsigned SPIEN:1;
} SpiStatusType;

typedef struct 
{
    unsigned PPRE:2;
    unsigned SPRE:3;
    unsigned MSTEN:1;
    unsigned CKP:1;
    unsigned SSEN:1;
    unsigned CKE:1;
    unsigned SMP:1;
    unsigned MODE16:1;
    unsigned DISSDO:1;
    unsigned DISSCK:1;
} SpiControl1Type;

typedef struct {
  unsigned SPIBEN:1;
  unsigned SPIFE:1;
  unsigned :11;
  unsigned SPIFPOL:1;
  unsigned SPIFSD:1;
  unsigned FRMEN:1;
} SpiControl2Type;

typedef enum {
    SPI_Device1 = 0,
    SPI_Device2,
    SPI_Device3
} SpiDeviceType;

typedef enum
{
    SPI_Disable = 0,    
    SPI_Enable
} SpiEnableType;

typedef enum
{
    SPI_ContinueWhenCpuIdle    = 0,
    SPI_DiscontinueWhenCpuIdle = 1,
} SpiIdleModeType;

typedef enum
{
    SPI_InterruptOnLastReadOfReceiveBuffer = 0,
    SPI_InterruptOnDataAvailableInReceiveBuffer,
    SPI_InterruptWhenReceiveBufferIsOver75PercentFull,
    SPI_InterruptWhenReceiveBufferFull,
    SPI_InterruptWhenTxFifoHasOneSpot,
    SPI_InterruptWhenByteTransmitComplete,
    SPI_InterruptWhenTxFifoEmpty,
    SPI_InterruptWhenTxBufferFull
} SpiInterruptModeType;

typedef enum
{
    SPI_ClockEnable = 0,
    SPI_ClockDisable
} SpiClockEnableType;

typedef enum
{
    SPI_DataOutEnable = 0,
    SPI_DataOutDisable
} SpiDataOutEnableType;

typedef enum
{
    SPI_Word8 = 0,
    SPI_Word16
} SpiWordWidthType;

typedef enum
{
    SPI_MiddleOfDataOut = 0,
    SPI_EndofDataOut
} SpiDataSamplePointType;

typedef enum
{
    SPI_DataTransitionOnIdleToActiveClock = 0,
    SPI_DataTransitionOnActiveToIdleClock
} SpiClockEdgeUpdateType;

typedef enum
{
    SPI_SlaveSelectNotUsed = 0,
    SPI_SlaveSelectUsed        
} SpiSlaveModeSelectType;

typedef enum
{
    SPI_IdleStateLow = 0,
    SPI_IdleStateHigh        
} SpiIdleClockPolarityType;

typedef enum
{
    SPI_SlaveMode = 0,
    SPI_MasterMode       
} SpiModeType;

typedef enum
{
    SPI_FrameSupportDisabled = 0,
    SPI_FrameSupportEnabled
} SpiFrameSupportType;

typedef enum
{
    SPI_FrameSyncMaster = 0,
    SPI_FrameSyncSlave
} SpiFrameSyncModeType;

typedef enum
{
    SPI_FrameSyncActiveLow = 0,
    SPI_FrameSyncActiveHigh
} SpiFrameSyncPolarityType;

typedef enum
{
    SPI_FrameSyncBeforeFirstClock = 0,
    SPI_FrameSyncConcurrentWithFirstClock
} SpiFrameSyncEdgeType;

typedef enum
{
    SPI_EnhancedDisabled = 0,
    SPI_EnhancedEnabled
} SpiEnhancedModeType;

typedef struct
{
    SpiDeviceType            spiDevice;
    SpiEnableType            enable;
    SpiIdleModeType          idleConfig;
    SpiInterruptModeType     interruptConfig;
    SpiClockEnableType       clockPinConfig;
    SpiDataOutEnableType     dataOutPinConfig;
    SpiWordWidthType         wordConfig;
    SpiDataSamplePointType   dataSampleConfig;
    SpiClockEdgeUpdateType   clockEdgeConfig;
    SpiSlaveModeSelectType   slaveSelectConfig;
    SpiIdleClockPolarityType idleClockConfig;
    SpiModeType              modeConfig;
    unsigned char            secondaryPrescalerConfig;
    unsigned char            primaryPrescalerConfig;
    SpiFrameSupportType      frameSupportConfig;
    SpiFrameSyncModeType     frameSyncModeConfig;
    SpiFrameSyncPolarityType frameSyncPolarityConfig;
    SpiFrameSyncEdgeType     frameSyncEdgeConfig;
    SpiEnhancedModeType      enhancedModeConfig;
    RemappablePinsType       spiDataOutPin;
    RemappablePinsType       spiDataInPin;
    RemappablePinsType       spiClockPin;
    RemappablePinsType       spiSlaveSelectPin;
    unsigned char            useInterrupts;
    unsigned char            interruptPriority;
} SpiConfigType;



void spi_init(SpiConfigType* config,
              RemappablePinsType dataInPin, 
              RemappablePinsType dataOutPin, 
              RemappablePinsType clockPin, 
              RemappablePinsType selectPin,
              SpiDeviceType      spiDevice,
              unsigned char      useInterrupts,
              unsigned char      interruptPriority);

void spi_selectSlave(SpiConfigType* config);
void spi_releaseSlave(SpiConfigType* config);

unsigned char spi_getRxBufferState(SpiConfigType* config);
unsigned char spi_dataAvailable(SpiConfigType* config);
unsigned char spi_busy(SpiConfigType* config);

void spi_begin(SpiConfigType* config);
unsigned char spi_transfer(SpiConfigType* config, unsigned int data);
unsigned int spi_sendData(SpiConfigType* config, unsigned int* data, size_t size);
unsigned int spi_getData(SpiConfigType* config, unsigned int* data, size_t size);

void spi_disable(SpiConfigType* config);

#endif