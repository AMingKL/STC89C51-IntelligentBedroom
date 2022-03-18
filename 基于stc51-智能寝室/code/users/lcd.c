/**
  *************************************************************************************************************************
  * @file    lcd.c
  * @author  
  * @version V1.0
  * @date    2020-03-19
  * @brief   STC89C51 lcd.c文件
  *************************************************************************************************************************
  * @attention
  *
  * 此lcd.c文件适用于LCD1602模块
  * 其中LCD部分写操作函数来自正点原子
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "lcd.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/
unsigned char display1[16] = " Time:   :  :   "; /* LCD1602第一行显示的内容 */
unsigned char display2[16] = "Temp:   Hemi:  %"; /* LCD1602第二行显示的内容 */

/**
 * @brief  延时函数，延时1ms
 * @param  c
 * @retval 无
 */
void Lcd1602_Delay1ms(unsigned int c)   //误差 0us
{
	unsigned char a,b;
	for (; c > 0; c--)
	{
		for (b = 199; b > 0; b--)
		{
			for(a = 1; a > 0; a--);
		}      
	}   	
}

#ifndef 	LCD1602_4PINS	 //当没有定义这个LCD1602_4PINS时

/**
 * @brief  向LCD写入一个字节的命令
 * @param  com
 * @retval 无
 */
void LcdWriteCom(unsigned char com)	  //写入命令
{
	LCD1602_E = 0;     //使能
	LCD1602_RS = 0;	   //选择发送命令
	LCD1602_RW = 0;	   //选择写入
	
	LCD1602_DATAPINS = com;     //放入命令
	Lcd1602_Delay1ms(1);		//等待数据稳定

	LCD1602_E = 1;	          //写入时序
	Lcd1602_Delay1ms(5);	  //保持时间
	LCD1602_E = 0;
}

#else 
void LcdWriteCom(unsigned char com)	  //写入命令
{
	LCD1602_E = 0;	 //使能清零
	LCD1602_RS = 0;	 //选择写入命令
	LCD1602_RW = 0;	 //选择写入

	LCD1602_DATAPINS = com;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 //写入时序
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = com << 4; //发送低四位
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 //写入时序
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
	   
#ifndef 	LCD1602_4PINS		  
/**
 * @brief  向LCD写入一个字节的数据
 * @param  dat
 * @retval 无
 */
void LcdWriteData(unsigned char dat)			//写入数据
{
	LCD1602_E = 0;	//使能清零
	LCD1602_RS = 1;	//选择输入数据
	LCD1602_RW = 0;	//选择写入

	LCD1602_DATAPINS = dat; //写入数据
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //写入时序
	Lcd1602_Delay1ms(5);   //保持时间
	LCD1602_E = 0;
}

#else
void LcdWriteData(unsigned char dat)			//写入数据
{
	LCD1602_E = 0;	  //使能清零
	LCD1602_RS = 1;	  //选择写入数据
	LCD1602_RW = 0;	  //选择写入

	LCD1602_DATAPINS = dat;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  //写入时序
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = dat << 4; //写入低四位
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  //写入时序
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif

#ifndef		LCD1602_4PINS
/**
 * @brief  初始化LCD屏
 * @param  无
 * @retval 无
 */
void LcdInit(void)						  //LCD初始化子程序
{
 	LcdWriteCom(0x38);  //开显示
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);  //设置数据指针起点
	
	//LcdDisplay(display1, display2); 
}

#else
void LcdInit(void)						  //LCD初始化子程序
{
	LcdWriteCom(0x32);	 //将8位总线转为4位总线
	LcdWriteCom(0x28);	 //在四位线下的初始化
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);  //设置数据指针起点
	
	LcdDisplay(display1, display2); 
}
#endif

/**
 * @brief  LCD1602显示函数
 * @param  两段字符串disp1[16],disp2[16]
 * @retval 无
 */
void LcdDisplay(unsigned char disp1[16], unsigned char disp2[16])
{
	unsigned char i;
	LcdWriteCom(0x80); //第一行
	for (i = 0; i < 16; i++)  
	{
		LcdWriteData(disp1[i]);	
	}
	LcdWriteCom(0x80 + 0x40); //第二行
	for (i = 0; i < 16; i++)  
	{
		LcdWriteData(disp2[i]);	
	}	
}	

/**
 * @brief  LCD1602显示函数
 * @param  x轴 y轴 内容
 * @retval 无
 */
void LCD_WriteDisplay(unsigned char x, unsigned char y, unsigned char dat)
{
	if (x == 0) //第一行
	{
	  LcdWriteCom(0x80 + y);	
	}
  else if (x== 1)
	{
		LcdWriteCom(0x80 + 0x40 + y); //第二行
	}
	
	LcdWriteData(dat);	
}

void LCD_ClearLine(unsigned char line)
{
	if (line == 0)
	{
		LcdWriteCom(0x80);
	}
	else if (line == 1)
	{
		LcdWriteCom(0x80 + 0x40);
	}
	
	LcdWriteData(' ');
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
	LcdWriteData(' ');	
		
}

/*****************************************************END OF FILE*********************************************************/	
