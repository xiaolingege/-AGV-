#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"
#include "usart.h"

extern volatile unsigned char RS232_dma_send_flag;

void RS232_DMA_Init(void);
void DMA_RS232_Send_Configuration(unsigned char *send_buff,unsigned int send_size);	   
#endif






























