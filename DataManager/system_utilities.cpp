/* Derek Schacht
 *  2016 03 03
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include "system_utilities.h"

timespec boot;
volatile int serverRunning;

void time_init()
{
  clock_gettime(CLOCK_REALTIME, &boot);
}


timespec ts_diff(timespec current, timespec past)
{
  current.tv_sec  = current.tv_sec - past.tv_sec;
  current.tv_nsec = current.tv_nsec - past.tv_nsec;
  if (current.tv_nsec < 0)
  {
    current.tv_sec--;
    current.tv_nsec+=1000000000;
  }
  
  return current;

}

/* Arduino style time functions.
 */

time64_t micros()
{
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts = ts_diff(ts, boot);

  return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

time64_t millis()
{
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts = ts_diff(ts, boot);

  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

/* Arduino style delay functions.
 */

void delay(time64_t millis)
{
  timespec ts;
  ts.tv_sec = millis/1000;
  ts.tv_nsec = (millis % 1000) * 1000000;
  nanosleep(&ts, 0);
}

void delayMicroseconds(time64_t micros)
{
  timespec ts;
  ts.tv_sec = micros/1000000;
  ts.tv_nsec = (micros % 1000000) * 1000;
  nanosleep(&ts, 0);
}

