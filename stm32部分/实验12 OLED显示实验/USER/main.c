#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"


//ALIENTEK ̽����STM32F407������ ʵ��12
//OLED��ʾʵ��-�⺯���汾 
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
int X_error=1000,Y_error=1000,Temp=0;//X_error��Y_errorΪ���˫���ɶ�ƫ��ֵ��TempΪ��������
char *Identity="ZDS";
int main(void)
{ 
	u8 t=0;
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
	t=' ';  
	while(1) 
	{		
		OLED_ShowNum(64,32,X_error,4,12);//��ʾASCII�ַ�	 ������յ���ֵ�����������ֵ������λ������˵���ڸ���ֵ��4���������Χ�ǣ�0~9999���������÷�Χ��������쳣��
		OLED_ShowNum(64,48,Y_error,4,12);	//��ʾASCII�ַ�����ֵ    
		OLED_ShowString(64,16,Identity,12); 
		OLED_Refresh_Gram();        //������ʾ��OLED
		delay_ms(50);
		LED0=!LED0;
	}
}
