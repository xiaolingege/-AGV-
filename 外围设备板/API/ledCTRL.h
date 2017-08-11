#ifndef _LED_CTRL_H
#define _LED_CTRL_H
#include "stm32f4xx.h"
#include "led.h"
#include "type.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

#define POWER_0 PFout(0)
#define POWER_25 PFout(1)
#define POWER_50 PFout(2)
#define POWER_75 PFout(3)
#define POWER_100 PFout(4)

#define TAPE_R PFout(10)
#define TAPE_G PFout(11)
#define TAPE_B PFout(12)


#define ERR_LED_PUSH PFout(5)

void errStatusShow(LED_SHOW_TYPE status);

void batteryLevelShow(BATTERY_LEVEL_TYPE battery);
void tapeLightShow(TAPE_LIGHT_TYPE status);
#endif // !_LED_CTRL_H


