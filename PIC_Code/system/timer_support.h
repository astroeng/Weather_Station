/* Derek Schacht 
 *  2016 01 25 
 *  Timer Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#ifndef TIMER_SUPPORT_H
#define TIMER_SUPPORT_H

#include <xc.h>
#include "picduino.h"

/* For T1CON (Timer1) configuration see the bottom of this file. Timer 1 is used
 * as the system timer so does not need to be as Arduino inspired. As such it 
 * has a less guarded setup. 
 */
#define USER_TIMER_1_INTERRUPT_VECTOR _T2Interrupt
#define USER_TIMER_2_INTERRUPT_VECTOR _T3Interrupt
#define USER_TIMER_3_INTERRUPT_VECTOR _T4Interrupt
#define USER_TIMER_4_INTERRUPT_VECTOR _T5Interrupt

#define USER_TIMER_1_INTERRUPT_FLAG_RESET() IFS0bits.T2IF = 0
#define USER_TIMER_2_INTERRUPT_FLAG_RESET() IFS0bits.T3IF = 0
#define USER_TIMER_3_INTERRUPT_FLAG_RESET() IFS1bits.T4IF = 0
#define USER_TIMER_4_INTERRUPT_FLAG_RESET() IFS1bits.T5IF = 0

#define USER_TIMER_1_INTERRUPT_PRIORITY 
#define USER_TIMER_2_INTERRUPT_PRIORITY 
#define USER_TIMER_3_INTERRUPT_PRIORITY 
#define USER_TIMER_4_INTERRUPT_PRIORITY 

typedef enum
{
    Timer_UserTimer1 = 0,
    Timer_UserTimer2,
    Timer_UserTimer3,
    Timer_UserTimer4,
    Timer_UserTimerCount
} UserTimerDeviceType;

typedef struct {
    unsigned :1;
    unsigned TCS:1;
    unsigned :1;
    unsigned T32:1;
    unsigned TCKPS:2;
    unsigned TGATE:1;
    unsigned :6;
    unsigned TSIDL:1;
    unsigned :1;
    unsigned TON:1;
} TimerControlType;

typedef enum
{
    Timer_Stop = 0,
    Timer_Start
} TimerEnableType;

typedef enum
{
    Timer_RunDuringIdle = 0,
    Timer_StopDuringIdle
} TimerIdleModeType;

typedef enum
{
    Timer_GatedTimeAccumulationDisabled = 0,
    Timer_GatedTimeAccumulationEnabled
} TimerGatedTimeAccumulationType;

typedef enum
{
    Timer_NativeClock = 0,
    Timer_EighthClock,
    Timer_SixtyFourthClock,
    Timer_TwoFiftySixthClock
} TimerPrescaleType;

typedef enum
{
    Timer_IsTwo16BitTimers = 0,
    Timer_IsSingle32BitTimer
} Timer32BitMode;

typedef enum
{
    Timer_InternalClock = 0,
    Timer_ExternalClock
} TimerClockType;

typedef struct 
{
    TimerEnableType                timerState;
    TimerIdleModeType              timerIdleConfig;
    TimerGatedTimeAccumulationType timerGateConfig;
    TimerPrescaleType              timerPrescaleConfig;
    Timer32BitMode                 timer32BitConfig;
    TimerClockType                 timerClockSourceConfig;
    RemappablePinsType             externalClockPin;
    VoidFuncPtr                    isrFunction;
    unsigned int                   timerLimit;
    unsigned char                  useInterrupts;
    unsigned char                  interruptPriority;
} TimerConfigType;

void timer_init(TimerConfigType*    config,
                RemappablePinsType  externalClockPin,
                unsigned int        limit,
                UserTimerDeviceType timerDevice);

void timer_setLimit(TimerConfigType* config, unsigned int value);
unsigned int timer_getValue(TimerConfigType* config);
void timer_setValue(TimerConfigType* config, unsigned int value);

void timer_begin(TimerConfigType* config);
void timer_disable(TimerConfigType* config);

unsigned long millis();
unsigned long micros();
void delay(unsigned long timeout);
void delayMicroseconds(unsigned int timeout);

/* System timer configuration. */

#define SYSTEM_TIMER_CONTROL T1CONbits
#define SYSTEM_TIMER_VALUE   TMR1
#define SYSTEM_TIMER_LIMIT   PR1

#define SYSTEM_TIMER_INTERRUPT_VECTOR   _T1Interrupt
#define SYSTEM_TIMER_INTERRUPT_RESET()  IFS0bits.T1IF = 0
#define SYSTEM_TIMER_INTERRUPT_ENABLE   IEC0bits.T1IE
#define SYSTEM_TIMER_INTERRUPT_PRIORITY IPC0bits.T1IP

void systemTimerConfig();

#endif