#include "voltage.h"
#include "sys.h"
#include "stm32BQ76925.h" 
#include "adc.h"
#include "oled.h"
#include "delay.h"


signed char offset_corr[7]={0};
signed char gain_corr[7]={0};
extern vu32  vref_corr;
extern u16 adc_sensen;


//��ֻ���Ĵ����е��������ȡ���� 
void CorrInit()   
{
	u8 i;
	u8 temp_corr;
	for(i=0;i<7;i++)
	{
		temp_corr=BQ76925_ReadOneByte(VREF_CAL|i);
		offset_corr[i]=temp_corr>>4;
		gain_corr[i]=temp_corr&0x0F;
	}
	
	temp_corr=BQ76925_ReadOneByte(VREF_CAL_EXT);
	offset_corr[0] |=(((temp_corr&0x06)<<3)^0x20)-0x20;
	gain_corr[0]   |=(((temp_corr&0x01)<<4)^0x10)-0x10;
	
	temp_corr=BQ76925_ReadOneByte(VC_CAL_EXT_1);
	offset_corr[1] |=(((temp_corr&0x80)>>3)^0x10)-0x10;
	gain_corr[1]   |=(((temp_corr&0x40)>>2)^0x10)-0x10;
	
	offset_corr[2] |=(((temp_corr&0x20)>>1)^0x10)-0x10;
	gain_corr[2]   |=((temp_corr&0x10)^0x10)-0x10;
	
	temp_corr=BQ76925_ReadOneByte(VC_CAL_EXT_2);
	offset_corr[3] |=(((temp_corr&0x80)>>3)^0x10)-0x10;
	gain_corr[3]   |=(((temp_corr&0x40)>>2)^0x10)-0x10;
	
	offset_corr[4] |=(((temp_corr&0x20)>>1)^0x10)-0x10;
	gain_corr[4]   |=((temp_corr&0x10)^0x10)-0x10;
	
	offset_corr[5] |=(((temp_corr&0x08)<<1)^0x10)-0x10;
	gain_corr[5]   |=(((temp_corr&0x04)<<2)^0x10)-0x10;

  offset_corr[6] |=(((temp_corr&0x02)<<3)^0x10)-0x10;
	gain_corr[6]   |=(((temp_corr&0x01)<<4)^0x10)-0x10;
  
}



double GET_Current(void)
{
	
	double current=0;
  long long int temp=0;
  static unsigned int adc=0;
  u8  VIOUT_G=0;
	
	adc=VIOUT;//��ȡSENSEP �Ĳ���ֵ
	
	//�Ŵ���
	if(I_Gain)  
		 VIOUT_G=8; 
	else  
		 VIOUT_G=4;

	//SENSEN==SENSEP ʱ����ʾû�е���  ���㲹��
	if(adc>adc_sensen)
	{
		temp=adc-adc_sensen; 
		temp=temp/VIOUT_G;
	}
	else if(adc==adc_sensen)
	       temp=adc;
		   else
			{
		   temp=adc_sensen-adc; 
		   temp=temp/VIOUT_G;
	     }

	temp=temp*vref_corr;
	current=temp/Full_Scale/1e3; 
  current=current/0.05;
	return current;
}

//////////////////
//��ȡÿһƬ��ص�ѹ
//��BQ76925 д�� ��ȡĳƬ��ص�ѹ
//�ӳ�32ms ȷ�����ģ�ͨ����ȡ��ȷֵ
////////////////
double GET_VCN(u8 n)
{
	double vcn=0;
	double G_vout=0;
	long long int temp_V=0;
	double VREF;
	
	BQ76925_WriteOneByte(0x01,0x10|(n-1)); 
	delay_ms(32);
   //��׼��ѹ
	if(REF_SEL)  
	{
		G_vout=0.6;
		VREF=3;
	}
	else    
	{
		G_vout=0.3;
    VREF=1.5;
	}

    //�����ѹֵ
    temp_V=(VCOUT*vref_corr+Full_Scale*offset_corr[n]);
	  temp_V=temp_V*(1000+gain_corr[n]);
	  vcn=((temp_V/4096)/1e6);
		vcn=vcn/G_vout;
	  return vcn;
}


//�ãϣ̣ţ���ʾ��ص�ѹ
void OLED_SHOW_VOLTAGE(void)
{
	  double test_vout_1,test_vout_2,test_vout_3,test_vout_4,test_vout_5,test_vout_6;
	  double test_vout_all,test_vout_all_2,test_vout_all_3,test_vout_all_4,test_vout_all_5;
	
		 test_vout_1=GET_VCN(1); //��ȡ��һƬ��ѹֵ
     test_vout_2=GET_VCN(2);
		 test_vout_3=GET_VCN(3);
		 test_vout_4=GET_VCN(4);
		 test_vout_5=GET_VCN(5);
		 test_vout_6=GET_VCN(6);
	   test_vout_all=test_vout_1+test_vout_2+test_vout_3;
		 test_vout_all=test_vout_all+test_vout_4+test_vout_5+test_vout_6;
		 test_vout_all_2=test_vout_1+test_vout_2;
		 test_vout_all_3=test_vout_all_2+test_vout_3;
		 test_vout_all_4=test_vout_all_3+test_vout_4;
		 test_vout_all_5=test_vout_all_4+test_vout_5;
		
	/**********�ֱ���ʾÿһƬ��ѹֵ***************/
		  OLED_FloatNumber(0,0,test_vout_all);
		  OLED_FloatNumber(0,2,test_vout_1);
		  OLED_FloatNumber(0,3,test_vout_2);
	    OLED_FloatNumber(0,4,test_vout_3);
	    OLED_FloatNumber(0,5,test_vout_4);
	    OLED_FloatNumber(0,6,test_vout_5);
			OLED_FloatNumber(0,7,test_vout_6);
			/***********************************************/
			OLED_FloatNumber(60,3,test_vout_all_2);
			OLED_FloatNumber(60,4,test_vout_all_3);
			OLED_FloatNumber(60,5,test_vout_all_4);
		  OLED_FloatNumber(60,6,test_vout_all_5);
	
	    
}

