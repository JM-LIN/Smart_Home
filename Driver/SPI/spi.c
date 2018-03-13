

#include "spi.h"
#include "Systick.h"
#include "flash.h"


/////CS - SDA---------PA4
/////SCK     ---------PA5
/////MOSI    ---------PA6
/////MISO    ---------PA7
/////RST     ---------PF0


///////////////////////////////
///////*RC522��ʼ��**//////////
void SPI1_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	//PA  SPI1ʹ��
	GPIO_InitTypeDef GPIO_InitStructure;//IO�ڽṹ��Ķ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SPI1, DISABLE);                           // ��ʱ��1��ȫ��ӳ��
		

	//PA5/6/7�ڽṹ���ʼ���ֱ�ΪSCK��MOSI��MISO
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	
	
	//PA4��ʼ��CS
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	
	
//		//PF0�ڽṹ���ʼ��RST
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	


	//����SPI1�Ĺ���ģʽ
	SPI_InitStructure.SPI_Direction  = SPI_Direction_2Lines_FullDuplex;//˫��ģʽ
	SPI_InitStructure.SPI_Mode       = SPI_Mode_Master;//����ģʽ
	SPI_InitStructure.SPI_DataSize   = SPI_DataSize_8b;//8λ����
	SPI_InitStructure.SPI_CPOL       = SPI_CPOL_Low;//ѡ���˴���ʱ�ӵ���̬ʱ�����ո�
	SPI_InitStructure.SPI_CPHA       = SPI_CPHA_1Edge;//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS        = SPI_NSS_Soft;//�������
	SPI_InitStructure.SPI_BaudRatePrescaler   = SPI_BaudRatePrescaler_8;//Ԥ��Ƶ256
	SPI_InitStructure.SPI_FirstBit            = SPI_FirstBit_MSB;//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial       = 7;//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1,&SPI_InitStructure);//SPI��ʼ��
	
	SPI_Cmd(SPI1,ENABLE);//SPIʹ��
	
}



/*********RC522��ʼ��*************/
void SPI2_Init(void)
{	
	SPI_InitTypeDef SPI_InitStructure;	
	
	//PB�ڵ�ʹ��
	GPIO_InitTypeDef GPIO_InitStructure;//IO�ڽṹ��Ķ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	
	//PB13/14/15�ڽṹ���ʼ���ֱ�ΪSCK��MOSI��MISO
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

    //PB12��ʼ��CS
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
    
    W25QXX_CS_OFF();				        //SPI FLASH��ѡ��
	
		
	//����SPI2�Ĺ���ģʽ
	SPI_InitStructure.SPI_Direction  = SPI_Direction_2Lines_FullDuplex;//˫��ģʽ
	SPI_InitStructure.SPI_Mode       = SPI_Mode_Master;//����ģʽ
	SPI_InitStructure.SPI_DataSize   = SPI_DataSize_8b;//8λ����
	SPI_InitStructure.SPI_CPOL       = SPI_CPOL_High;//ѡ���˴���ʱ�ӵ���̬ʱ�����ո�
	SPI_InitStructure.SPI_CPHA       = SPI_CPHA_2Edge;//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS        = SPI_NSS_Soft;//�������
	SPI_InitStructure.SPI_BaudRatePrescaler   = SPI_BaudRatePrescaler_256;//Ԥ��Ƶ256
	SPI_InitStructure.SPI_FirstBit            = SPI_FirstBit_MSB;//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial       = 7;      //CRCֵ����Ķ���ʽ
	SPI_Init(SPI2,&SPI_InitStructure);                  //SPI��ʼ��
	
	SPI_Cmd(SPI2,ENABLE);//SPIʹ��
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
	return SPI_I2S_ReceiveData(SPI1);//����ͨ��SPIx������ܵ�����
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
	return SPI_I2S_ReceiveData(SPI2);//����ͨ��SPIx������ܵ�����
}



void SPI2_SetSpeed(u8 SpeedSet)
{
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler   = SpeedSet;//Ԥ��
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
}





