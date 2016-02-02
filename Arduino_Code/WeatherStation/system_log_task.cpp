/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */
#include "sparkfun_log.h"

#include "system_log_task.h"
#include "global_data.h"
#include "discrete_task.h"

unsigned long systemMessageCount  = 0;

/* Function to populate logString with the system data. */

void buildSystemString()
{
  systemMessageCount++;

  /* The entries in dataArray must match the order of the entries in
   * systemStrings defined in sparkfun_log.h
   */

  long dataArray[] = {taskRunTime[task_discrete_read].getMean(),
                      taskRunTime[task_discrete_read].getMax(),
                      taskRunTime[task_digital_read].getMean(),
                      taskRunTime[task_digital_read].getMax(),
                      taskRunTime[task_weather_log].getMean(),
                      taskRunTime[task_weather_log].getMax(),
                      taskRunTime[task_system_log].getMean(),
                      taskRunTime[task_system_log].getMax(),
                      getDiscreteData()->batteryVoltage.getMean(),
                      millis()/1000,
                      systemMessageCount};

  createLoggingString(systemPublicKey, systemPrivateKey,
                      dataArray, systemStrings, 11);

}


/* Perform the steps to provide the remote server with system data. */

void logSystem()
{
  buildSystemString();

  sendLoggingString(&sparkfun_logger);

  getDiscreteData()->batteryVoltage.reset();

  taskRunTime[task_system_log].includeValue(schedule.getTaskExecutionTime(task_system_log));
}
