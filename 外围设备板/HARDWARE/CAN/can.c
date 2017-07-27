#include "can.h"

unsigned char CAN1_data[8];
unsigned char can1_rec_flag = 0;
unsigned char CAN2_data[8];
unsigned char can2_rec_flag = 0;


//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg, u8 len)
{
	u8 mbox;
	u16 i = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x12;	 // ��׼��ʶ��Ϊ0
	TxMessage.ExtId = 0x12;	 // ������չ��ʾ����29λ��
	TxMessage.IDE = CAN_ID_STD;		  // ʹ����չ��ʶ��
	TxMessage.RTR = 0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
	TxMessage.DLC = len;							 // ������֡��Ϣ
	for (i = 0; i < len; i++)
	{
		TxMessage.Data[i] = msg[i];				 // ��һ֡��Ϣ   
	}

	mbox = CAN_Transmit(CAN1, &TxMessage);
	i = 0;
	while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))i++;	//�ȴ����ͽ���
	if (i >= 0XFFF)return 1;
	return 0;

}

void canMsgTx(u8 module, u8 Data1, u8 Data2, u8 Data3, u8 Data4)
{
	CanTxMsg TxMessage;
	if (module == 0x00)
	{
		TxMessage.ExtId = 0x181f02f5;	//��׼��ʶ��Ϊ0x00
	}
	else if (module == 0x02)
	{
		TxMessage.ExtId = 0x181f22f5;	//��׼��ʶ��Ϊ0x00
	}
	TxMessage.IDE = CAN_ID_EXT;//ʹ�ñ�׼��ʶ��
	TxMessage.RTR = CAN_RTR_DATA;//Ϊ����֡
	TxMessage.DLC = 4;	//	��Ϣ�����ݳ���Ϊ2���ֽ�
	TxMessage.Data[0] = Data1; //��һ���ֽ�����
	TxMessage.Data[1] = Data2; //�ڶ����ֽ����� 
	TxMessage.Data[2] = Data3; //�������ֽ�����
	TxMessage.Data[3] = Data4; //���ĸ��ֽ�����
	CAN_Transmit(CAN1, &TxMessage); //��������
}


//��ʽ�� ������  CAN_SJW 	CAN_BS1  CAN_BS2 CAN_Prescaler �ο�CAN_Baud_Process(unsigned int Baud,CAN_InitTypeDef *CAN_InitStructure)
const unsigned int CAN_baud_table[CAN_BAUD_NUM][5] =
{
	//�����ʣ� CAN_SJW��   CAN_BS1��    CAN_BS2��CAN_Prescaler 
	{ 5,   CAN_SJW_1tq, CAN_BS1_13tq,CAN_BS2_2tq,450 },		//δͨ			
	{ 10,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_2tq, 400 },		//δͨ			
	{ 15,  CAN_SJW_1tq, CAN_BS1_13tq,CAN_BS2_2tq,150 },		//15K  δͨ
	{ 20,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_2tq,200 },		//20k //δͨ
	{ 25,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq,112 },		//25k  δͨ
	{ 40,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_2tq,100 },		//40k  δͨ
	{ 50,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq,56 },			//50k	ok
	{ 62,  CAN_SJW_1tq, CAN_BS1_13tq,CAN_BS2_2tq,36 },			//62.5k
	{ 80,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_2tq,50 },			//80k   δͨ
	{ 100, CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq,28 },			//100K	ok
	{ 125, CAN_SJW_1tq, CAN_BS1_13tq,CAN_BS2_2tq,18 },		//125K δͨ
	{ 200, CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq,14 },			//200K  ok
	{ 250, CAN_SJW_1tq, CAN_BS1_15tq,CAN_BS2_5tq,8 },		    //250k  ok
	{ 400, CAN_SJW_1tq, CAN_BS1_15tq,CAN_BS2_5tq,5 },			//400K  ok
	{ 500, CAN_SJW_1tq, CAN_BS1_15tq,CAN_BS2_5tq,4 },			//500K	ok
	{ 666, CAN_SJW_1tq, CAN_BS1_5tq, CAN_BS2_2tq,8 },			//δͨ
	{ 800, CAN_SJW_1tq, CAN_BS1_8tq, CAN_BS2_3tq,14 },			//800K δͨ
	{ 1000,CAN_SJW_1tq,	CAN_BS1_15tq,CAN_BS2_5tq,2 },			//1000K	ok
};
void CAN1_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	//ʹ�����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 
	//��ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12

	  //���Ÿ���ӳ������
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); //GPIOA12����ΪCAN1

	/* CAN configuration ********************************************************/
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	CAN_Baud_Process(500, &CAN_InitStructure);
	CAN_Init(CAN1, &CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 0;	   //CAN1�˲����Ŵ�0��13

	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	   //�˲�����ģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;	//�������κ�ID
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;		//�������κ�ID
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;	  // /*!< Specifies the FIFO (0 or 1) which will be assigned to the filter.
															  //This parameter can be a value of @ref CAN_filter_FIFO */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/***********************************************************************
�������ƣ�CAN_Configuration(void)
��    �ܣ����can������
���������
���������
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
void CAN2_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	/* CAN GPIOs configuration **************************************************/

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
	/* CAN configuration ********************************************************/
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);//��can2ʱ��can1ʱ��ҲҪ����
	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_Baud_Process(500, &CAN_InitStructure);
	CAN_Init(CAN2, &CAN_InitStructure);
	CAN_FilterInitStructure.CAN_FilterNumber = 14;	   //CAN2�˲����Ŵ�14��27
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	   //�˲�����ģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;	//�������κ�ID
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;		//�������κ�ID
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;	  // /*!< Specifies the FIFO (0 or 1) which will be assigned to the filter.
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}
/***********************************************************************
�������ƣ�CCAN_Baud_Process(unsigned int Baud,CAN_InitTypeDef *CAN_InitStructure)
��    �ܣ����㲨���ʣ�����
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺CANʱ��Ϊ42M
CAN_SJW : CAN_SJW_1tq - CAN_SJW_4tq	  ���ܱ��κ�һ��λ����γ�
CAN_BS1 : CAN_BS1_1tq - CAN_BS1_16tq
CAN_BS2 : CAN_BS2_1tq - CAN_BS2_8tq
CAN_Prescaler : 1 - 1024
����˵����
CAN_SJW + CAN_BS1 / (CAN_SJW + CAN_BS1 + CAN_BS2)
0.75     baud > 800k
0.80     baud > 500k
0.875    baud <= 500k
baud = 42M / (CAN_SJW + CAN_BS1 + CAN_BS2) / CAN_Prescaler
***********************************************************************/
void CAN_Baud_Process(unsigned int Baud, CAN_InitTypeDef *CAN_InitStructure)
{
	unsigned int i = 0;
	for (i = 0; i < CAN_BAUD_NUM; i++)
	{
		if (Baud == CAN_baud_table[i][0])
		{
			CAN_InitStructure->CAN_SJW = CAN_baud_table[i][1];
			CAN_InitStructure->CAN_BS1 = CAN_baud_table[i][2];
			CAN_InitStructure->CAN_BS2 = CAN_baud_table[i][3];
			CAN_InitStructure->CAN_Prescaler = CAN_baud_table[i][4];
			return;
		}
	}
}
/***********************************************************************
�������ƣ�CAN1_RX0_IRQHandler(void)
��    �ܣ�CAN1�жϺ���
���������
���������CAN1_data[8]
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	unsigned int i = 0;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	for (i = 0; i < 8; i++)
	{
		CAN1_data[i] = RxMessage.Data[i];
	}
	can1_rec_flag = 1;
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);  /* ��������ж� */
}
/***********************************************************************
�������ƣ�CAN1_WriteData(void)
��    �ܣ�CAN1���ͺ���
���������ID��CAN1_data
���������
��дʱ�䣺
�� д �ˣ�
ע    �⣺ ��չ����֡��8�ֽڳ��ȣ�ID
***********************************************************************/
void CAN1_WriteData(unsigned int ID)
{

	unsigned char i;
	uint8_t transmit_mailbox = 0;
	CanTxMsg TxMessage;

	TxMessage.ExtId = ID;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA; /* ����Ϊ����֡ */
	TxMessage.DLC = 8;            /* ���ݳ���, can���Ĺ涨�������ݳ���Ϊ8�ֽ� */
	for (i = 0; i < 8; i++)
	{
		TxMessage.Data[i] = CAN1_data[i];
	}
	transmit_mailbox = CAN_Transmit(CAN1, &TxMessage);  /* ���������Ϣ�����͵������0,1,2��û���������뷢��no_box */
	while ((CAN_TransmitStatus(CAN1, transmit_mailbox) != CANTXOK) && (i != 0xFFFF))
	{
		i++;
	}
}
/***********************************************************************
�������ƣ�CAN2_RX0_IRQHandler(void)
��    �ܣ�CAN2�жϺ���
���������
���������CAN2_data[8]
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	unsigned int i = 0;
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
	for (i = 0; i < 8; i++)
	{
		CAN2_data[i] = RxMessage.Data[i];
	}
	can2_rec_flag = 1;
	CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);  /* ��������ж� */
}
/***********************************************************************
�������ƣ�CAN2_WriteData(void)
��    �ܣ�CAN2���ͺ���
���������ID��CAN2_data
���������
��дʱ�䣺
�� д �ˣ�
ע    �⣺ ��չ����֡��8�ֽڳ��ȣ�ID
***********************************************************************/
void CAN2_WriteData(unsigned int ID)
{

	unsigned char i;
	uint8_t transmit_mailbox = 0;
	CanTxMsg TxMessage;

	TxMessage.ExtId = ID;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA; /* ����Ϊ����֡ */
	TxMessage.DLC = 8;            /* ���ݳ���, can���Ĺ涨�������ݳ���Ϊ8�ֽ� */
	for (i = 0; i < 8; i++)
	{
		TxMessage.Data[i] = CAN2_data[i];
	}
	transmit_mailbox = CAN_Transmit(CAN2, &TxMessage);  /* ���������Ϣ�����͵������0,1,2��û���������뷢��no_box */
	while ((CAN_TransmitStatus(CAN2, transmit_mailbox) != CANTXOK) && (i != 0xFFFF))
	{
		i++;
	}
}











