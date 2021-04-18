#include "adc.h"
#include "delay.h"		 
#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
u16 ADC_Data[1][2]={0};

//ADC1
//PC5 --channel 15  VCOUT
//PB0 --channel 8   VIOUT
//��ʼ��ADC			 DMA												   
void  Adc_dma_Init(void)
{    
  GPIO_InitTypeDef      GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
	
	/************************************************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;    //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&ADC1->DR; 	//DMA�����ַ		 = & ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Data;		//DMA�洢��ַ	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //���赽�洢��           
	DMA_InitStructure.DMA_BufferSize =1*2;   //���ݴ�����                      	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//16 bit data����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;        //�洢��16 bit data  32??MCU?1?half-word?16 bits
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//���δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	
	
  /* GPIO��ʼ��*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��  
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 


  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2; //DMA
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 2;//ת��3��ͨ��
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	//�趨ָ����ADCͨ���飬����ת��˳��Ͳ���ʱ��
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1, ADC_SampleTime_56Cycles );
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15, 1, ADC_SampleTime_56Cycles );
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8, 2, ADC_SampleTime_56Cycles );
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
		/* Enable ADCx DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	ADC_Cmd(ADC1, ENABLE);//����ADת����	
   ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	DMA_Cmd(DMA2_Stream0, ENABLE);
}			  

//���ADCֵ
//ch: @ref 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 Get_Adc(u8 ADC_Channe1)   
{
	   u16 value;
	  
	   
		switch(ADC_Channe1)
		{
			case ADC_Channel_8: 
						value=ADC_Data[0][1];
						break;
			case ADC_Channel_15:  
						value=ADC_Data[0][0];
						break;
		}
		return value;
}

//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 


