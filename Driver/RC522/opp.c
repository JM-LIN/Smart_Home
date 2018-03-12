#include "opp.h"
#include "usart.h"
#include "wifi.h"
#include "Systick.h"
#include "rc522.h"
#include "flash.h"
#include "time.h"
#include "WIFI_BufferPool.h"

u32 FLASH_SIZE_opp = 16*1024*1024;//flash���ܿռ��С
uint8_t ResetNumber[4] = {0x00,0x00,0x00,0x00};//���ÿ���
char ATOrder[16]="AT+CIPSEND=0,";

void get(void)
{
    uint8_t Rx1_Buffer_get[3];    //������
    uint8_t Rx2_Buffer_get[80];  //���Ż�����
	uint8_t l,j,i = 1;
	int num;

	int week;
//	PC_Usart ((unsigned char*)"���뷢�ͽ׶�\r");
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
			W25QXX_Read(&Rx1_Buffer_get[0], FLASH_SIZE_opp - week + day(i) ,sizeof(Rx1_Buffer_get[0]));  //��

			W25QXX_Read(&Rx1_Buffer_get[1], FLASH_SIZE_opp - week + day(i) + 1 ,sizeof(Rx1_Buffer_get[1]));  //��

			W25QXX_Read(&Rx1_Buffer_get[2], FLASH_SIZE_opp - week + day(i) + 2 ,sizeof(Rx1_Buffer_get[2]));  //����
			delay_ms (350);
			ESP8266_Cmd ("AT+CIPSEND=0,3",NULL,NULL,1000);   delay_us(255); delay_us(255);delay_us(255); delay_us(255);
			ESP8266_SendString(DISABLE,(unsigned char*)Rx1_Buffer_get,3,0);   delay_us(255); delay_us(255);delay_us(255); delay_us(255);
			delay_ms (250);
			for(j=0;j<Rx1_Buffer_get[2];j++)      //ͨ����֪�����жϷ��Ͷ��ٸ�����
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
//	PC_Usart ((unsigned char*)"���ͽ���\r");
}

void del(uint8_t ch3[4])
{
	uint8_t ID[4];
	uint8_t temp1;
	uint8_t temp2;
	uint8_t Read_ID[4];
	int i;

	//���Ŵ���
	ID[0] = ch3[0]; 
	ID[1] = ch3[1];
	ID[2] = ch3[2];
	ID[3] = ch3[3];

	W25QXX_Read(&temp1,FLASH_SIZE_opp - 1000,sizeof(temp1));//��������
	temp2=temp1;          
	for(i=0;i<temp1;i++)     //ѭ�����ҿ���
	{		
		W25QXX_Read(Read_ID,FLASH_SIZE_opp- 40000+i*4,4);

		if (Buffercmp(ID,Read_ID,4))
		{
//			PC_Usart ((unsigned char*)"�ҵ����ţ�\r");
			W25QXX_Write(ResetNumber,FLASH_SIZE_opp- 40000+i*4,4);
			temp2--;           //�ı�����
			break;
		}		 
	}
	W25QXX_Write(&temp2,FLASH_SIZE_opp - 1000,sizeof(temp2));//д������
	WIFI_SetFlowSize(3);     //�������ֵ

//	PC_Usart ((unsigned char*)"\r");
	W25QXX_Read(&temp2,FLASH_SIZE_opp - 1000,sizeof(temp2));
//	PC_Usart((unsigned char*)"%d\r",temp2);

	//��鿨���Ƿ�ɾ��
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

	PC_Usart((unsigned char*)"����ģʽ\r");

	/*       //����//       */
	while (MI_OK!=PcdRequest(0x52, Card_Type)); //һֱѭ��ֱ��������   
	PC_Usart ((unsigned char*)"������");
	status1 = PcdAnticoll(ID);//����ײ
	status1 = PcdSelect(ID);  //ѡ��
	PcdHalt();

	/*       //�洢//        */
	if(status1==MI_OK)//�x���ɹ�
	{
		W25QXX_Read(&temp1,FLASH_SIZE_opp - 1000,sizeof(temp1));//��������
		PC_Usart ((unsigned char*)"%d\r",temp1);				
		for(i = temp1;i<temp1+1;i++)   //д��
		{
			W25QXX_Write((u8*)ID,FLASH_SIZE_opp - 40000 + temp1*4 , 4);
		}				
	}
	W25QXX_Read(ID,FLASH_SIZE_opp - 40000 + temp1*4 , 4);
	temp1 = temp1 + 1;  //������+1
	W25QXX_Write(&temp1,FLASH_SIZE_opp - 1000,sizeof(temp1));	

			/*       //����//       */
	ESP8266_Cmd ("AT+CIPSEND=0,4",NULL,NULL,1000);   delay_us(255); delay_us(255);  //��������ǰ��Ҫ��ָ��
	ESP8266_SendString (DISABLE,(unsigned char*)ID,4,0);     //��������
	PC_Usart ((unsigned char*)"���ͽ���\r");				
}


void BuildATOrder (int data)
{
	char ten,one;
	one = data%10 + '0';
	ten = data/10 + '0';

	ATOrder[13] = ten;
	ATOrder[14] = one;
}
	

char Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)  //�ַ����Ƚ�
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
    uint8_t Rx1_Buffer_get[3] = {0x00,0x00,7};    //������
    uint8_t Rx2_Buffer_get[4] = {0xd7,0x66,0xa4,0x7c};  //���Ż�����
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
			W25QXX_Write(&Rx1_Buffer_get[0], FLASH_SIZE_opp - week + day(i) ,sizeof(Rx1_Buffer_get[0]));  //��

			W25QXX_Write(&Rx1_Buffer_get[1], FLASH_SIZE_opp - week + day(i) + 1 ,sizeof(Rx1_Buffer_get[1]));  //��
			Rx1_Buffer_get[1] ++ ;								 
			W25QXX_Write(&Rx1_Buffer_get[2], FLASH_SIZE_opp - week + day(i) + 2 ,sizeof(Rx1_Buffer_get[2]));  //����
		  
			for(j=0;j<Rx1_Buffer_get[2];j++)      //ͨ����֪�����жϷ��Ͷ��ٸ�����
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
	uint8_t Rx1_Buffer_get[3];    //������
	uint8_t Rx2_Buffer_get[4];  //���Ż�����
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
			W25QXX_Read(&Rx1_Buffer_get[0], FLASH_SIZE_opp - week + day(i) ,sizeof(Rx1_Buffer_get[0]));  //��
//			PC_Usart ((unsigned char*)"%d  ",Rx1_Buffer_get[0]);
			W25QXX_Read(&Rx1_Buffer_get[1], FLASH_SIZE_opp - week + day(i) + 1 ,sizeof(Rx1_Buffer_get[1]));  //��
//			PC_Usart ((unsigned char*)"%d  ",Rx1_Buffer_get[1]);							 
			W25QXX_Read(&Rx1_Buffer_get[2], FLASH_SIZE_opp - week + day(i) + 2 ,sizeof(Rx1_Buffer_get[2]));  //����
//			PC_Usart ((unsigned char*)"%d  \r",Rx1_Buffer_get[2]);		
			//ESP8266_Cmd ("AT+CIPSEND=0,3",NULL,NULL,1000);   delay_us(255); delay_us(255);
			//ESP8266_SendByteArray  (Rx1_Buffer_get,3,NULL,NULL,1000);

			for(j=0;j<Rx1_Buffer_get[2];j++)      //ͨ����֪�����жϷ��Ͷ��ٸ�����
			{
				W25QXX_Read(Rx2_Buffer_get, FLASH_SIZE_opp - week + day(i) + 3 + 4*j  ,4);
//				PC_Usart ((unsigned char*)"%x\r",*((int*)Rx2_Buffer_get));						
			//	ESP8266_Cmd ("AT+CIPSEND=0,4",NULL,NULL,1000);   delay_us(255); delay_us(255);
			//  ESP8266_SendByteArray  (Rx2_Buffer_get,4,NULL,NULL,1000);								 
			}						  
		}
	}
}
