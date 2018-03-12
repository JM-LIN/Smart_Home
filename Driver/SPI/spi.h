#ifndef __SPI_H_
#define __SPI_H_

#include "stm32f10x.h"

/////CS - SDA---------PB12
/////SCK     ---------PB13
/////MOSI    ---------PB14
/////MISO    ---------PB15
/////RST     ---------PF1
//#define  GPIO_SetBits(GPIOB,GPIO_Pin_12)    RC522_SDA_SET //PB12Êä³ö1
//#define  GPIO_ResetBits(GPIOB,GPIO_Pin_12)  RC522_SDA_RESET //PB12Êä³ö0
//#define  GPIO_SetBits(GPIOF,GPIO_Pin_1)     RC522_RST_SET //PB12Êä³ö1


void SPI1_Init(void);//FLASH
void SPI2_Init(void);//RC522

u8 SPI1_ReadWriteByte(u8 TxData);
u8 SPI2_ReadWriteByte(u8 TxData);

void SPI2_SetSpeed(u8 SpeedSet);//FLASH
//int32_t SPI1_ReadByte(uint8_t *p_RxData,uint32_t readDataNum);
//int32_t SPI1_WriteByte(uint8_t *p_TxData,uint32_t sendDataNum);

#endif

