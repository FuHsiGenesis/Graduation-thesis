#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "oled.h"


int main(void)
{ 
 
	u8 t; // 串口
	u8 len;	// 串口
	u16 times=0; // 串口
	int X_error=1000,Y_error=1000,Temp=0;//X_error、Y_error为舵机双自由度偏差值；Temp为测量体温
  char Identity[10]=""; // 字符串定义

	//按键初始化代码块
  u8 key;             //保存键值
	delay_init(168);    //初始化延时函数
	KEY_Init();         //初始化与按键连接的硬件接口
	//LED0=0;				  	//先点亮红?	
	//LED_Init();				//初始化LED端口 
	//BEEP_Init();      //初始化蜂鸣器端口
	
	//串口初始化代码块
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	LED_Init();		  		//初始化与LED连接的硬件接口  
	
	//OLED初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED
 	OLED_Init();				//初始化OLED
  OLED_ShowString(0,0 ,"Temp:",12);  
	OLED_ShowString(0,16,"Identity:",12); 
	OLED_ShowString(0,32,"X_error:",12); 
	OLED_ShowString(0,48,"Y_error:",12); 
	OLED_Refresh_Gram();//更新显示到OLED	 
	while(1)
	{
		LED1=!LED1;
		delay_ms(200);
		if(USART_RX_STA&0x8000) // 如果收到数据
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				LED0=!LED0;
				delay_ms(200);
				USART_SendData(USART1, USART_RX_BUF[t]);              //向串口1发送数据，USART_RX_BUF[]为串口接收数据的缓存区，接收缓冲,最大200个字节(库里面有定义).
				OLED_ShowString(64,32,USART_RX_BUF,12);					      //第三个形参的数据类型是字符串的地址，也是字符串的名字（不带方框号的部分）   USART_RX_BUF
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			USART_RX_STA=0;
		}
		OLED_Refresh_Gram();        //更新显示到OLED
		delay_ms(500);               //服务于LED,使得LED的亮灭变化可见LED0=!LED0;
	}
}

