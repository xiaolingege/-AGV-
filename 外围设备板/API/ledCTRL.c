#include "ledCTRL.h"
void errStatusShow(LED_SHOW_TYPE status)
{
	u8 i = 0;
	for (i = 0; i < status + 1; i++)
	{
		ERR_LED_PUSH = 0;
		vTaskDelay(200);
		ERR_LED_PUSH = 1;
		vTaskDelay(200);
	}
}


void batteryLevelShow(BATTERY_LEVEL_TYPE battery)
{

	switch (battery)
	{
	case empty:
		POWER_0 = 0;
		POWER_25 = 1;
		POWER_50 = 1;
		POWER_75 = 1;
		POWER_100 = 1;
		break;
	case Low_Level:
		POWER_0 = 1;
		POWER_25 = 0;
		POWER_50 = 1;
		POWER_75 = 1;
		POWER_100 = 1;
		break;
	case Half_Level:
		POWER_0 = 1;
		POWER_25 = 0;
		POWER_50 = 0;
		POWER_75 = 1;
		POWER_100 = 1;
		break;
	case High_Level:
		POWER_0 = 1;
		POWER_25 = 0;
		POWER_50 = 0;
		POWER_75 = 0;
		POWER_100 = 1;
		break;
	case Full:
		POWER_0 = 1;
		POWER_25 = 0;
		POWER_50 = 0;
		POWER_75 = 0;
		POWER_100 = 0;
		break;
	default:
		break;
	}

}

//************************************
// FunctionName:  tapeLightShow
// Returns:   void
// Qualifier: 通过给定灯条颜色参数输出到IO口
// Parameter: TAPE_LIGHT_TYPE status
//************************************
void tapeLightShow(TAPE_LIGHT_TYPE colour)
{
	TAPE_R = (colour & 0x01);
	TAPE_G = ((colour & 0x02) >> 1);
	TAPE_B = ((colour & 0x04) >> 2);
}



