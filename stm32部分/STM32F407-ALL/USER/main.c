#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "oled.h"
#include "mlx90614.h"

int main(void)
{ 
 
	u8 t,n; // 串口
	u8 len;	// 串口
	u16 times=30; // 串口
	int temp=30; // 温度参数
	int X_error=1000,Y_error=1000,Temp=0;//X_error、Y_error为舵机双自由度偏差值；Temp为测量体温
  char Identity[10]=""; // 字符串定义
  u8 UsartDisPlay[200];  // oled上面显示的
	
	//按键初始化代码块
  u8 key;             //保存键值
	delay_init(168);    //初始化延时函数
	KEY_Init();         //初始化与按键连接的硬件接口
	//LED0=0;				  	//先点亮红?	
	//LED_Init();				//初始化LED端口 
	//BEEP_Init();      //初始化蜂鸣器端口
	
	//MLX90614初始化
	SMBus_Init();
	
	//串口初始化代码块//只用来接收身份信息
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
  OLED_ShowString(0,8 ,"Temp:",16);  
	OLED_ShowString(0,40,"Identity:",16); 
//	OLED_ShowString(0,32,"X_error:",12); 
//	OLED_ShowString(0,48,"Y_error:",12); 
	OLED_Refresh_Gram();//更新显示到OLED	 
	while(1)
	{
		if(USART_RX_STA&0x8000) // 如果收到数据
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				 if(USART_RX_BUF[t]==0xb3)     
					 if(USART_RX_BUF[t+1]==0xb3)
						  n=t+2;    
			}
		 OLED_Fill(64,0,123,63,0);   //部分清屏
     for(t=0;n<len;t++,n++)
				UsartDisPlay[t] = USART_RX_BUF[n];  
			OLED_ShowString(64,32,UsartDisPlay,12);
			USART_RX_STA=0;
		}
		OLED_Refresh_Gram();        //更新显示到OLED
		for(t=0;t<=100;t++)
			UsartDisPlay[t] ='\0';    //显示数组清空
		temp=SMBus_ReadTemp(); 
		OLED_ShowNum(64,32,temp,3,24);
		
	}
}

