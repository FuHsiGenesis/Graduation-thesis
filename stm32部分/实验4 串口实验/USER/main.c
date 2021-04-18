#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "oled.h"


int main(void)
{ 
 
	u8 t; // ����
	u8 len;	// ����
	u16 times=0; // ����
	int X_error=1000,Y_error=1000,Temp=0;//X_error��Y_errorΪ���˫���ɶ�ƫ��ֵ��TempΪ��������
  char Identity[10]=""; // �ַ�������

	//������ʼ�������
  u8 key;             //�����ֵ
	delay_init(168);    //��ʼ����ʱ����
	KEY_Init();         //��ʼ���밴�����ӵ�Ӳ���ӿ�
	//LED0=0;				  	//�ȵ�����	
	//LED_Init();				//��ʼ��LED�˿� 
	//BEEP_Init();      //��ʼ���������˿�
	
	//���ڳ�ʼ�������
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
  OLED_ShowString(0,0 ,"Temp:",12);  
	OLED_ShowString(0,16,"Identity:",12); 
	OLED_ShowString(0,32,"X_error:",12); 
	OLED_ShowString(0,48,"Y_error:",12); 
	OLED_Refresh_Gram();//������ʾ��OLED	 
	while(1)
	{
		LED1=!LED1;
		delay_ms(200);
		if(USART_RX_STA&0x8000) // ����յ�����
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)
			{
				LED0=!LED0;
				delay_ms(200);
				USART_SendData(USART1, USART_RX_BUF[t]);              //�򴮿�1�������ݣ�USART_RX_BUF[]Ϊ���ڽ������ݵĻ����������ջ���,���200���ֽ�(�������ж���).
				OLED_ShowString(64,32,USART_RX_BUF,12);					      //�������βε������������ַ����ĵ�ַ��Ҳ���ַ��������֣���������ŵĲ��֣�   USART_RX_BUF
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			USART_RX_STA=0;
		}
		OLED_Refresh_Gram();        //������ʾ��OLED
		delay_ms(500);               //������LED,ʹ��LED������仯�ɼ�LED0=!LED0;
	}
}

