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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����4
	delay_init(168);		//��ʼ����ʱ����
	uart_init(115200);     	//��ʼ������
	batteryLevelLedInit();
    CAN1_Configuration();
	MYDMA_Config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)SendBuff, SEND_BUF_SIZE);
	TIM14_PWM_Init(500 - 1, 84 - 1);       	//84M/84=1Mhz�ļ���Ƶ�ʼ�����500,PWMƵ��Ϊ1M/500=2Khz     
	TIM5CH1Cap_Init(0XFFFFFFFF, 84 - 1); //��1Mhz��Ƶ�ʼ��� 
    TIM3_Int_Init(84 - 1, 0); //TIM3��1MHZ�����ж�
	ADC_Configuration();
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
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //�����ٽ���
	//����LED0����
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

	//����LED1����
	xTaskCreate((TaskFunction_t)led1_task,
		(const char*)"led1_task",
		(uint16_t)LED1_STK_SIZE,
		(void*)NULL,
		(UBaseType_t)LED1_TASK_PRIO,
		(TaskHandle_t*)&LED1Task_Handler);
	//�����������
	xTaskCreate((TaskFunction_t)float_task,
		(const char*)"float_task",
		(uint16_t)FLOAT_STK_SIZE,
		(void*)NULL,
		(UBaseType_t)FLOAT_TASK_PRIO,
		(TaskHandle_t*)&FLOATTask_Handler);
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
		vTaskDelay(1000);
	}
}

//LED1������
void led1_task(void *pvParameters)
{
	taskENTER_CRITICAL();
	setFlipTimer(10);
	taskEXIT_CRITICAL();
	vTaskDelete(LED1Task_Handler);
}

//�����������
void float_task(void *pvParameters)
{
	static float float_num = 0.00;
	while (1)
	{
        LED0 = !LED0;
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
