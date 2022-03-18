#ifndef	__LEDBEEP__H_
#define	__LEDBEEP__H_

#include "reg52.h"

#define ON  1
#define OFF 0

sbit LED =  P1^1;
sbit BEEP = P1^2;


void  beep_control(unsigned char beep);
void  led_control(unsigned char beep);

#endif	/* __LEDBEEP__H_ */