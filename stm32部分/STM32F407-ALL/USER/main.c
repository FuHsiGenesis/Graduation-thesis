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
 
	u8 t,n; // ����
	u8 len;	// ����
	u16 times=30; // ����
	int temp=30; // �¶Ȳ���
	int X_error=1000,Y_error=1000,Temp=0;//X_error��Y_errorΪ���˫���ɶ�ƫ��ֵ��TempΪ��������
  char Identity[10]=""; // �ַ�������
  u8 UsartDisPlay[200];  // oled������ʾ��
	
	//������ʼ�������
  u8 key;             //�����ֵ
	delay_init(168);    //��ʼ����ʱ����
	KEY_Init();         //��ʼ���밴�����ӵ�Ӳ���ӿ�
	//LED0=0;				  	//�ȵ�����	
	//LED_Init();				//��ʼ��LED�˿� 
	//BEEP_Init();      //��ʼ���������˿�
	
	//MLX90614��ʼ��
	SMBus_Init();
	
	//���ڳ�ʼ�������//ֻ�������������Ϣ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	
	//OLED��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED
 	OLED_Init();				//��ʼ��OLED
  OLED_ShowString(0,8 ,"Temp:",16);  
	OLED_ShowString(0,40,"Identity:",16); 
//	OLED_ShowString(0,32,"X_error:",12); 
//	OLED_ShowString(0,48,"Y_error:",12); 
	OLED_Refresh_Gram();//������ʾ��OLED	 
	while(1)
	{
		if(USART_RX_STA&0x8000) // ����յ�����
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)
			{
				 if(USART_RX_BUF[t]==0xb3)     
					 if(USART_RX_BUF[t+1]==0xb3)
						  n=t+2;    
			}
		 OLED_Fill(64,0,123,63,0);   //��������
     for(t=0;n<len;t++,n++)
				UsartDisPlay[t] = USART_RX_BUF[n];  
			OLED_ShowString(64,32,UsartDisPlay,12);
			USART_RX_STA=0;
		}
		OLED_Refresh_Gram();        //������ʾ��OLED
		for(t=0;t<=100;t++)
			UsartDisPlay[t] ='\0';    //��ʾ�������
		temp=SMBus_ReadTemp(); 
		OLED_ShowNum(64,32,temp,3,24);
		
	}
}

