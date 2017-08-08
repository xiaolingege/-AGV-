#ifndef _TASK_MANAGER_H
#define _TASK_MANAGER_H
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define _INDICATOR_MANAGE_TASK_PRIO		2
//�����ջ��С	
#define _INDICATOR_MANAGE_TASK_STK 		50  
//������
TaskHandle_t IndicatorTaskHandle;
//������
void indicatorManageTask(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO		3
//�����ջ��С	
#define LED1_STK_SIZE 		50  
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);

//�������ȼ�
#define FLOAT_TASK_PRIO		4
//�����ջ��С	
#define FLOAT_STK_SIZE 		128
//������
TaskHandle_t FLOATTask_Handler;
//������
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
