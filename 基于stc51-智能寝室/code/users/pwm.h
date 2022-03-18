#ifndef	__PWM__H_
#define	__PWM__H_

#include "reg52.h"

sbit IN1 = P3^3;
sbit IN2 = P3^4;
sbit ENA = P3^2;


#define Fosc (12000000L)

void Motor_forward(void);
void Motor_reverse(void);
void Motor_stop(void);
void T0Init(unsigned char vT0);

#endif	/* __PWM__H_ */