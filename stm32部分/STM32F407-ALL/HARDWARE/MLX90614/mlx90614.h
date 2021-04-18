/*******************************************************************************
* �ļ���		: mlx90614.h
* ��  ��	: 
* ��  ��	: 
* ��  ��	: 2013-08-07
* ��  ��	: mlx90614����
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MLX90614_H
#define __MLX90614_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Delay(u16);
void SMBus_Init(void);
u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
int SMBus_ReadTemp(void); //��ȡ�¶�ֵ
void SMBus_SDA_OUTMode(void);
void SMBus_SDA_INMode(void);

#endif

/*********************************END OF FILE*********************************/
