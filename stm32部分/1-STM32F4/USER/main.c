
///////////////////////////////////////////////////////////////////////////
//�˳���Ϊstm32F4��IIC����0.96OLED ��4�ߣ�
//˵����
//     --------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PC9
//              SDA   ��PA8   
//     ---------------------------------------------------------
//ACTION
///////////////////////////////////////////////////////////////////
#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "stm32f4xx_gpio.h"
#include "delay.h"
#include "myiic.h"
int main(void)
{ 
/*********************��ʼ��*************************/

	 delay_init(168);  //��ʼ����ʱ����  168
	 IIC_Init();       //IIC��ʼ��
   OLED_Init();		   //��ʼ��OLED
	
	while(1)
	{
		OLED_ShowCHinese(0,0,0);//��
		OLED_ShowCHinese(18,0,1);//��
		OLED_ShowCHinese(36,0,2);//԰
		OLED_ShowCHinese(54,0,3);//��
		OLED_ShowCHinese(72,0,4);//��
		OLED_ShowCHinese(90,0,5);//��
		OLED_ShowCHinese(108,0,6);//��
	} 	    
}


