#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "type.h"
#include "ledCTRL.h"
#include "flash.h"
#include "dma.h"
#include "can.h"
#include "timer.h"
#include "adc.h"

#define _ENABLE_LIDAR 0

#define SEND_BUF_SIZE 10
u8 SendBuff[SEND_BUF_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

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

xQueueHandle ConfigArgsQueue;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数
	uart_init(115200);     	//初始化串口
	batteryLevelLedInit();
    CAN1_Configuration();
	MYDMA_Config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)SendBuff, SEND_BUF_SIZE);
	TIM14_PWM_Init(500 - 1, 84 - 1);       	//84M/84=1Mhz的计数频率计数到500,PWM频率为1M/500=2Khz     
	TIM5CH1Cap_Init(0XFFFFFFFF, 84 - 1); //以1Mhz的频率计数 
    TIM3_Int_Init(84 - 1, 0); //TIM3以1MHZ产生中断
	ADC_Configuration();
	//创建开始任务
	xTaskCreate((TaskFunction_t)start_task,            //任务函数
		(const char*)"start_task",          //任务名称
		(uint16_t)START_STK_SIZE,        //任务堆栈大小
		(void*)NULL,                  //传递给任务函数的参数
		(UBaseType_t)START_TASK_PRIO,       //任务优先级
		(TaskHandle_t*)&StartTask_Handler);   //任务句柄              
	vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //进入临界区
	//创建LED0任务
	xTaskCreate((TaskFunction_t)indicatorManageTask,
		(const char*)"IndicatorManageTask",
		(uint16_t)_INDICATOR_MANAGE_TASK_STK,
		(void*)NULL,
		(UBaseType_t)_INDICATOR_MANAGE_TASK_PRIO,
		(TaskHandle_t*)&IndicatorTaskHandle);
#if _ENABLE_LIDAR
	xTaskCreate((TaskFunction_t)lidarTask,
		(const char*)"LidarTask",
		(uint16_t)_LIDAR_TASK_STK,
		(void*)NULL,
		(UBaseType_t)_LIDAR_TASK_PRIO,
		(TaskHandle_t*)&LidarTaskHandle);
#endif

	//创建LED1任务
	xTaskCreate((TaskFunction_t)led1_task,
		(const char*)"led1_task",
		(uint16_t)LED1_STK_SIZE,
		(void*)NULL,
		(UBaseType_t)LED1_TASK_PRIO,
		(TaskHandle_t*)&LED1Task_Handler);
	//浮点测试任务
	xTaskCreate((TaskFunction_t)float_task,
		(const char*)"float_task",
		(uint16_t)FLOAT_STK_SIZE,
		(void*)NULL,
		(UBaseType_t)FLOAT_TASK_PRIO,
		(TaskHandle_t*)&FLOATTask_Handler);
	vTaskDelete(StartTask_Handler); //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区
}

//************************************
// FunctionName:  indicatorManageTask
// Returns:   void
// Qualifier:驱动电量显示LED与景观LED
// Parameter: void * pvParameters
//************************************
void indicatorManageTask(void *pvParameters)
{
	while (1)
	{
		errStatusShow(Lost_3288);
		batteryLevelShow(Full);
		vTaskDelay(1000);
	}
}

//LED1任务函数
void led1_task(void *pvParameters)
{
	while (1)
	{
		//LED1 = 1;
		vTaskDelay(200);
	//	LED1 = 0;
		vTaskDelay(800);
	}
}

//浮点测试任务
void float_task(void *pvParameters)
{
	static float float_num = 0.00;
	while (1)
	{
		float_num += 0.01f;
		vTaskDelay(1000);
	}
}

void lidarTask(void *pvParameters)
{
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

	while (1)
	{
		MYDMA_Enable(DMA2_Stream7, SEND_BUF_SIZE);
		while (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET)
		{
			vTaskDelay(2);
		}//等待DMA2_Steam7传输完成
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志

		vTaskDelay(100);
	}
}

void configArgTask(void *pvParameters)
{
	s32 writeBuf[10] = { 1,2,3,4,5,6,7,8,9,0 };
	s32 readBuf[10] = { 0,0,0,0,0,0,0,0,0,0 };
	taskENTER_CRITICAL();
	Flash_Write32BitDatas(FLASH_USER_START_ADDR, 10, writeBuf);
	Flash_Read32BitDatas(FLASH_USER_START_ADDR, 10, readBuf);
	vTaskDelete(ConfigArgsTaskHandle);
	taskEXIT_CRITICAL();
}
