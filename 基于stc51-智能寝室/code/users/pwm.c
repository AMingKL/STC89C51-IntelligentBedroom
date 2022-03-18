#include "pwm.h"

#define uchar unsigned char
#define uint unsigned int

uint cnt_PWM = 0;  //PWM频率计数 
unsigned char VTH0 ,VTL0;	//T0定时初值

void Motor_forward(void)
{
	IN1 = 0;
	IN2 = 1;
	ENA=1;
}

void Motor_reverse(void)
{
	IN1 = 1;
	IN2 = 0;
	ENA=1;
}

void Motor_stop(void)
{
	IN1 = 0;
	IN2 = 0;
	ENA=0;
}



void T0Init(unsigned char vT0)
{ 
  TMOD=(TMOD&0xf0)|0x01;  //T0工作在方式1
  VTH0=(65536-vT0*Fosc/120000)/256;
  VTL0=(65536-vT0*Fosc/120000)%256;
  TH0=VTH0;		//写入初值高8位
  TL0=VTL0;		//写入初值低8位  
  EA=1;		   //总中断允许
  ET0=1;	   //T0中断允许
  TR0=1;	   //启动定时器T0
}

void T0Int(void) interrupt 1
{  	
	TH0=VTH0;
	TL0=VTL0;
	cnt_PWM ++;
	if(cnt_PWM >= 100)
	{
		cnt_PWM = 0;
	}
}