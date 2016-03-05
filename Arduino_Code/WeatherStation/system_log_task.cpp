/* Derek Schacht
 *  2016 01 21
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */
#include "station_types.h"
#include "system_log_task.h"
#include "global_data.h"
#include "discrete_task.h"

unsigned long systemMessageCount  = 0;

/* Function to populate logString with the system data. */

void sendSystemLog()
{
  systemMessageCount++;

  StatusMessageType message;

  message.header.messageKind                = StatusMessage;
  message.data.messageCount                 = systemMessageCount;
  message.data.task1_average_execution_time = taskRunTime[task_discrete_read].getMean();
  message.data.task1_max_execution_time     = taskRunTime[task_discrete_read].getMax();
  message.data.task2_average_execution_time = taskRunTime[task_digital_read].getMean();
  message.data.task2_max_execution_time     = taskRunTime[task_digital_read].getMax();
  message.data.task3_average_execution_time = taskRunTime[task_weather_log].getMean();
  message.data.task3_max_execution_time     = taskRunTime[task_weather_log].getMax();
  message.data.task4_average_execution_time = taskRunTime[task_system_log].getMean();
  message.data.task4_max_execution_time     = taskRunTime[task_system_log].getMax();
  message.data.batteryVoltage               = getDiscreteData()->batteryVoltage.getMean();
  message.data.uptime                       = millis()/1000;

  sparkfun_logger.sendData((char*)&message, sizeof(message));

}


/* Perform the steps to provide the remote server with system data. */

void logSystem()
{
  sendSystemLog();

  getDiscreteData()->batteryVoltage.reset();

  taskRunTime[task_system_log].includeValue(schedule.getTaskExecutionTime(task_system_log));
}
