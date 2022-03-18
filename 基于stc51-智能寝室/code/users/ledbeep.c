#include "ledbeep.h"
#include "Delay.h"

void  led_control(unsigned char led)
{
   if(led==1)
	 {
		LED=0;
	 }
	 else
	 {
	  LED=1;
	 }
}

void  beep_control(unsigned char beep)
{
		if(beep==1)
	 {
		BEEP=0;
		 delay_ms(20);
		 BEEP=1;
	 }
	 else
	 {
	  BEEP=1;
	 }
}