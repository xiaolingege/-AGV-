#ifndef _ADC_H
#define _ADC_H
#include "stm32f4xx_adc.h"
#include "stm32f4xx.h"

typedef struct _ADC_VALUE_STRUCT_TYPE
{
	float batteryPower;
	float batteryCharger;
	float infrared;
}ADC_VALUE_STRUCT_TYPE;

void adcConfig(void);

ADC_VALUE_STRUCT_TYPE checkADCBufs(void);
#endif // !_ADC_H
