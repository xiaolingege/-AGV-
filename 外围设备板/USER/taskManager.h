#ifndef _TASK_MANAGER_H
#define _TASK_MANAGER_H
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
static void start_task(void *pvParameters);

//任务优先级
#define _INDICATOR_MANAGE_TASK_PRIO		2
//任务堆栈大小	
#define _INDICATOR_MANAGE_TASK_STK 		128 
//任务句柄
TaskHandle_t IndicatorTaskHandle;
//任务函数
void indicatorManageTask(void *pvParameters);

//任务优先级
#define SENSOR_CHECK_TASK_PRIO		3
//任务堆栈大小	
#define SENSOR_CHECK_STK_SIZE 		128
//任务句柄
TaskHandle_t SensorCheckTaskHandler;
//任务函数
void sensorCheckTask(void *pvParameters);

//任务优先级
#define COMMUNICATE_TASK_PRIO		4
//任务堆栈大小	
#define COMMUNICATE_STK_SIZE 		128
//任务句柄
TaskHandle_t CommunicateTaskHandler;
//任务函数
void communicateTask(void *pvParameters);


#define _LIDAR_TASK_PRIO	5
#define _LIDAR_TASK_STK		512
TaskHandle_t LidarTaskHandle;
void lidarTask(void *pvParameters);

#define _CONFIG_ARGS_TASK_PRIO 6
#define _CONFIG_ARGS_TASK_STK 128
TaskHandle_t ConfigArgsTaskHandle;
void configArgTask(void *pvParameters);


#endif // !_TASK_MANAGER_H
