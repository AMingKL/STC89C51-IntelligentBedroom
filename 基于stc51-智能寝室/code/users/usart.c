#include "usart.h"
#include "Delay.h"

//�����ǿ������͵��ֻ������ݣ����͵ĺ����ڳ������޸ġ�	
unsigned int  rec_data_len_uart=0;    //���Buffer_Uart0��������
unsigned char idata Buffer_Uart0_Rec[25]={0};		 //Uart0�жϽ�������

//ע�⣬���۽��յ��źŻ��Ƿ������źţ�������жϷ�������
/*��ʼ�����򣨱���ʹ�ã������޷��շ������γ��򽫻�ʹ�ö�ʱ��1*/
void SerialInti()//��ʼ�����򣨱���ʹ�ã������޷��շ���
{
	TMOD=0x20;//��ʱ��1����ģʽ2:8λ�Զ����ض�ʱ��
	
	TH1=0xfd;//װ���ֵ��������9600
	TL1=0xfd;	
	
	TR1=1;//�򿪶�ʱ��
	SM0=0;//���ô���ͨѶ����ģʽ����10Ϊһ�����ͣ������ʿɱ䣬�ɶ�ʱ��1������ʿ��ƣ�
	SM1=1;//(ͬ��)�ڴ�ģʽ�£���ʱ�����һ�ξͷ���һ��λ������
	REN=1;//���н�������λ��Ҫ������sm0sm1�ٿ���������
	EA=1;//�����ж�
	ES=1;//�����п��ж�	
}

/*����ͨѶ�жϣ��շ���ɽ�������ж�*/
void Serial_interrupt() interrupt 4 
{
	unsigned char temp_rec_data_uart0;	
	temp_rec_data_uart0 = SBUF;//��ȡ��������		
	RI=0;//�����ж��ź����㣬��ʾ����������			
	Buffer_Uart0_Rec[rec_data_len_uart]=temp_rec_data_uart0;	//��������
	rec_data_len_uart++;
	if(rec_data_len_uart>24)
	{
		rec_data_len_uart=0; 	//��ͷ��ʼ��������
	}	
}

void Uart1Send(uchar c)
{
	SBUF=c;
	while(!TI);//�ȴ���������źţ�TI=1������
	TI=0;	
}

//���п���������char�����飬������ֹ��/0��ֹͣ
void Uart1Sends(uchar *str)
{
	while(*str!='\0')
	{
		SBUF=*str;
		while(!TI);//�ȴ���������źţ�TI=1������
		TI=0;
		str++;
	}
}

void Gsm_proc(void)
{
                    //��ʾ��ʼ����ָ���ʼ����ָ��ʱ�����ȷ��ģ���ϵ�LED �Ѿ���������ģ���Ѿ�ע�ᵽ����
    //----------------Ϊʲô��������ЩATָ���أ��뿴Ⱥ�����ļ�SIM900A��Ҫ�Ķ���ָ���ļ�------------
    Uart1Sends("AT+CSCS=\"UCS2\"\r\n");
    delay_ms(1000);//��ʱ��Լ3��
    Uart1Sends("AT+CMGF=1\r\n");
    delay_ms(1000);//��ʱ3��
		Uart1Sends("AT+CSCA?\r\n");
    delay_ms(1000);//��ʱ3��
		Uart1Sends("AT+CSMP=17,167,0,25\r\n");
    delay_ms(1000);//��ʱ3��											   
    Uart1Sends("AT+CMGS=\"0031003500390038003300360037003400350030\"\r\n");
	  //�˴��޸�Ϊ�Է��ĵ绰�ţ��绰����UNICON���룬��ʹ���������׵�
	  //���ϰ��е� ����תUNICON���빤������ȡ���ر�ע�⣺�Ǹ����
		//��õ�unicon ������пո�ʵ����Ҫȥ��������ģ��ᱨ��
    delay_ms(1000);//��ʱ3��
    Uart1Sends("����ˣ����119");//�޸Ķ�������,�������ݿ�Ϊ���ġ�Ӣ�ĺ����֣������Ƕ�Ӧ��unicon ���롣
    Uart1Send(0x1a);
    delay_ms(5000);//��ʱ20��
	      		 //��ʾ�������
}




///*���ڳ�ʼ������*/
//void Usart_Init(void)//9600
//{	
//	SCON=0X50;			
//	TMOD=0X20;			
//	PCON=0X00;		
//	TH1=0XFd;		    
//	TL1=0XFd;
//	TR1=1;					
//	ES = 1;        
//  EA = 1;         
//}
///*chuankou*/  
//void Uart_SendChar(unsigned char  dat)
//{
//    SBUF = dat; 
//    while(!TI); 
//    TI = 0; 
//}
