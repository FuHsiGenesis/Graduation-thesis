/******************************Copyright (c)***********************************
*                Jiangsu Zhihai Electronic Technology Co., Ltd.
*                      Research & Development Department
*
*													www.smarthealth-tech.com
*
-------------------------------------------------------------------------------
* @file    TM1637.h
* @author  GU DONGDONG
* @date    2015-11-25  
*******************************************************************************/
#include "TM1637.h"
#include "systick.h"

/** Write multiple bits in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 */
extern  unsigned char chi1,chi2,ch3,ch4;//数码管第1234位

unsigned char temp=0xCF;//显示跳动的冒号
unsigned char NumDis[26]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0X6F,\
                          0x77,0x7C,0x39,0x5E,0x79,0x71,\
                          0x08,0x48,0x49,\
                          0x76,0x38,0x0E,0x50,0x37,0x00,0x73};//0123456789ABCDEF一二三HLJRS无P

unsigned char chi1,chi2,ch3,ch4;
											
void TM1637_Delay_us(unsigned  int Nus)
{
 for(;Nus>0;Nus--)
	{
	  __nop();
	}
}

void TM1637_Start(void)
{
	TM1637_DIO_OUT(); 
  TM1637_CLK = 1;
	TM1637_DIO = 1;
	TM1637_Delay_us(2);
	TM1637_DIO = 0;
}
void TM1637_Ack(void)
{
	TM1637_DIO_IN(); 
  TM1637_CLK = 0;
	TM1637_Delay_us(5);
	while(TM1637_READ_DIO);
	TM1637_CLK = 1;
	TM1637_Delay_us(2);
	TM1637_CLK = 0;
}
void TM1637_Stop(void)
{
	TM1637_DIO_OUT(); 
	TM1637_CLK = 0;
	TM1637_Delay_us(2);
	TM1637_DIO = 0;
	TM1637_Delay_us(2);
	TM1637_CLK = 1;
	TM1637_Delay_us(2);
	TM1637_DIO = 1;
}
void TM1637_WriteByte(unsigned char oneByte)
{
 unsigned char i;
	TM1637_DIO_OUT(); 
	for(i=0;i<8;i++)
	{
	 TM1637_CLK = 0;
		if(oneByte&0x01)
		{
			TM1637_DIO = 1;
		}
		else
		{
			TM1637_DIO = 0;
		}
		TM1637_Delay_us(3);
		oneByte=oneByte>>1;
		TM1637_CLK = 1;
		TM1637_Delay_us(3);
	}
}

unsigned char TM1637_ScanKey(void)
{
	unsigned char reKey,i;
	TM1637_Start();
	TM1637_WriteByte(0x42);
	TM1637_Ack();
	TM1637_DIO = 1;
	TM1637_DIO_IN(); 
	for(i=0;i<8;i++)
	{
	  TM1637_CLK = 0;
		reKey=reKey>>1;
		TM1637_Delay_us(30);
		TM1637_CLK = 1;
		if(TM1637_READ_DIO)
		{
		  reKey=reKey|0x80;
		}
		else
		{
		  reKey=reKey|0x00;
		}
		TM1637_Delay_us(30);
	}
  TM1637_Ack();
	TM1637_Stop();
	return(reKey);
}
void TM1637_NixieTubeDisplayChar(unsigned char ch,unsigned char p)
{
	if(ch>23)ch=0;//防止数组越界
	TM1637_Start();
	TM1637_WriteByte(0x44);//0x44固定模式有利于控制显示位，0x40则采用显示地址自加模式这里不使用
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Start();
	
	TM1637_WriteByte(0xC0+p);//0X00地址开始显示
	TM1637_Ack();
	
	TM1637_WriteByte(NumDis[ch]);//显示
	
	TM1637_Ack();
	
	
		
	
}
void TM1637_NixieTubeDisplay(void)
{
	
	TM1637_Start();
	TM1637_WriteByte(0x40);//0x40则采用显示地址自加1模式
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0xC0);//0X00地址开始显示
	TM1637_Ack();
		
	TM1637_WriteByte(0x3F);//显示第一位
	TM1637_Ack();
	 
  temp=0x3F;
	TM1637_WriteByte(temp+0x80);//显示第二位和冒号
	TM1637_Ack();
	
	TM1637_WriteByte(0x3F);//显示第三位
	TM1637_Ack();
		
	TM1637_WriteByte(0x3F);//显示第四位
	TM1637_Ack();
		
	//TM1637_WriteByte(0xFF);
	//TM1637_Ack();
		
	//TM1637_WriteByte(0xFF);
	//TM1637_Ack();
	
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0x8f);
	TM1637_Ack();
	TM1637_Stop();
}
void TM1637_Display(unsigned char chi1,unsigned char chi2,unsigned char ch3,unsigned char ch4)//冒号不显示
{
	TM1637_Start();
	TM1637_WriteByte(0x40);//0x40则采用显示地址自加1模式
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0xC0);//0X00地址开始显示
	TM1637_Ack();
		
	TM1637_WriteByte(NumDis[chi1]);//显示第一位
	TM1637_Ack();
	 
  //TM1637_WriteByte(NumDis[chi2]+0x80);//显示2和跳动的冒号
	TM1637_WriteByte(NumDis[chi2]);//显示第二位
	TM1637_Ack();
	
	TM1637_WriteByte(NumDis[ch3]);//显示第三位
	TM1637_Ack();
		
	TM1637_WriteByte(NumDis[ch4]);//显示第四位
	TM1637_Ack();
		
	//TM1637_WriteByte(0xFF);
	//TM1637_Ack();
		
	//TM1637_WriteByte(0xFF);
	//TM1637_Ack();
	
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0x8f);
	TM1637_Ack();
	TM1637_Stop();
}

void TM1637_mhDisplay(unsigned char chi1,unsigned char chi2,unsigned char ch3,unsigned char ch4)//显示冒号
{
	TM1637_Start();
	TM1637_WriteByte(0x40);//0x40则采用显示地址自加1模式
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0xC0);//0X00地址开始显示
	TM1637_Ack();
		
	TM1637_WriteByte(NumDis[chi1]);//显示第一位
	TM1637_Ack();
	 
  TM1637_WriteByte(NumDis[chi2]+0x80);//显示第二位和冒号
	//TM1637_WriteByte(NumDis[chi2]);//显示第二位，不显示冒号
	TM1637_Ack();
	
	TM1637_WriteByte(NumDis[ch3]);//显示第三位
	TM1637_Ack();
		
	TM1637_WriteByte(NumDis[ch4]);//显示第四位
	TM1637_Ack();
		
	//TM1637_WriteByte(0xFF);
	//TM1637_Ack();
		
	//TM1637_WriteByte(0xFF);
	//TM1637_Ack();
	
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0x8f);
	TM1637_Ack();
	TM1637_Stop();
}


void TM1637_setadd(unsigned char chi1,unsigned char chi2)//数值加
{
	
	TM1637_Start();
	TM1637_WriteByte(0x40);//0x40则采用显示地址自加1模式
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0xC0);//0X00地址开始显示
	TM1637_Ack();
		
	TM1637_WriteByte(NumDis[chi1]);//显示第一位
	TM1637_Ack();
	 
  TM1637_WriteByte(NumDis[chi2]+0x80);//显示2和跳动的冒号
	TM1637_Ack();
	
	//TM1637_WriteByte(NumDis[ch3]);//显示第三位
	//TM1637_Ack();
		
	//TM1637_WriteByte(NumDis[ch4]);//显示第四位
	//TM1637_Ack();
		
	//TM1637_WriteByte(0xFF);
	//TM1637_Ack();
		
	//TM1637_WriteByte(0xFF);
	//TM1637_Ack();
	
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0x8f);
	TM1637_Ack();
	TM1637_Stop();
}



void TM1637_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PB10:DIO 	数码管针脚			  
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
 GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//PA3:CLK数码管针脚	  
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
 GPIO_Init(GPIOA, &GPIO_InitStructure);					  

	TM1637_VCC=1;
}
 


