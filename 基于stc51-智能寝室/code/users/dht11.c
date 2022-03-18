#include "dht11.h"
#include "lcd.h"
#include "stdio.h"
//#include "uart.h"


//��ʱus   --2*n+5us
void DHT11_delay_us(unsigned char n)
{
    while(--n);
}

//��ʱms
void DHT11_delay_ms(unsigned int z)
{
   unsigned int i,j;
   for(i=z;i>0;i--)
      for(j=110;j>0;j--);
}
/*------------------------------------------------
              DHT11��ʼ�ź�
------------------------------------------------*/
void DHT11_start()
{
   Data=1;
   DHT11_delay_us(2);
   Data=0;
   DHT11_delay_ms(20);   //��ʱ18ms����
   Data=1;
   DHT11_delay_us(30);
}

/*------------------------------------------------
              ���հ�λ������
------------------------------------------------*/
unsigned char DHT11_rec_byte()      //����һ���ֽ�
{

	unsigned char i,dat;
 
	
	for(i=0;i<8;i++)
	{
			while(!Data);//�ȹ�54us�͵�ƽ
		  DHT11_delay_us(8);//��ʱ30us���жϸߵ�ƽΪ1����Ϊ��
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
              ����40bit����
------------------------------------------------*/
void DHT11_receive()      //����40λ������
{
    unsigned char R_H,R_L,T_H,T_L,revise; 
	
    DHT11_start();
 
    if(Data==0)
    {	
			while(Data==0);   //�ȴ�����     
			DHT11_delay_us(60);  //���ߺ���ʱ80us
	
			R_H=DHT11_rec_byte();    //����ʪ�ȸ߰�λ  
			R_L=DHT11_rec_byte();    //����ʪ�ȵͰ�λ  
			T_H=DHT11_rec_byte();    //�����¶ȸ߰�λ  
			T_L=DHT11_rec_byte();    //�����¶ȵͰ�λ
			revise=DHT11_rec_byte(); //����У��λ

			DHT11_delay_us(25);    //����
			
			LCD_WriteDisplay(1, 5,'0'+R_H/10);
			LCD_WriteDisplay(1, 6,'0'+R_H%10);
				
			LCD_WriteDisplay(1, 13,'0'+T_H/10);
			LCD_WriteDisplay(1, 14,'0'+T_H%10);
    }
}
 
