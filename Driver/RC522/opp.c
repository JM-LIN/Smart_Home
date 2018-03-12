#include "opp.h"
#include "usart.h"
#include "wifi.h"
#include "Systick.h"
#include "rc522.h"
#include "flash.h"
#include "time.h"
#include "WIFI_BufferPool.h"

u32 FLASH_SIZE_opp = 16*1024*1024;//flash的总空间大小
uint8_t ResetNumber[4] = {0x00,0x00,0x00,0x00};//重置卡号
char ATOrder[16]="AT+CIPSEND=0,";

void get(void)
{
    uint8_t Rx1_Buffer_get[3];    //缓存区
    uint8_t Rx2_Buffer_get[80];  //卡号缓存区
	uint8_t l,j,i = 1;
	int num;

	int week;
//	PC_Usart ((unsigned char*)"进入发送阶段\r");
	for (l=0;l<3;l++)
    {
		if (l==0)
			week = weekone;
		else if (l==1)
			week = weektwo;
		else if (l==2)
			week = weekthe;
		W25QXX_Read(&Rx1_Buffer_get[0], FLASH_SIZE_opp - week + day(i) ,sizeof(Rx1_Buffer_get[0]));
		delay_ms (100);
		if(Rx1_Buffer_get[0]==0)
			continue;
		delay_ms (1000);
		for (i=1;i<=7;i++)
		{   
			num = 0;
			W25QXX_Read(&Rx1_Buffer_get[0], FLASH_SIZE_opp - week + day(i) ,sizeof(Rx1_Buffer_get[0]));  //月

			W25QXX_Read(&Rx1_Buffer_get[1], FLASH_SIZE_opp - week + day(i) + 1 ,sizeof(Rx1_Buffer_get[1]));  //日

			W25QXX_Read(&Rx1_Buffer_get[2], FLASH_SIZE_opp - week + day(i) + 2 ,sizeof(Rx1_Buffer_get[2]));  //人数
			delay_ms (350);
			ESP8266_Cmd ("AT+CIPSEND=0,3",NULL,NULL,1000);   delay_us(255); delay_us(255);delay_us(255); delay_us(255);
			ESP8266_SendString(DISABLE,(unsigned char*)Rx1_Buffer_get,3,0);   delay_us(255); delay_us(255);delay_us(255); delay_us(255);
			delay_ms (250);
			for(j=0;j<Rx1_Buffer_get[2];j++)      //通过已知人数判断发送多少个卡号
			{
				W25QXX_Read(&Rx2_Buffer_get[num], FLASH_SIZE_opp - week + day(i) + 3 + 4*j + 0 ,sizeof(Rx2_Buffer_get[0]));
				W25QXX_Read(&Rx2_Buffer_get[++num], FLASH_SIZE_opp - week + day(i) + 3 + 4*j + 1 ,sizeof(Rx2_Buffer_get[0]));
				W25QXX_Read(&Rx2_Buffer_get[++num], FLASH_SIZE_opp - week + day(i) + 3 + 4*j + 2 ,sizeof(Rx2_Buffer_get[0]));
				W25QXX_Read(&Rx2_Buffer_get[++num], FLASH_SIZE_opp - week + day(i) + 3 + 4*j + 3 ,sizeof(Rx2_Buffer_get[0]));
				num++;
			}
			BuildATOrder (num);									
			delay_ms (350);
			ESP8266_Cmd (ATOrder,NULL,NULL,1000);   delay_us(255); delay_us(255);delay_us(255); delay_us(255);
			ESP8266_SendString(DISABLE,(unsigned char*)Rx2_Buffer_get,num,0);	 delay_us(255); delay_us(255);delay_us(255); delay_ms(1500);		
		}										
	}
//	PC_Usart ((unsigned char*)"发送结束\r");
}

void del(uint8_t ch3[4])
{
	uint8_t ID[4];
	uint8_t temp1;
	uint8_t temp2;
	uint8_t Read_ID[4];
	int i;

	//卡号处理
	ID[0] = ch3[0]; 
	ID[1] = ch3[1];
	ID[2] = ch3[2];
	ID[3] = ch3[3];

	W25QXX_Read(&temp1,FLASH_SIZE_opp - 1000,sizeof(temp1));//读出人数
	temp2=temp1;          
	for(i=0;i<temp1;i++)     //循环查找卡号
	{		
		W25QXX_Read(Read_ID,FLASH_SIZE_opp- 40000+i*4,4);

		if (Buffercmp(ID,Read_ID,4))
		{
//			PC_Usart ((unsigned char*)"找到卡号！\r");
			W25QXX_Write(ResetNumber,FLASH_SIZE_opp- 40000+i*4,4);
			temp2--;           //改变人数
			break;
		}		 
	}
	W25QXX_Write(&temp2,FLASH_SIZE_opp - 1000,sizeof(temp2));//写回人数
	WIFI_SetFlowSize(3);     //重置溢出值

//	PC_Usart ((unsigned char*)"\r");
	W25QXX_Read(&temp2,FLASH_SIZE_opp - 1000,sizeof(temp2));
//	PC_Usart((unsigned char*)"%d\r",temp2);

	//检查卡号是否被删除
	for(i=0;i<temp2;i++)    
	{		
		W25QXX_Read(Read_ID,FLASH_SIZE_opp - 40000+i*4,4);
//		PC_Usart((unsigned char*)"%x%x%x%x\r",Read_ID[0],Read_ID[1],Read_ID[2],Read_ID[3]);
	}
}

void add(void)
{
	uint8_t Card_Type[2];
	uint8_t ID[4];
	uint8_t status1;
	uint8_t temp1;
	uint8_t i;

	PC_Usart((unsigned char*)"测试模式\r");

	/*       //读卡//       */
	while (MI_OK!=PcdRequest(0x52, Card_Type)); //一直循环直到读到卡   
	PC_Usart ((unsigned char*)"读到卡");
	status1 = PcdAnticoll(ID);//防冲撞
	status1 = PcdSelect(ID);  //选卡
	PcdHalt();

	/*       //存储//        */
	if(status1==MI_OK)//選卡成功
	{
		W25QXX_Read(&temp1,FLASH_SIZE_opp - 1000,sizeof(temp1));//读出人数
		PC_Usart ((unsigned char*)"%d\r",temp1);				
		for(i = temp1;i<temp1+1;i++)   //写入
		{
			W25QXX_Write((u8*)ID,FLASH_SIZE_opp - 40000 + temp1*4 , 4);
		}				
	}
	W25QXX_Read(ID,FLASH_SIZE_opp - 40000 + temp1*4 , 4);
	temp1 = temp1 + 1;  //卡数加+1
	W25QXX_Write(&temp1,FLASH_SIZE_opp - 1000,sizeof(temp1));	

			/*       //发送//       */
	ESP8266_Cmd ("AT+CIPSEND=0,4",NULL,NULL,1000);   delay_us(255); delay_us(255);  //发送数据前必要的指令
	ESP8266_SendString (DISABLE,(unsigned char*)ID,4,0);     //发送数据
	PC_Usart ((unsigned char*)"发送结束\r");				
}


void BuildATOrder (int data)
{
	char ten,one;
	one = data%10 + '0';
	ten = data/10 + '0';

	ATOrder[13] = ten;
	ATOrder[14] = one;
}
	

char Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)  //字符串比较
{
	while(BufferLength--)
	{
		if(*pBuffer1 != *pBuffer2)
		{
		  return 0;
		}

		pBuffer1++;
		pBuffer2++;
	}
	return 1;
}

void WriteBufferArea (void)
{
    uint8_t Rx1_Buffer_get[3] = {0x00,0x00,7};    //缓存区
    uint8_t Rx2_Buffer_get[4] = {0xd7,0x66,0xa4,0x7c};  //卡号缓存区
	int l,i,j,week;
	
	
	for (l=0;l<3;l++)
	{
		if (l==0)
			week = weekone;
		else if (l==1)
		{	   
			week = weektwo;
			Rx1_Buffer_get[0] = 4;
			Rx1_Buffer_get[1] = 5;					
			Rx1_Buffer_get[2] = 4;
			Rx2_Buffer_get[1] = 0xd7;
		}
		else if (l==2)
		{    
			week = weekthe;
			Rx1_Buffer_get[0] = 4;
			Rx1_Buffer_get[1] = 12;
			Rx1_Buffer_get[2] = 3;
			Rx2_Buffer_get[2] = 0xd7;
		}

		for (i=1;i<=7;i++)
		{
			W25QXX_Write(&Rx1_Buffer_get[0], FLASH_SIZE_opp - week + day(i) ,sizeof(Rx1_Buffer_get[0]));  //月

			W25QXX_Write(&Rx1_Buffer_get[1], FLASH_SIZE_opp - week + day(i) + 1 ,sizeof(Rx1_Buffer_get[1]));  //日
			Rx1_Buffer_get[1] ++ ;								 
			W25QXX_Write(&Rx1_Buffer_get[2], FLASH_SIZE_opp - week + day(i) + 2 ,sizeof(Rx1_Buffer_get[2]));  //人数
		  
			for(j=0;j<Rx1_Buffer_get[2];j++)      //通过已知人数判断发送多少个卡号
			{
				W25QXX_Write(Rx2_Buffer_get, FLASH_SIZE_opp - week + day(i) + 3 + 4*j + 0 ,4);
			//	W25QXX_Write(&Rx2_Buffer_get[1], FLASH_SIZE_opp - week + day(i) + 3 + 4*j + 1 ,sizeof(Rx2_Buffer_get[1]));
			//	W25QXX_Write(&Rx2_Buffer_get[2], FLASH_SIZE_opp - week + day(i) + 3 + 4*j + 2 ,sizeof(Rx2_Buffer_get[2]));
			//	W25QXX_Write(&Rx2_Buffer_get[3], FLASH_SIZE_opp - week + day(i) + 3 + 4*j + 3 ,sizeof(Rx2_Buffer_get[3]));						
			}						  
		}
	}
}

void text()
{
	uint8_t Rx1_Buffer_get[3];    //缓存区
	uint8_t Rx2_Buffer_get[4];  //卡号缓存区
	int i,j,week,l;
	
	for (l=0;l<3;l++)
	{
		if (l==0)
			week = weekone;
		else if (l==1)
			week = weektwo;
		else if (l==2)
			week = weekthe;

		for (i=1;i<=7;i++)
		{
			W25QXX_Read(&Rx1_Buffer_get[0], FLASH_SIZE_opp - week + day(i) ,sizeof(Rx1_Buffer_get[0]));  //月
//			PC_Usart ((unsigned char*)"%d  ",Rx1_Buffer_get[0]);
			W25QXX_Read(&Rx1_Buffer_get[1], FLASH_SIZE_opp - week + day(i) + 1 ,sizeof(Rx1_Buffer_get[1]));  //日
//			PC_Usart ((unsigned char*)"%d  ",Rx1_Buffer_get[1]);							 
			W25QXX_Read(&Rx1_Buffer_get[2], FLASH_SIZE_opp - week + day(i) + 2 ,sizeof(Rx1_Buffer_get[2]));  //人数
//			PC_Usart ((unsigned char*)"%d  \r",Rx1_Buffer_get[2]);		
			//ESP8266_Cmd ("AT+CIPSEND=0,3",NULL,NULL,1000);   delay_us(255); delay_us(255);
			//ESP8266_SendByteArray  (Rx1_Buffer_get,3,NULL,NULL,1000);

			for(j=0;j<Rx1_Buffer_get[2];j++)      //通过已知人数判断发送多少个卡号
			{
				W25QXX_Read(Rx2_Buffer_get, FLASH_SIZE_opp - week + day(i) + 3 + 4*j  ,4);
//				PC_Usart ((unsigned char*)"%x\r",*((int*)Rx2_Buffer_get));						
			//	ESP8266_Cmd ("AT+CIPSEND=0,4",NULL,NULL,1000);   delay_us(255); delay_us(255);
			//  ESP8266_SendByteArray  (Rx2_Buffer_get,4,NULL,NULL,1000);								 
			}						  
		}
	}
}
