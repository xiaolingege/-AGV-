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
#include "taskManager.h"

#define _ENABLE_LIDAR 0

#define SEND_BUF_SIZE 10
u8 SendBuff[SEND_BUF_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

xQueueHandle ConfigArgsQueue;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delayInit(168);		
	uartInit(115200);     	
	batteryLevelLedInit();
    can1Config();
	myDmaConfig(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)SendBuff, SEND_BUF_SIZE);
	tim4PwmInit(5000 - 1, 84 - 1);
	adcConfig();
	//������ʼ����
	xTaskCreate((TaskFunction_t)start_task,            //������
		(const char*)"start_task",          //��������
		(uint16_t)START_STK_SIZE,        //�����ջ��С
		(void*)NULL,                  //���ݸ��������Ĳ���
		(UBaseType_t)START_TASK_PRIO,       //�������ȼ�
		(TaskHandle_t*)&StartTask_Handler);   //������              
	vTaskStartScheduler();          //�����������
}

//��ʼ����������
static void start_task(void *pvParameters)
{
	taskENTER_CRITICAL(); 

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

	xTaskCreate((TaskFunction_t)sensorCheckTask,
		(const char*)"SensorCheck",
		(uint16_t)SENSOR_CHECK_STK_SIZE,
		(void*)NULL,
		(UBaseType_t)SENSOR_CHECK_TASK_PRIO,
		(TaskHandle_t*)&SensorCheckTaskHandler);

	xTaskCreate((TaskFunction_t)communicateTask,
		(const char*)"communicate_task",
		(uint16_t)COMMUNICATE_STK_SIZE,
		(void*)NULL,
		(UBaseType_t)COMMUNICATE_TASK_PRIO,
		(TaskHandle_t*)&CommunicateTaskHandler);

	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
	taskEXIT_CRITICAL();            //�˳��ٽ���
}

//************************************
// FunctionName:  indicatorManageTask
// Returns:   void
// Qualifier:����������ʾLED�뾰��LED
// Parameter: void * pvParameters
//************************************
void indicatorManageTask(void *pvParameters)
{
	while (1)
	{
		errStatusShow(Lost_3288);
		batteryLevelShow(Full);
		tapeLightShow(RED);
		vTaskDelay(500);
	}
}

//************************************
// FunctionName:  sensorCheck
// Returns:   void
// Qualifier:�����ص���������������⴫��������ֵ
// Parameter: void * pvParameters
//************************************
void sensorCheckTask(void *pvParameters)
{
	ADC_VALUE_STRUCT_TYPE checkValue;

	while (1)
	{
		checkValue = checkADCBufs();
		vTaskDelay(10);
	}
}

//************************************
// FunctionName:  communicateTask
// Returns:   void
// Qualifier:������ͨ�Ź��ܣ�������������������ִ��
// Parameter: void * pvParameters
//************************************
void communicateTask(void *pvParameters)
{
	while (1)
	{
		//������ͨ��
		changeStrobe(200, 150, 50);
        LED0 = !LED0;
		vTaskDelay(1000);
	}
}

//************************************
// FunctionName:  lidarTask
// Returns:   void
// Qualifier:���ϼ������ݶ�ȡ
// Parameter: void * pvParameters
//************************************
void lidarTask(void *pvParameters)
{
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

	while (1)
	{
		MYDMA_Enable(DMA2_Stream7, SEND_BUF_SIZE);
		while (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET)
		{
			vTaskDelay(2);
		}//�ȴ�DMA2_Steam7�������
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);//���DMA2_Steam7������ɱ�־
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
