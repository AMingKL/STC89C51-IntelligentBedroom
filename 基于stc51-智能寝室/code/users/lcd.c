/**
  *************************************************************************************************************************
  * @file    lcd.c
  * @author  
  * @version V1.0
  * @date    2020-03-19
  * @brief   STC89C51 lcd.c�ļ�
  *************************************************************************************************************************
  * @attention
  *
  * ��lcd.c�ļ�������LCD1602ģ��
  * ����LCD����д����������������ԭ��
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "lcd.h"

/* ���� -----------------------------------------------------------------------------------------------------------------*/
unsigned char display1[16] = " Time:   :  :   "; /* LCD1602��һ����ʾ������ */
unsigned char display2[16] = "Temp:   Hemi:  %"; /* LCD1602�ڶ�����ʾ������ */

/**
 * @brief  ��ʱ��������ʱ1ms
 * @param  c
 * @retval ��
 */
void Lcd1602_Delay1ms(unsigned int c)   //��� 0us
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

#ifndef 	LCD1602_4PINS	 //��û�ж������LCD1602_4PINSʱ

/**
 * @brief  ��LCDд��һ���ֽڵ�����
 * @param  com
 * @retval ��
 */
void LcdWriteCom(unsigned char com)	  //д������
{
	LCD1602_E = 0;     //ʹ��
	LCD1602_RS = 0;	   //ѡ��������
	LCD1602_RW = 0;	   //ѡ��д��
	
	LCD1602_DATAPINS = com;     //��������
	Lcd1602_Delay1ms(1);		//�ȴ������ȶ�

	LCD1602_E = 1;	          //д��ʱ��
	Lcd1602_Delay1ms(5);	  //����ʱ��
	LCD1602_E = 0;
}

#else 
void LcdWriteCom(unsigned char com)	  //д������
{
	LCD1602_E = 0;	 //ʹ������
	LCD1602_RS = 0;	 //ѡ��д������
	LCD1602_RW = 0;	 //ѡ��д��

	LCD1602_DATAPINS = com;	//����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 //д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = com << 4; //���͵���λ
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 //д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
	   
#ifndef 	LCD1602_4PINS		  
/**
 * @brief  ��LCDд��һ���ֽڵ�����
 * @param  dat
 * @retval ��
 */
void LcdWriteData(unsigned char dat)			//д������
{
	LCD1602_E = 0;	//ʹ������
	LCD1602_RS = 1;	//ѡ����������
	LCD1602_RW = 0;	//ѡ��д��

	LCD1602_DATAPINS = dat; //д������
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //д��ʱ��
	Lcd1602_Delay1ms(5);   //����ʱ��
	LCD1602_E = 0;
}

#else
void LcdWriteData(unsigned char dat)			//д������
{
	LCD1602_E = 0;	  //ʹ������
	LCD1602_RS = 1;	  //ѡ��д������
	LCD1602_RW = 0;	  //ѡ��д��

	LCD1602_DATAPINS = dat;	//����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  //д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = dat << 4; //д�����λ
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  //д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif

#ifndef		LCD1602_4PINS
/**
 * @brief  ��ʼ��LCD��
 * @param  ��
 * @retval ��
 */
void LcdInit(void)						  //LCD��ʼ���ӳ���
{
 	LcdWriteCom(0x38);  //����ʾ
	LcdWriteCom(0x0c);  //����ʾ����ʾ���
	LcdWriteCom(0x06);  //дһ��ָ���1
	LcdWriteCom(0x01);  //����
	LcdWriteCom(0x80);  //��������ָ�����
	
	//LcdDisplay(display1, display2); 
}

#else
void LcdInit(void)						  //LCD��ʼ���ӳ���
{
	LcdWriteCom(0x32);	 //��8λ����תΪ4λ����
	LcdWriteCom(0x28);	 //����λ���µĳ�ʼ��
	LcdWriteCom(0x0c);  //����ʾ����ʾ���
	LcdWriteCom(0x06);  //дһ��ָ���1
	LcdWriteCom(0x01);  //����
	LcdWriteCom(0x80);  //��������ָ�����
	
	LcdDisplay(display1, display2); 
}
#endif

/**
 * @brief  LCD1602��ʾ����
 * @param  �����ַ���disp1[16],disp2[16]
 * @retval ��
 */
void LcdDisplay(unsigned char disp1[16], unsigned char disp2[16])
{
	unsigned char i;
	LcdWriteCom(0x80); //��һ��
	for (i = 0; i < 16; i++)  
	{
		LcdWriteData(disp1[i]);	
	}
	LcdWriteCom(0x80 + 0x40); //�ڶ���
	for (i = 0; i < 16; i++)  
	{
		LcdWriteData(disp2[i]);	
	}	
}	

/**
 * @brief  LCD1602��ʾ����
 * @param  x�� y�� ����
 * @retval ��
 */
void LCD_WriteDisplay(unsigned char x, unsigned char y, unsigned char dat)
{
	if (x == 0) //��һ��
	{
	  LcdWriteCom(0x80 + y);	
	}
  else if (x== 1)
	{
		LcdWriteCom(0x80 + 0x40 + y); //�ڶ���
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
