#ifndef _TYPE_H
#define _TYPE_H

#include "stm32f4xx.h"


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
	empty = 1,//电池电量低于25，判断为警报灯电量
	Low_Level = 2,//电池电量剩余25左右
	Half_Level = 4,//电池电量剩余50
	High_Level = 8,//电池电量剩余75
	Full = 16//满点
}BATTERY_LEVEL_TYPE;

typedef enum _TAPE_LIGHT_TYPE
{
	RED = 0x01,
	GREEN = 0x02,
	BLUE = 0x04
}TAPE_LIGHT_TYPE;

#endif // !_TYPE_H
