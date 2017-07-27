#include "can.h"

unsigned char CAN1_data[8];
unsigned char can1_rec_flag = 0;
unsigned char CAN2_data[8];
unsigned char can2_rec_flag = 0;


//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u8* msg, u8 len)
{
	u8 mbox;
	u16 i = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x12;	 // 标准标识符为0
	TxMessage.ExtId = 0x12;	 // 设置扩展标示符（29位）
	TxMessage.IDE = CAN_ID_STD;		  // 使用扩展标识符
	TxMessage.RTR = 0;		  // 消息类型为数据帧，一帧8位
	TxMessage.DLC = len;							 // 发送两帧信息
	for (i = 0; i < len; i++)
	{
		TxMessage.Data[i] = msg[i];				 // 第一帧信息   
	}

	mbox = CAN_Transmit(CAN1, &TxMessage);
	i = 0;
	while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))i++;	//等待发送结束
	if (i >= 0XFFF)return 1;
	return 0;

}

void canMsgTx(u8 module, u8 Data1, u8 Data2, u8 Data3, u8 Data4)
{
	CanTxMsg TxMessage;
	if (module == 0x00)
	{
		TxMessage.ExtId = 0x181f02f5;	//标准标识符为0x00
	}
	else if (module == 0x02)
	{
		TxMessage.ExtId = 0x181f22f5;	//标准标识符为0x00
	}
	TxMessage.IDE = CAN_ID_EXT;//使用标准标识符
	TxMessage.RTR = CAN_RTR_DATA;//为数据帧
	TxMessage.DLC = 4;	//	消息的数据长度为2个字节
	TxMessage.Data[0] = Data1; //第一个字节数据
	TxMessage.Data[1] = Data2; //第二个字节数据 
	TxMessage.Data[2] = Data3; //第三个字节数据
	TxMessage.Data[3] = Data4; //第四个字节数据
	CAN_Transmit(CAN1, &TxMessage); //发送数据
}


//格式： 波特率  CAN_SJW 	CAN_BS1  CAN_BS2 CAN_Prescaler 参考CAN_Baud_Process(unsigned int Baud,CAN_InitTypeDef *CAN_InitStructure)
const unsigned int CAN_baud_table[CAN_BAUD_NUM][5] =
{
	//波特率， CAN_SJW，   CAN_BS1，    CAN_BS2，CAN_Prescaler 
	{ 5,   CAN_SJW_1tq, CAN_BS1_13tq,CAN_BS2_2tq,450 },		//未通			
	{ 10,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_2tq, 400 },		//未通			
	{ 15,  CAN_SJW_1tq, CAN_BS1_13tq,CAN_BS2_2tq,150 },		//15K  未通
	{ 20,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_2tq,200 },		//20k //未通
	{ 25,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq,112 },		//25k  未通
	{ 40,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_2tq,100 },		//40k  未通
	{ 50,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq,56 },			//50k	ok
	{ 62,  CAN_SJW_1tq, CAN_BS1_13tq,CAN_BS2_2tq,36 },			//62.5k
	{ 80,  CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_2tq,50 },			//80k   未通
	{ 100, CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq,28 },			//100K	ok
	{ 125, CAN_SJW_1tq, CAN_BS1_13tq,CAN_BS2_2tq,18 },		//125K 未通
	{ 200, CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq,14 },			//200K  ok
	{ 250, CAN_SJW_1tq, CAN_BS1_15tq,CAN_BS2_5tq,8 },		    //250k  ok
	{ 400, CAN_SJW_1tq, CAN_BS1_15tq,CAN_BS2_5tq,5 },			//400K  ok
	{ 500, CAN_SJW_1tq, CAN_BS1_15tq,CAN_BS2_5tq,4 },			//500K	ok
	{ 666, CAN_SJW_1tq, CAN_BS1_5tq, CAN_BS2_2tq,8 },			//未通
	{ 800, CAN_SJW_1tq, CAN_BS1_8tq, CAN_BS2_3tq,14 },			//800K 未通
	{ 1000,CAN_SJW_1tq,	CAN_BS1_15tq,CAN_BS2_5tq,2 },			//1000K	ok
};
void CAN1_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	//使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 
	//初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12

	  //引脚复用映射配置
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); //GPIOA11复用为CAN1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); //GPIOA12复用为CAN1

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

	CAN_FilterInitStructure.CAN_FilterNumber = 0;	   //CAN1滤波器号从0到13

	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	   //滤波屏蔽模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;	//不屏蔽任何ID
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;		//不屏蔽任何ID
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;	  // /*!< Specifies the FIFO (0 or 1) which will be assigned to the filter.
															  //This parameter can be a value of @ref CAN_filter_FIFO */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//FIFO0消息挂号中断允许.		    
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/***********************************************************************
函数名称：CAN_Configuration(void)
功    能：完成can的配置
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);//用can2时，can1时钟也要开启
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
	CAN_FilterInitStructure.CAN_FilterNumber = 14;	   //CAN2滤波器号从14到27
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	   //滤波屏蔽模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;	//不屏蔽任何ID
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;		//不屏蔽任何ID
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;	  // /*!< Specifies the FIFO (0 or 1) which will be assigned to the filter.
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}
/***********************************************************************
函数名称：CCAN_Baud_Process(unsigned int Baud,CAN_InitTypeDef *CAN_InitStructure)
功    能：计算波特率，返回
编写时间：2013.4.25
编 写 人：
注    意：CAN时钟为42M
CAN_SJW : CAN_SJW_1tq - CAN_SJW_4tq	  不能比任何一相位缓冲段长
CAN_BS1 : CAN_BS1_1tq - CAN_BS1_16tq
CAN_BS2 : CAN_BS2_1tq - CAN_BS2_8tq
CAN_Prescaler : 1 - 1024
配置说明：
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
函数名称：CAN1_RX0_IRQHandler(void)
功    能：CAN1中断函数
输入参数：
输出参数：CAN1_data[8]
编写时间：
编 写 人：
注    意：
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
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);  /* 清除挂起中断 */
}
/***********************************************************************
函数名称：CAN1_WriteData(void)
功    能：CAN1发送函数
输入参数：ID和CAN1_data
输出参数：
编写时间：
编 写 人：
注    意： 扩展数据帧，8字节长度，ID
***********************************************************************/
void CAN1_WriteData(unsigned int ID)
{

	unsigned char i;
	uint8_t transmit_mailbox = 0;
	CanTxMsg TxMessage;

	TxMessage.ExtId = ID;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA; /* 设置为数据帧 */
	TxMessage.DLC = 8;            /* 数据长度, can报文规定最大的数据长度为8字节 */
	for (i = 0; i < 8; i++)
	{
		TxMessage.Data[i] = CAN1_data[i];
	}
	transmit_mailbox = CAN_Transmit(CAN1, &TxMessage);  /* 返回这个信息请求发送的邮箱号0,1,2或没有邮箱申请发送no_box */
	while ((CAN_TransmitStatus(CAN1, transmit_mailbox) != CANTXOK) && (i != 0xFFFF))
	{
		i++;
	}
}
/***********************************************************************
函数名称：CAN2_RX0_IRQHandler(void)
功    能：CAN2中断函数
输入参数：
输出参数：CAN2_data[8]
编写时间：
编 写 人：
注    意：
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
	CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);  /* 清除挂起中断 */
}
/***********************************************************************
函数名称：CAN2_WriteData(void)
功    能：CAN2发送函数
输入参数：ID和CAN2_data
输出参数：
编写时间：
编 写 人：
注    意： 扩展数据帧，8字节长度，ID
***********************************************************************/
void CAN2_WriteData(unsigned int ID)
{

	unsigned char i;
	uint8_t transmit_mailbox = 0;
	CanTxMsg TxMessage;

	TxMessage.ExtId = ID;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA; /* 设置为数据帧 */
	TxMessage.DLC = 8;            /* 数据长度, can报文规定最大的数据长度为8字节 */
	for (i = 0; i < 8; i++)
	{
		TxMessage.Data[i] = CAN2_data[i];
	}
	transmit_mailbox = CAN_Transmit(CAN2, &TxMessage);  /* 返回这个信息请求发送的邮箱号0,1,2或没有邮箱申请发送no_box */
	while ((CAN_TransmitStatus(CAN2, transmit_mailbox) != CANTXOK) && (i != 0xFFFF))
	{
		i++;
	}
}











