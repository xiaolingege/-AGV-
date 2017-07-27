#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
	
#define CAN_BAUD_NUM	18

void CAN1_Configuration(void);

void CAN2_Configuration(void);
void CAN_Baud_Process(unsigned int Baud, CAN_InitTypeDef *CAN_InitStructure);
void CAN1_WriteData(unsigned int ID);
void CAN2_WriteData(unsigned int ID);
    
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
 
u8 CAN1_Send_Msg(u8* msg,u8 len);						//��������

u8 CAN1_Receive_Msg(u8 *buf);							//��������

void canMsgTx(u8 module, u8 Data1, u8 Data2, u8 Data3, u8 Data4);
#endif

















