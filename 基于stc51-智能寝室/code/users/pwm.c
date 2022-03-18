#include "pwm.h"

#define uchar unsigned char
#define uint unsigned int

uint cnt_PWM = 0;  //PWMƵ�ʼ��� 
unsigned char VTH0 ,VTL0;	//T0��ʱ��ֵ

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
  TMOD=(TMOD&0xf0)|0x01;  //T0�����ڷ�ʽ1
  VTH0=(65536-vT0*Fosc/120000)/256;
  VTL0=(65536-vT0*Fosc/120000)%256;
  TH0=VTH0;		//д���ֵ��8λ
  TL0=VTL0;		//д���ֵ��8λ  
  EA=1;		   //���ж�����
  ET0=1;	   //T0�ж�����
  TR0=1;	   //������ʱ��T0
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