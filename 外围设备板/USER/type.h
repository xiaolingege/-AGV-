#ifndef _TYPE_H
#define _TYPE_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"


typedef enum _LED_SHOW_TYPE
{
	Running = 0,//良好运行
	Chargering = 1,//当前正在充电
	IDLE = 2,//空闲无任务
	Lost_3288 = 3,//3288控制卡丢失通信
	Lost_Ctrl_Card = 4,//运动控制卡丢失通信
	Low_Power =5,//电量低
	Unknow = 0xff//未知错误
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
