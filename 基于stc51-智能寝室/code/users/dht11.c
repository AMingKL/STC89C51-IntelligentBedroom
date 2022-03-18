#include "dht11.h"
#include "lcd.h"
#include "stdio.h"
//#include "uart.h"


//延时us   --2*n+5us
void DHT11_delay_us(unsigned char n)
{
    while(--n);
}

//延时ms
void DHT11_delay_ms(unsigned int z)
{
   unsigned int i,j;
   for(i=z;i>0;i--)
      for(j=110;j>0;j--);
}
/*------------------------------------------------
              DHT11开始信号
------------------------------------------------*/
void DHT11_start()
{
   Data=1;
   DHT11_delay_us(2);
   Data=0;
   DHT11_delay_ms(20);   //延时18ms以上
   Data=1;
   DHT11_delay_us(30);
}

/*------------------------------------------------
              接收八位二进制
------------------------------------------------*/
unsigned char DHT11_rec_byte()      //接收一个字节
{

	unsigned char i,dat;
 
	
	for(i=0;i<8;i++)
	{
			while(!Data);//度过54us低电平
		  DHT11_delay_us(8);//延时30us后判断高电平为1否则为零
			dat<<=1;
			if(Data==1)
			{
				dat+=1;
			}
			while(Data);
	}
 
	return dat;
}

/*------------------------------------------------
              接收40bit数据
------------------------------------------------*/
void DHT11_receive()      //接收40位的数据
{
    unsigned char R_H,R_L,T_H,T_L,revise; 
	
    DHT11_start();
 
    if(Data==0)
    {	
			while(Data==0);   //等待拉高     
			DHT11_delay_us(60);  //拉高后延时80us
	
			R_H=DHT11_rec_byte();    //接收湿度高八位  
			R_L=DHT11_rec_byte();    //接收湿度低八位  
			T_H=DHT11_rec_byte();    //接收温度高八位  
			T_L=DHT11_rec_byte();    //接收温度低八位
			revise=DHT11_rec_byte(); //接收校正位

			DHT11_delay_us(25);    //结束
			
			LCD_WriteDisplay(1, 5,'0'+R_H/10);
			LCD_WriteDisplay(1, 6,'0'+R_H%10);
				
			LCD_WriteDisplay(1, 13,'0'+T_H/10);
			LCD_WriteDisplay(1, 14,'0'+T_H%10);
    }
}
 
