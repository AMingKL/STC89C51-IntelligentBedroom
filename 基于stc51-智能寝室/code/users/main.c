/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  AMKL
  * @version V1.0
  * @date    2022-03-12
  * @brief   ����51��Ƭ��-��������ϵͳ
  *************************************************************************************************************************
  * @attention
  *
  * ʹ��ǰ�Ķ�readme.txt�ļ������н��ߣ�
  *�趨�ϵ�ʱ��Ϊ7:59:50����������Ϊ08:00:00
  *KEYI--�����л�����һ�ΰ��µ�����ʱ�����ý��棬�ڶ��λص�ʱ����ʾ���棬����ѭ��
  *KEY2--����ʱ�䵥λ�л�����ʼΪСʱ��������һ�ΰ���Ϊ���ӵ������ڶ��ΰ���Ϊ���ӵ���������ѭ��
	*KEY3--����ʱ��Ϊѡ��ʱ�䵥λ����һ,����ѭ��
  *KEY2--����ʱ��Ϊѡ��ʱ�䵥λ����һ,����ѭ��
  *	
*************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "reg52.h"
#include "ds1302.h"
#include "lcd.h"
#include "pwm.h"
#include "key.h"
#include "Delay.h"
#include "dht11.h"
#include "usart.h"
#include "ledbeep.h"
#include "intrins.h"
/* IO���� -----------------------------------------------------------------------------------------------------------------*/
sbit SmokeOUT=P1^6;//����������������
sbit LedRead=P2^4;//�����������led����
sbit MotorReadLeft=P2^5;//����������Ƶ����ת����
sbit MotorReadRight=P2^6;//����������Ƶ����ת����
/* �������� -----------------------------------------------------------------------------------------------------------------*/
void key_Proc(void);
void lcd1602_Proc(void);
void motor_Proc(void);
void dht11_proc(void);
void VoiceWake_Proc(void);
void ds1302_proc(void);
void led_beep_proc(void);
void Alarm_proc(void);
void SystemPara_Init();
/* �������� -----------------------------------------------------------------------------------------------------------------*/
typedef struct
{
	unsigned char hour;//Сʱ
	unsigned char min;//����
	unsigned char sec;//����
	
	unsigned char alarm_hour;//����Сʱ
	unsigned char alarm_min;//���ӷ���
	unsigned char alarm_sec;//��������
}TIME_InitTypeDef;

TIME_InitTypeDef DS1302;

typedef struct
{
	unsigned char interface;//lcd1602����
	unsigned char choose;////���ӵ���ʱ���л�����
}SYSTEM_InitTypeDef;

SYSTEM_InitTypeDef SYSTEM;

unsigned char flag_send=0;//���Ͷ��ű�־λ

#define MAIN 0  //������
#define SET  1  //���ý���

/**
 * @brief  ������
 * @param  ��
 * @retval ��
 */
int main(void)
{
	T0Init(10);//10ms��ʱ��
	SystemPara_Init();//ϵͳ������ʼ��
	SerialInti();//���ڳ�ʼ��
	delay_ms(15000);//��ʱԼ15��,�˴���ʱ����Ϊ����ģ�����㹻��ʱ��ע�ᵽ���磬
	Ds1302Init();//ds1302��ʼ��
  LcdInit();//lcd1602��ʼ��
	LcdDisplay(display1,display2);//����lcd�ַ�������
	delay_ms(100);//��ʱ
	while(1)
	{
		ds1302_proc();
		dht11_proc();
		key_Proc();
		Alarm_proc();
		VoiceWake_Proc();
		lcd1602_Proc();		
	}
}

/**
 * @brief  ����������
 * @param  ��
 * @retval ��
 */
void VoiceWake_Proc(void)
{
  if(LedRead==0)
	{
	 led_control(ON);
	}
	else
	{
	 led_control(OFF);
	}
	
	if(MotorReadLeft==1)
	{
		Motor_forward();
	}
  else if(MotorReadRight==1)	
	{
	 Motor_reverse();
	}
	else
	{
	 Motor_stop();
	}
	
}

/**
 * @brief  ���ӣ���������
 * @param  ��
 * @retval ��
 */
void Alarm_proc(void)
{
  if((DS1302.alarm_hour==DS1302.hour)&&(DS1302.alarm_min==DS1302.min)&&(DS1302.alarm_sec==DS1302.sec))
	{
	 beep_control(ON);
	}
	else
	{
	 beep_control(OFF);
	}
	
	 if(SmokeOUT==0)//��Ũ�ȸ����趨ֵʱ ��ִ����������
	 {
		delay_ms(20);//��ʱ������
		if((SmokeOUT==0)&&(flag_send==0))//ȷ�� Ũ�ȸ����趨ֵʱ ��ִ����������
	  {
			flag_send=1;
			beep_control(ON);
			Gsm_proc();   //�������죬���ͱ�����Ϣ
		}
	 }
	 else
	 {
	  beep_control(OFF);
		flag_send=0;
	 }
}

/**
 * @brief  dht11��ȡ������
 * @param  ��
 * @retval ��
 */
void dht11_proc(void)
{
 DHT11_receive();
}


/**
 * @brief  ����������
 * @param  ��
 * @retval ��
 */
void key_Proc(void)
{
	unsigned char key_temp;
 		key_temp = KeyScan();
		switch (key_temp)
		{
			case 1:  
				if(SYSTEM.interface==MAIN)
				{
					SYSTEM.interface=SET;
					LCD_ClearLine(1);
				}
				else if(SYSTEM.interface==SET)
				{
				  //�����趨ֵ��������ҳ��
					SYSTEM.interface=MAIN;
					LcdDisplay(display1,display2);
				}
			break;//���ü����ٴΰ���ȷ�Ϸ��ؼ�
				
			case 2: 
				if(SYSTEM.interface==SET)
				{
				 (SYSTEM.choose==3)?(SYSTEM.choose=0):(SYSTEM.choose++);
				}
			break;//�л���
			
			case 3: 
				if(SYSTEM.choose==0)//Сʱ
				{
				 (DS1302.alarm_hour==23)?(DS1302.alarm_hour=0):(DS1302.alarm_hour++);
				}
				else if	(SYSTEM.choose==1)//����
				{
				 (DS1302.alarm_min==59)?(DS1302.alarm_min=0):(DS1302.alarm_min++);
				}
				else //����
				{
				 (DS1302.alarm_sec==59)?(DS1302.alarm_sec=0):(DS1302.alarm_sec++);
				}
			break;//ʱ���һ��
			
			case 4: 
					if(SYSTEM.choose==0)//Сʱ
				{
				 (DS1302.alarm_hour==0)?(DS1302.alarm_hour=23):(DS1302.alarm_hour--);
				}
				else if	(SYSTEM.choose==1)//����
				{
				 (DS1302.alarm_min==0)?(DS1302.alarm_min=59):(DS1302.alarm_min--);
				}
				else //����
				{
				 (DS1302.alarm_sec==0)?(DS1302.alarm_sec=59):(DS1302.alarm_sec--);
				}	
			break;//ʱ���һ��
			
			default:break;
		}
}

/**
 * @brief  ϵͳ������ʼ������
 * @param  ��
 * @retval ��
 */
void SystemPara_Init()
{
  SYSTEM.interface=MAIN;
	SYSTEM.choose=0;
	DS1302.alarm_hour=8;
	DS1302.alarm_min=0;
	DS1302.alarm_sec=0;
}
	

/**
 * @brief  ϵͳ������ʼ������
 * @param  ��
 * @retval ��
 */
void ds1302_proc(void)
{
	ET0=0;
	Ds1302ReadTime();	
	ET0=1;
	DS1302.sec  =(TIME[0]/16)*10+(TIME[0]&0x0f);
	DS1302.min  =(TIME[1]/16)*10+(TIME[1]&0x0f);
	DS1302.hour =(TIME[2]/16)*10+(TIME[2]&0x0f);
}

/**
 * @brief  lcd1602����
 * @param  ��
 * @retval ��
 */
void lcd1602_Proc(void)
{
	if(SYSTEM.interface==MAIN)
	{
		LCD_WriteDisplay(0, 7,'0'+DS1302.hour/10);
		LCD_WriteDisplay(0, 8,'0'+DS1302.hour%10);
		
		LCD_WriteDisplay(0, 10,'0'+DS1302.min/10);
		LCD_WriteDisplay(0, 11,'0'+DS1302.min%10);
		
		LCD_WriteDisplay(0, 13,'0'+DS1302.sec/10);
		LCD_WriteDisplay(0, 14,'0'+DS1302.sec%10);	 
	}
	else
	{
		LCD_WriteDisplay(0, 7,'0'+DS1302.alarm_hour/10);
		LCD_WriteDisplay(0, 8,'0'+DS1302.alarm_hour%10);
		
		LCD_WriteDisplay(0, 10,'0'+DS1302.alarm_min/10);
		LCD_WriteDisplay(0, 11,'0'+DS1302.alarm_min%10);
		
		LCD_WriteDisplay(0, 13,'0'+DS1302.alarm_sec/10);
		LCD_WriteDisplay(0, 14,'0'+DS1302.alarm_sec%10);	
	}

}
/*****************************************************END OF FILE*********************************************************/	
