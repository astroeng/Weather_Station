/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <primitive_scheduler.h>
#include <numerical_statistics.h>

#include "tcp_client.h"
#include "config.h"

/* Setup for the logging task */
extern TCP_Client sparkfun_logger;
/* Declare a scheduler */
extern PrimitiveScheduler schedule;

/* "Tasks" for data collection and logging */
typedef enum
{
  task_discrete_read = 0,
  task_digital_read,
  task_weather_log,
  task_system_log,
  task_name_length
} TaskNameEnumType;

/* Some system statistics classes. The task run time for the core tasks is recorded and
 * uploaded to it's own datastream independent of the weather data stream.
 */
extern Numerical_Statistics taskRunTime[task_name_length];

#endif
