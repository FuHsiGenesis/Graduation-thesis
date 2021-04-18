#include "mlx90614.h"
#include "TM1637.h"
#include "systick.h"//�ӳٺ���
#include "math.h"//ceilȡ������Ҫ�õ�
/****************************************************************************
* Function Name  : main
* Description    : Main program.

STM32F103C8T6 ����mlx90614�������ģ�飬��ȡ�¶���ʾ��TM1637�������

PB6 SCL; 
PB7 SDA;
PA3�������CLK;
PB10�������DIO
****************************************************************************/
int main()
{ 
	unsigned char chi1,chi2,ch3,ch4;//����ܵ�1234λ
	int i,a,b;//��������
	double temp;//�¶ȱ���������
	SMBus_Init();//��ʼ��
	TM1637_Init();//����ܳ�ʼ��
	TM1637_NixieTubeDisplay();//�������ʾ����Ҫ������һ�Ρ�
	
	while(1)
	{
		temp=SMBus_ReadTemp();  //��ȡ�¶�
		a = (int)(temp*10)%10;//С�����һλ
		b = (int)(temp*100)%10;//С������λ
		i=(int)temp;//ȡ�� 
		chi1=i/10%10;//ʮλ
	  chi2=i/1%10;//��λ
		ch3=a;
		ch4=b;
	  TM1637_mhDisplay(chi1,chi2,ch3,ch4);//��ð����������С��
	  delay_ms (1000);
		/*
    i=ceil(temp); //����ȡ����ֻ��ʾ����ֵ
		chi1=i/1000%10;/
		chi2=i/100%10;//
		ch3=i/10%10;//
		ch4=i/1%10;//
		TM1637_Display(chi1,chi2,ch3,ch4);
		
		*/		
	}			
}



