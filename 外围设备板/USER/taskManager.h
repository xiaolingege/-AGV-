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
void start_task(void *pvParameters);

//任务优先级
#define _INDICATOR_MANAGE_TASK_PRIO		2
//任务堆栈大小	
#define _INDICATOR_MANAGE_TASK_STK 		50  
//任务句柄
TaskHandle_t IndicatorTaskHandle;
//任务函数
void indicatorManageTask(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define FLOAT_TASK_PRIO		4
//任务堆栈大小	
#define FLOAT_STK_SIZE 		128
//任务句柄
TaskHandle_t FLOATTask_Handler;
//任务函数
void float_task(void *pvParameters);


#define _LIDAR_TASK_PRIO	5
#define _LIDAR_TASK_STK		512
TaskHandle_t LidarTaskHandle;
void lidarTask(void *pvParameters);

#define _CONFIG_ARGS_TASK_PRIO 6
#define _CONFIG_ARGS_TASK_STK 128
TaskHandle_t ConfigArgsTaskHandle;
void configArgTask(void *pvParameters);


#endif // !_TASK_MANAGER_H
