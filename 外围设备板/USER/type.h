#ifndef _TYPE_H
#define _TYPE_H

#include "stm32f4xx.h"


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
	empty = 1,//��ص�������25���ж�Ϊ�����Ƶ���
	Low_Level = 2,//��ص���ʣ��25����
	Half_Level = 4,//��ص���ʣ��50
	High_Level = 8,//��ص���ʣ��75
	Full = 16//����
}BATTERY_LEVEL_TYPE;

typedef enum _TAPE_LIGHT_TYPE
{
	RED = 0x01,
	GREEN = 0x02,
	BLUE = 0x04
}TAPE_LIGHT_TYPE;

#endif // !_TYPE_H
