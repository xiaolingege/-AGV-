#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#define _FLIP_FREQUENCY 30l
#define _FLIP_COUNT (500000l)/_FLIP_FREQUENCY
void TIM14_PWM_Init(u32 arr,u32 psc);
void TIM5CH1Cap_Init(u32 arr,u16 psc);

void TIM3_Int_Init(u16 arr, u16 psc);
void TIM3_IRQHandler(void);
extern void setFlipTimer(u32 frequency);
#endif























