#include "key.h"
#include "Delay.h"

/**
  * @brief  按键检测函数
  * @param  无  
  * @retval 按键键值
  */
unsigned char KeyScan(void)
{
	unsigned char key_val =  0;
	
	if (KEY1 == 0)
	{
		delay_ms(20);
		if (KEY1 == 0)
		{
			key_val = 1;
		}
	}	
	if (KEY2 == 0)
	{
		delay_ms(20);
		if (KEY2 == 0)
		{
			key_val = 2;
		}
	}	
	if (KEY3 == 0)
	{
		delay_ms(20);
		if (KEY3 == 0)
		{
			key_val = 3;
		}
	}
	if (KEY4 == 0)
	{
		delay_ms(20);
		if (KEY4 == 0)
		{
			key_val = 4;
		}
	}	
	return key_val;
}
