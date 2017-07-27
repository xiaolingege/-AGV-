#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
	
#define CAN_BAUD_NUM	18

void CAN1_Configuration(void);

void CAN2_Configuration(void);
void CAN_Baud_Process(unsigned int Baud, CAN_InitTypeDef *CAN_InitStructure);
void CAN1_WriteData(unsigned int ID);
void CAN2_WriteData(unsigned int ID);
    
//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.								    
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
 
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据

u8 CAN1_Receive_Msg(u8 *buf);							//接收数据

void canMsgTx(u8 module, u8 Data1, u8 Data2, u8 Data3, u8 Data4);
#endif

















