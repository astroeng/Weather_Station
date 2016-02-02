/* Derek Schacht 
 *  2016 01 25 
 *  Timer Support for PIC24 devices.
 * 
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 */

#include <xc.h>
#include "timer_support.h"
#include "picduino.h"

void defaultTimerISR()
{

}

TimerControlType* timerControl[] = {(TimerControlType*)&T2CON,
                                    (TimerControlType*)&T3CON,
                                    (TimerControlType*)&T4CON,
                                    (TimerControlType*)&T5CON};

VoidFuncPtr timerISR[] = {defaultTimerISR,
                          defaultTimerISR,
                          defaultTimerISR,
                          defaultTimerISR};

volatile unsigned long internalMilliseconds = 0;

void _SYSTEM_ISR USER_TIMER_1_INTERRUPT_VECTOR()
{
    USER_TIMER_1_INTERRUPT_FLAG_RESET();
    timerISR[0]();
}

void _SYSTEM_ISR USER_TIMER_2_INTERRUPT_VECTOR()
{
    USER_TIMER_2_INTERRUPT_FLAG_RESET();
    timerISR[1]();
}

void _SYSTEM_ISR USER_TIMER_3_INTERRUPT_VECTOR()
{
    USER_TIMER_3_INTERRUPT_FLAG_RESET();
    timerISR[2]();
}

void _SYSTEM_ISR USER_TIMER_4_INTERRUPT_VECTOR()
{
    USER_TIMER_4_INTERRUPT_FLAG_RESET();
    timerISR[3]();
}


void timer_init(TimerConfigType*    config,
                RemappablePinsType  externalClockPin,
                unsigned int        limit,
                UserTimerDeviceType timerDevice)
{

}

void timer_begin(TimerConfigType* config)
{

}

unsigned long micros()
{
    return (internalMilliseconds * 1000ul) + (unsigned long)(SYSTEM_TIMER_VALUE >> 1);
}

unsigned long millis()
{
    return internalMilliseconds;
}

void delay(unsigned long timeout)
{
    unsigned long limit = internalMilliseconds + timeout;

    while(limit > internalMilliseconds)
    {
        Nop();
    }
}

void delayMicroseconds(unsigned int timeout)
{
    unsigned long limit = micros() + (unsigned long)timeout;

    while(limit > micros())
    {
        Nop();
    }
}



/* System timer clock frequency of 2 mhz this will clock every 0.5 us. 
 * Using this timer with a 1ms interrupt the system will be able to measure
 * timing at the uSecond level. This all assumes a 16mhz core clock. If the user
 * changes the core clock the CORE_CLOCK_HZ define should be updated. This will
 * attempt to set the system timer to best emulate the behavior tested with a 
 * 16mhz core clock
 */

void systemTimerConfig()
{
    SYSTEM_TIMER_CONTROL.TON = 0;
    SYSTEM_TIMER_CONTROL.TSIDL = 0;
    SYSTEM_TIMER_CONTROL.TGATE = 0;
    SYSTEM_TIMER_CONTROL.TCKPS = 1;
    SYSTEM_TIMER_CONTROL.TSYNC = 0;
    SYSTEM_TIMER_CONTROL.TCS   = 0;
    
    SYSTEM_TIMER_LIMIT = (CORE_CLOCK_HZ / 8000) - 2;
    
    SYSTEM_TIMER_INTERRUPT_ENABLE = 1;
    
    SYSTEM_TIMER_CONTROL.TON = 1;
}

void _SYSTEM_ISR SYSTEM_TIMER_INTERRUPT_VECTOR()
{
    SYSTEM_TIMER_INTERRUPT_RESET();
    internalMilliseconds++;
}