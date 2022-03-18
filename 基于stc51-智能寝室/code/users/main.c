/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  AMKL
  * @version V1.0
  * @date    2022-03-12
  * @brief   基于51单片机-智能寝室系统
  *************************************************************************************************************************
  * @attention
  *
  * 使用前阅读readme.txt文件，进行接线！
  *设定上电时间为7:59:50，闹钟设置为08:00:00
  *KEYI--界面切换，第一次按下到闹钟时间设置界面，第二次回到时间显示界面，往复循环
  *KEY2--闹钟时间单位切换，初始为小时调整，第一次按下为分钟调整，第二次按下为秒钟调整，往复循环
	*KEY3--闹钟时间为选中时间单位数加一,往复循环
  *KEY2--闹钟时间为选中时间单位数减一,往复循环
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
/* IO定义 -----------------------------------------------------------------------------------------------------------------*/
sbit SmokeOUT=P1^6;//烟雾传感器数据引脚
sbit LedRead=P2^4;//语音输出控制led引脚
sbit MotorReadLeft=P2^5;//语音输出控制电机正转引脚
sbit MotorReadRight=P2^6;//语音输出控制电机反转引脚
/* 函数申明 -----------------------------------------------------------------------------------------------------------------*/
void key_Proc(void);
void lcd1602_Proc(void);
void motor_Proc(void);
void dht11_proc(void);
void VoiceWake_Proc(void);
void ds1302_proc(void);
void led_beep_proc(void);
void Alarm_proc(void);
void SystemPara_Init();
/* 变量定义 -----------------------------------------------------------------------------------------------------------------*/
typedef struct
{
	unsigned char hour;//小时
	unsigned char min;//分钟
	unsigned char sec;//秒钟
	
	unsigned char alarm_hour;//闹钟小时
	unsigned char alarm_min;//闹钟分钟
	unsigned char alarm_sec;//闹钟秒钟
}TIME_InitTypeDef;

TIME_InitTypeDef DS1302;

typedef struct
{
	unsigned char interface;//lcd1602界面
	unsigned char choose;////闹钟调整时间切换变量
}SYSTEM_InitTypeDef;

SYSTEM_InitTypeDef SYSTEM;

unsigned char flag_send=0;//发送短信标志位

#define MAIN 0  //主界面
#define SET  1  //设置界面

/**
 * @brief  主函数
 * @param  无
 * @retval 无
 */
int main(void)
{
	T0Init(10);//10ms定时器
	SystemPara_Init();//系统参数初始化
	SerialInti();//串口初始化
	delay_ms(15000);//延时约15秒,此处延时，是为了让模块有足够的时间注册到网络，
	Ds1302Init();//ds1302初始化
  LcdInit();//lcd1602初始化
	LcdDisplay(display1,display2);//加载lcd字符串常量
	delay_ms(100);//延时
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
 * @brief  语音处理函数
 * @param  无
 * @retval 无
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
 * @brief  闹钟，警报函数
 * @param  无
 * @retval 无
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
	
	 if(SmokeOUT==0)//当浓度高于设定值时 ，执行条件函数
	 {
		delay_ms(20);//延时抗干扰
		if((SmokeOUT==0)&&(flag_send==0))//确定 浓度高于设定值时 ，执行条件函数
	  {
			flag_send=1;
			beep_control(ON);
			Gsm_proc();   //蜂鸣器响，发送报警消息
		}
	 }
	 else
	 {
	  beep_control(OFF);
		flag_send=0;
	 }
}

/**
 * @brief  dht11读取处理函数
 * @param  无
 * @retval 无
 */
void dht11_proc(void)
{
 DHT11_receive();
}


/**
 * @brief  按键处理函数
 * @param  无
 * @retval 无
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
				  //保存设定值，返回主页面
					SYSTEM.interface=MAIN;
					LcdDisplay(display1,display2);
				}
			break;//设置键，再次按下确认返回键
				
			case 2: 
				if(SYSTEM.interface==SET)
				{
				 (SYSTEM.choose==3)?(SYSTEM.choose=0):(SYSTEM.choose++);
				}
			break;//切换键
			
			case 3: 
				if(SYSTEM.choose==0)//小时
				{
				 (DS1302.alarm_hour==23)?(DS1302.alarm_hour=0):(DS1302.alarm_hour++);
				}
				else if	(SYSTEM.choose==1)//分钟
				{
				 (DS1302.alarm_min==59)?(DS1302.alarm_min=0):(DS1302.alarm_min++);
				}
				else //秒钟
				{
				 (DS1302.alarm_sec==59)?(DS1302.alarm_sec=0):(DS1302.alarm_sec++);
				}
			break;//时间加一键
			
			case 4: 
					if(SYSTEM.choose==0)//小时
				{
				 (DS1302.alarm_hour==0)?(DS1302.alarm_hour=23):(DS1302.alarm_hour--);
				}
				else if	(SYSTEM.choose==1)//分钟
				{
				 (DS1302.alarm_min==0)?(DS1302.alarm_min=59):(DS1302.alarm_min--);
				}
				else //秒钟
				{
				 (DS1302.alarm_sec==0)?(DS1302.alarm_sec=59):(DS1302.alarm_sec--);
				}	
			break;//时间减一键
			
			default:break;
		}
}

/**
 * @brief  系统参数初始化函数
 * @param  无
 * @retval 无
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
 * @brief  系统参数初始化函数
 * @param  无
 * @retval 无
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
 * @brief  lcd1602函数
 * @param  无
 * @retval 无
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
