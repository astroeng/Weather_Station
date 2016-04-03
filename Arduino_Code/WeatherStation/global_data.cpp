/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include <primitive_scheduler.h>
#include <numerical_statistics.h>

#include "tcp_client.h"
#include "global_data.h"

IPAddress baseStation(192,168,23,72);

/* Setup for the logging task */
TCP_Client sparkfun_logger(baseStation, 9876);

/* Declare a scheduler */
PrimitiveScheduler schedule(NUMBER_OF_TASKS);

/* Some system statistics classes. The task run time for the core tasks is recorded and
 * uploaded to it's own datastream independent of the weather data stream.
 */
Numerical_Statistics taskRunTime[task_name_length];
