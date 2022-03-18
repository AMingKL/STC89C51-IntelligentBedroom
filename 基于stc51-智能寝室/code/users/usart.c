#include "usart.h"
#include "Delay.h"

//以下是开机后发送到手机的内容，发送的号码在程序中修改。	
unsigned int  rec_data_len_uart=0;    //标记Buffer_Uart0接收数组
unsigned char idata Buffer_Uart0_Rec[25]={0};		 //Uart0中断接收数组

//注意，无论接收到信号还是发送完信号，都会进中断服务程序的
/*初始化程序（必须使用，否则无法收发），次程序将会使用定时器1*/
void SerialInti()//初始化程序（必须使用，否则无法收发）
{
	TMOD=0x20;//定时器1操作模式2:8位自动重载定时器
	
	TH1=0xfd;//装入初值，波特率9600
	TL1=0xfd;	
	
	TR1=1;//打开定时器
	SM0=0;//设置串行通讯工作模式，（10为一部发送，波特率可变，由定时器1的溢出率控制）
	SM1=1;//(同上)在此模式下，定时器溢出一次就发送一个位的数据
	REN=1;//串行接收允许位（要先设置sm0sm1再开串行允许）
	EA=1;//开总中断
	ES=1;//开串行口中断	
}

/*串行通讯中断，收发完成将进入该中断*/
void Serial_interrupt() interrupt 4 
{
	unsigned char temp_rec_data_uart0;	
	temp_rec_data_uart0 = SBUF;//读取接收数据		
	RI=0;//接收中断信号清零，表示将继续接收			
	Buffer_Uart0_Rec[rec_data_len_uart]=temp_rec_data_uart0;	//接收数据
	rec_data_len_uart++;
	if(rec_data_len_uart>24)
	{
		rec_data_len_uart=0; 	//从头开始接收数据
	}	
}

void Uart1Send(uchar c)
{
	SBUF=c;
	while(!TI);//等待发送完成信号（TI=1）出现
	TI=0;	
}

//串行口连续发送char型数组，遇到终止号/0将停止
void Uart1Sends(uchar *str)
{
	while(*str!='\0')
	{
		SBUF=*str;
		while(!TI);//等待发送完成信号（TI=1）出现
		TI=0;
		str++;
	}
}

void Gsm_proc(void)
{
                    //提示开始发送指令，开始发送指令时，务必确认模块上的LED 已经慢闪，即模块已经注册到网络
    //----------------为什么是下面这些AT指令呢，请看群共享文件SIM900A重要的短信指令文件------------
    Uart1Sends("AT+CSCS=\"UCS2\"\r\n");
    delay_ms(1000);//延时大约3秒
    Uart1Sends("AT+CMGF=1\r\n");
    delay_ms(1000);//延时3秒
		Uart1Sends("AT+CSCA?\r\n");
    delay_ms(1000);//延时3秒
		Uart1Sends("AT+CSMP=17,167,0,25\r\n");
    delay_ms(1000);//延时3秒											   
    Uart1Sends("AT+CMGS=\"0031003500390038003300360037003400350030\"\r\n");
	  //此处修改为对方的电话号，电话话的UNICON编码，可使用我们配套的
	  //资料包中的 中文转UNICON编码工具来获取。特别注意：那个软件
		//获得的unicon 编码待有空格，实际中要去掉，否则模块会报错
    delay_ms(1000);//延时3秒
    Uart1Sends("起火了！请打119");//修改短信内容,短信内容可为中文、英文和数字，但都是对应的unicon 编码。
    Uart1Send(0x1a);
    delay_ms(5000);//延时20秒
	      		 //提示发送完毕
}




///*串口初始化函数*/
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
