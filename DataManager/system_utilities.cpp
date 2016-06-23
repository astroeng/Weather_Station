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
#include <cstring>
#include <iostream>
timespec boot;
volatile int serverRunning;

struct tm previous_tm;

void time_init()
{
  // Prime the boot structure with the current time.
  clock_gettime(CLOCK_REALTIME, &boot);
  
  // Prime the function that detects crossing midnight.
  crossedMidnight();
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

char* timeDateString(char* buffer, int buffer_len)
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, buffer_len, "%FT%T.000%Z",timeinfo);

  return buffer;
}

bool crossedMidnight()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime(&rawtime);

  std::cout << "midnight: " << previous_tm.tm_mday << " " << timeinfo->tm_mday << std::endl;

  if (previous_tm.tm_mday != timeinfo->tm_mday)
  {
    memcpy(&previous_tm, timeinfo, sizeof(previous_tm));
    return true;
  }

  memcpy(&previous_tm, timeinfo, sizeof(previous_tm));
  return false;
}

