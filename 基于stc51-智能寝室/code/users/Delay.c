#include "Delay.h"
#include "intrins.h"



void delay_ms(unsigned int xTime)
{
	unsigned char i=0;
  for(;xTime>0;xTime--)
		for(i=110;i>0;i--);
}

//void delay_us(unsigned int wTime)
//{
//	for(;wTime>0;wTime--)
//	{
//	  _nop_();
//	}
//}

