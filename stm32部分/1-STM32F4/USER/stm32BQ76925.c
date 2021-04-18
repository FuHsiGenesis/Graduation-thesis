#include "stm32BQ76925.h" 
#include "delay.h" 
#include "myiic.h"
#include "stm32f4xx.h"
#include "lcd.h"
#include "24cxx.h"
#include "oled.h"
#include "voltage.h"
#include "adc.h"

extern signed char offset_corr[7];
extern signed char gain_corr[7];
vu32  vref_corr=0;


const u8 Config_reg[][2]={
	{0x03,0x00},//CONFIG_1   ���÷Ŵ��� �Լ�SENSEN/SENSEP respect to VSS
	{0x04,0x01},//CONGIG_2   0x01��׼��ѹ3.0  ��������2   0x00��׼��ѹ1.5 ��������1  REF_SEL 
	{0x05,0x0D},//POWER_CTL  enable Current comparator and amplifier ,Cell amplifier ,Voltage reference
	{0x00,0x00},//STATUS     
  {0x02,0x00},//BAL_CTL
	{0x01,0x10},//CELL_CTL   ���ò���ĳƬ��ص�ѹ
};

void BQ76925Init(void)
{
	u8 i=0;
	double VREF;
	IIC_Init();//IIC��ʼ��

	for(i=0;i<6;i++)
	{
		BQ76925_WriteOneByte(Config_reg[i][0],Config_reg[i][1]);
		delay_ms(32);
	} 
	CorrInit();//��ȡ�Ĵ�����ֵ
	 
	/*********�����׼��ѹ***************/
	 if(REF_SEL)  
			 VREF=3;
	 else      
			 VREF=1.5;
  vref_corr=VREF*(1000+gain_corr[0])+offset_corr[0];
	 
}



//��BQ76925���ƶ���ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 test1=0;
u8 BQ76925_ReadOneByte(u16 ReadAddr)
{			
	  u8 temp=0,address;
	
    IIC_Start(); 
	  SDA_OUT(); 
		IIC_Send_Byte((((I2C_GROUP_ADDR<<3)+ReadAddr)<<1)+Read_IIC);	    //���Ͷ�����

	  IIC_Wait_Ack();
	  SDA_IN();
    temp=IIC_Read_Byte(0);		   
    
    IIC_Stop();//����һ��ֹͣ����	 
 delay_ms(10);	 	
	return temp;
}

//��BQ76925���ƶ�λ������һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void BQ76925_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	  SDA_OUT();  
		IIC_Send_Byte((((I2C_GROUP_ADDR<<3)+WriteAddr)<<1)+Write_IIC);	    //����д����
		if(IIC_Wait_Ack())
		{
			while(1);//error
		}
	  IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	  if(IIC_Wait_Ack())
			{
			while(1);//error
		}
    IIC_Stop();//����һ��ֹͣ���� 
	  delay_ms(12);	 
}


//��ʾ����
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  







void SHOWFIVE(void)
{
	u16 value1,value2,value3,value4,value5=0;
	OLED_ShowString(20,1,"SHOW TIME 1");

	value1=value2=value3=value4=value5=100;
	
	OLED_ShowString(0,3,"VREF_CAL:");
	   OLED_ShowNum(70,3,value1,3);
	OLED_ShowString(0,4,"VC1_CAL:");
	   OLED_ShowNum(70,4,value2,3);
	OLED_ShowString(0,5,"VC2_CAL:");
	   OLED_ShowNum(70,5,value3,3);
	OLED_ShowString(0,6,"VC3_CAL:");
	   OLED_ShowNum(70,6,value4,3);
	OLED_ShowString(0,7,"VC4_CAL:");
	   OLED_ShowNum(70,7,value5,3);

}


void SHOWANOTHERFIVE(void)
{
	u16 value1,value2,value3,value4,value5=0;
	value1=BQ76925_ReadOneByte(VC5_CAL);
	value2=BQ76925_ReadOneByte(VC6_CAL);
	value3=BQ76925_ReadOneByte(VC_CAL_EXT_1);
	value4=BQ76925_ReadOneByte(VC_CAL_EXT_2);
	value5=BQ76925_ReadOneByte(VREF_CAL_EXT);
	OLED_ShowString(20,1,"SHOW TIME 2");
#ifdef  IIC
	OLED_ShowString(0,3,"VC5_CAL:");
	   OLED_ShowNum(70,3,VC5_CAL,3);
	OLED_ShowString(0,4,"VC6_CAL:");
	   OLED_ShowNum(70,4,VC6_CAL,3);
	OLED_ShowString(0,5,"VC_CAL_EXT_1:");
	   OLED_ShowNum(70,5,VC_CAL_EXT_1,3);
	OLED_ShowString(0,6,"VC_CAL_EXT_2:");
	   OLED_ShowNum(70,6,VC_CAL_EXT_2,3);
	OLED_ShowString(0,7,"VREF_CAL_EXT:");
	   OLED_ShowNum(80,7,VREF_CAL_EXT,3);
#else
	value1=value2=value3=value4=value5=200;
	OLED_ShowString(0,3,"VC5_CAL:");
	   OLED_ShowNum(70,3,value1,3);
	OLED_ShowString(0,4,"VC6_CAL:");
	   OLED_ShowNum(70,4,value2,3);
	OLED_ShowString(0,5,"VC_CAL_EXT_1:");
	   OLED_ShowNum(80,5,value3,3);
	OLED_ShowString(0,6,"VC_CAL_EXT_2:");
	   OLED_ShowNum(80,6,value4,3);
	OLED_ShowString(0,7,"VREF_CAL_EXT:");
	   OLED_ShowNum(80,7,value5,3);
#endif


}



