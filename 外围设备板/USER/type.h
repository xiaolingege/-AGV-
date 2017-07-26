#ifndef _TYPE_H
#define _TYPE_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"


typedef enum _LED_SHOW_TYPE
{
	Running = 0,//��������
	Chargering = 1,//��ǰ���ڳ��
	IDLE = 2,//����������
	Lost_3288 = 3,//3288���ƿ���ʧͨ��
	Lost_Ctrl_Card = 4,//�˶����ƿ���ʧͨ��
	Low_Power =5,//������
	Unknow = 0xff//δ֪����
}LED_SHOW_TYPE;

typedef enum _BATTERY_LEVEL_TYPE
{
	empty = 0,
	Low_Level = 1,
	Half_Level = 2,
	High_Level = 3,
	Full = 4
}BATTERY_LEVEL_TYPE;

#endif // !_TYPE_H
