#ifndef	__USART__H_
#define	__USART__H_

#include "reg52.h"

#define uchar unsigned char
#define uint unsigned int

void Gsm_proc(void);
void SerialInti();
void Uart1Send(uchar c);
void Uart1Sends(uchar *str);

//void Usart_Init(void);
//void Uart_SendChar(unsigned char  dat);

#endif	/* __USART__H_ */