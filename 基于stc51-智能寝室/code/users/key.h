#ifndef	__KEY__H_
#define	__KEY__H_

#include "reg52.h"

sbit KEY1 = P2^0;
sbit KEY2 = P2^1;
sbit KEY3 = P2^2;
sbit KEY4 = P2^3;

unsigned char KeyScan(void);


#endif	/* __KEY__H_ */