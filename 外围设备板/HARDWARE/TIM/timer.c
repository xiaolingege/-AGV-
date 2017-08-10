#include "timer.h"
#include "led.h"

//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM4_PWM_Init(u32 arr, u32 psc)
{
	//�˲������ֶ��޸�IO������

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  	//TIM14ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//ʹ��PORTFʱ��	

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //GPIOA9 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned3;
	TIM_TimeBaseStructure.TIM_Period = arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���

		//��ʼ��TIM14 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���


	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM14		
	TIM_SetCompare1(TIM4, 75);
	TIM_SetCompare2(TIM4, 25);
}
void changeStrobe(u32 frequency, u32 exposure, u32 shoot)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	u32 arr = 500000 / frequency;
	TIM_Cmd(TIM4, DISABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned3; 
	TIM_TimeBaseStructure.TIM_Period = arr - 1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM4, ENABLE);
	TIM_SetCompare1(TIM4, exposure / 2);
	TIM_SetCompare2(TIM4, (exposure - 2 * shoot) / 2);
}

//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void TIM5CH1Cap_Init(u32 arr, u16 psc)
{
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA0

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //PA0����λ��ʱ��5
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_Cmd(TIM5, ENABLE); 	//ʹ�ܶ�ʱ��5

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}
//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM5CH1_CAPTURE_STA = 0;	//���벶��״̬		    				
u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{
	if ((TIM5CH1_CAPTURE_STA & 0X80) == 0)//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
		{
			if (TIM5CH1_CAPTURE_STA & 0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if ((TIM5CH1_CAPTURE_STA & 0X3F) == 0X3F)//�ߵ�ƽ̫����,���������
				{
					TIM5CH1_CAPTURE_STA |= 0X80;		//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL = 0XFFFFFFFF;
				}
				else TIM5CH1_CAPTURE_STA++;
			}
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
		{
			if (TIM5CH1_CAPTURE_STA & 0X40)		//����һ���½��� 		
			{
				TIM5CH1_CAPTURE_STA |= 0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM5CH1_CAPTURE_VAL = TIM_GetCapture1(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA = 0;			//���
				TIM5CH1_CAPTURE_VAL = 0;
				TIM5CH1_CAPTURE_STA |= 0X40;		//��ǲ�����������
				TIM_Cmd(TIM5, DISABLE); 	//�رն�ʱ��5
				TIM_SetCounter(TIM5, 0);
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM5, ENABLE); 	//ʹ�ܶ�ʱ��5
			}
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_Update); //����жϱ�־λ
}
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  ///ʹ��TIM3ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);//��ʼ��TIM3

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3, DISABLE); //ʹ�ܶ�ʱ��3

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

static struct _FLIP_ARGS
{
	u32 FlipCount;
	u32 FlipFrequency;
}FlipArgs;

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	static u32 countTimer3 = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //����ж�
	{
		if (!countTimer3)
		{
			LED1 = !LED1;//DS1��ת
			countTimer3 = FlipArgs.FlipCount;
		}
		else
		{
			countTimer3--;
		}
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //����жϱ�־λ
}

extern void setFlipTimer(u32 frequency)
{
	if (frequency == 0)
	{
		TIM_Cmd(TIM3, DISABLE);
		return;
	}
	else
	{
		FlipArgs.FlipFrequency = frequency;
		FlipArgs.FlipCount = 500000l / FlipArgs.FlipFrequency;
		TIM_Cmd(TIM3, ENABLE);
	}
}
