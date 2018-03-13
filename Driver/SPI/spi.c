

#include "spi.h"
#include "Systick.h"
#include "flash.h"


/////CS - SDA---------PA4
/////SCK     ---------PA5
/////MOSI    ---------PA6
/////MISO    ---------PA7
/////RST     ---------PF0


///////////////////////////////
///////*RC522初始化**//////////
void SPI1_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	//PA  SPI1使能
	GPIO_InitTypeDef GPIO_InitStructure;//IO口结构体的定义
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SPI1, DISABLE);                           // 定时器1完全重映射
		

	//PA5/6/7口结构体初始化分别为SCK、MOSI、MISO
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	
	
	//PA4初始化CS
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	
	
//		//PF0口结构体初始化RST
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	


	//设置SPI1的工作模式
	SPI_InitStructure.SPI_Direction  = SPI_Direction_2Lines_FullDuplex;//双工模式
	SPI_InitStructure.SPI_Mode       = SPI_Mode_Master;//主机模式
	SPI_InitStructure.SPI_DataSize   = SPI_DataSize_8b;//8位数据
	SPI_InitStructure.SPI_CPOL       = SPI_CPOL_Low;//选择了串行时钟的稳态时钟悬空高
	SPI_InitStructure.SPI_CPHA       = SPI_CPHA_1Edge;//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS        = SPI_NSS_Soft;//软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler   = SPI_BaudRatePrescaler_8;//预分频256
	SPI_InitStructure.SPI_FirstBit            = SPI_FirstBit_MSB;//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial       = 7;//CRC值计算的多项式
	SPI_Init(SPI1,&SPI_InitStructure);//SPI初始化
	
	SPI_Cmd(SPI1,ENABLE);//SPI使能
	
}



/*********RC522初始化*************/
void SPI2_Init(void)
{	
	SPI_InitTypeDef SPI_InitStructure;	
	
	//PB口的使能
	GPIO_InitTypeDef GPIO_InitStructure;//IO口结构体的定义
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	
	//PB13/14/15口结构体初始化分别为SCK、MOSI、MISO
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

    //PB12初始化CS
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
    
    W25QXX_CS_OFF();				        //SPI FLASH不选中
	
		
	//设置SPI2的工作模式
	SPI_InitStructure.SPI_Direction  = SPI_Direction_2Lines_FullDuplex;//双工模式
	SPI_InitStructure.SPI_Mode       = SPI_Mode_Master;//主机模式
	SPI_InitStructure.SPI_DataSize   = SPI_DataSize_8b;//8位数据
	SPI_InitStructure.SPI_CPOL       = SPI_CPOL_High;//选择了串行时钟的稳态时钟悬空高
	SPI_InitStructure.SPI_CPHA       = SPI_CPHA_2Edge;//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS        = SPI_NSS_Soft;//软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler   = SPI_BaudRatePrescaler_256;//预分频256
	SPI_InitStructure.SPI_FirstBit            = SPI_FirstBit_MSB;//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial       = 7;      //CRC值计算的多项式
	SPI_Init(SPI2,&SPI_InitStructure);                  //SPI初始化
	
	SPI_Cmd(SPI2,ENABLE);//SPI使能
}



u8 SPI1_ReadWriteByte(u8 TxData)
{
	u8 retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry > 200)
			return 0;
	}
	SPI_I2S_SendData(SPI1,TxData);
	retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry > 200)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI1);//返回通过SPIx最近接受的数据
}



u8 SPI2_ReadWriteByte(u8 TxData)
{
	u8 retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry > 200)
			return 0;
	}
	SPI_I2S_SendData(SPI2,TxData);
	retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry > 200)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI2);//返回通过SPIx最近接受的数据
}



void SPI2_SetSpeed(u8 SpeedSet)
{
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler   = SpeedSet;//预分
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
}





