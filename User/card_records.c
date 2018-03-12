#include "card_records.h"



unsigned char Card_Type_s[2];	//������
unsigned char Card_ID[4]; 		//����
unsigned char RFID[16];			//���RFID 

u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
uint16_t cardType;

u8 TEXT_Buffer[4];
u8 READ_Buffer[4];

u8 Stu_Card_Id[4];
u8 Read_Stu_Card_Id[4];//����ʹ�ã��������ţ��鿴�Ƿ��д���

__IO u8 CLI_ID; 

u8 KEY_SE;
u8 mode_flag = 2; //= 2;    	//ģʽ��ʶ��


u8 time_Del = 0;	
u8 temp = 0;								
u8 temp1 = 0;						
u8 Del_temp;           			//ɾ�����ű�־
u8 Del_flag = 0;				//ɾ�����ܱ�־
u8 finish_flag;					//ÿ��ģʽ�Ƿ���ɱ�־
u8 status;                   	//�����ɹ���ʶ��
u8 time_flag = 0;
u8 same_id_day_flag = 0;
	
u8 check_ID_flag = 1;        	    // ����Ƿ������ı�־��	


u32 FLASH_SIZE = 16*1024*1024;	    //flash���ܿռ��С

extern _calendar_obj calendar;      //ʱ�ӽṹ�� 
u8 week1 = 0;   				    //ʱ���ʶ��
u8 week2 = 0;
u8 week3 = 0;
u8 day1 = 1;
u8 day2 = 1;
u8 day3 = 1;
u8 sec1  = 0;
u8 sec2  = 0;
u8 sec3  = 0;

u8 read_card_num;   

struct Student_ID_Structure
{
	char name[30];
	unsigned char Stu_Card_ID[4];
}Student_ID[100],Del_Student_ID[100],Read_Day_Stu_ID[100];//�ֱ��ʾ������flash�Ŀ��ţ���ɾ����Ŀ���,�������ǵ���ˢ��������

struct Time_Structure
{
	u8 year1;
	u8 year2;
	u8 month;
	u8 data;
	u8 hour;
	u8 min;
	u8 sec;
}Swiping_Time,Read_Time;


void Swiping_Card(void)
{
    int i = 0;
    status = PcdRequest(PICC_REQALL,Card_Type_s);								// Ѱ��
    cardType = (Card_Type_s[0]<<8)|Card_Type_s[1];					
    if(status==MI_OK)															// Ѱ���ɹ�
    {					
        status=MI_ERR;					
        status = PcdAnticoll(Card_ID);											// ����ײ
		PC_Usart ((unsigned char*)"find\r\n");					
    }					
    if (status==MI_OK)															// ����ײ�ɹ�
    {					
        status=MI_ERR;					
        status =PcdSelect(Card_ID);					
    }					
    if(status==MI_OK)															// ѡ���ɹ�
    {			
        cardType = (Card_Type_s[0]<<8)|Card_Type_s[1];
        for(i = 0;i<temp;i++)
        {
            W25QXX_Read(READ_Buffer,FLASH_SIZE-40000+i*4,4);
            Student_ID[i].Stu_Card_ID[0] = READ_Buffer[0];
            Student_ID[i].Stu_Card_ID[1] = READ_Buffer[1];
            Student_ID[i].Stu_Card_ID[2] = READ_Buffer[2];
            Student_ID[i].Stu_Card_ID[3] = READ_Buffer[3];
        }
        for(i = 0;i<temp;i++)
        {
            if((Card_ID[0] == Student_ID[i].Stu_Card_ID[0]) && (Card_ID[1] == Student_ID[i].Stu_Card_ID[1]) && (Card_ID[2] == Student_ID[i].Stu_Card_ID[2]) && (Card_ID[3] == Student_ID[i].Stu_Card_ID[3]) )
            {	
                PC_Usart((unsigned char*)"OK!\r\n");
                LCD_P6x8Str(30,1,(unsigned char*)"ON ");						// �����ɹ�									
                TIM_SetCompare4(TIM4,15);										// �������
                delay_ms(1500);	
                delay_ms(1500);
                TIM_SetCompare4(TIM4,7);
                delay_ms(500);
//                Door_flag = CLOSE;
            }
        }
    }
    else
    {
        LCD_P6x8Str(30,1,(unsigned char*)"OFF");
        
    }					
}



void Add_Flash_ID(void)         //һ��ʼ���ѧ������
{
	u8 i;
	u8 initcardnum=4;
	
	Student_ID[0].Stu_Card_ID[0] = 0xD9;//ѧ��һ
	Student_ID[0].Stu_Card_ID[1] = 0x6A;
	Student_ID[0].Stu_Card_ID[2] = 0x5D;
	Student_ID[0].Stu_Card_ID[3] = 0x4A;
	
	Student_ID[1].Stu_Card_ID[0] = 0xA3;//ѧ����
	Student_ID[1].Stu_Card_ID[1] = 0xFF;
	Student_ID[1].Stu_Card_ID[2] = 0x81;
	Student_ID[1].Stu_Card_ID[3] = 0xB5;
	
	Student_ID[2].Stu_Card_ID[0] = 0xE3;//
	Student_ID[2].Stu_Card_ID[1] = 0x2D;
	Student_ID[2].Stu_Card_ID[2] = 0xED;
	Student_ID[2].Stu_Card_ID[3] = 0x00;
	
	Student_ID[3].Stu_Card_ID[0] = 0x36;//
	Student_ID[3].Stu_Card_ID[1] = 0xD4;
	Student_ID[3].Stu_Card_ID[2] = 0xD4;
	Student_ID[3].Stu_Card_ID[3] = 0x02;
	
	
	W25QXX_Write(&initcardnum,FLASH_SIZE - 1000,sizeof(initcardnum));
	
	
	for(i = 0;i<initcardnum;i++)
	{

		W25QXX_Write(&Student_ID[i].Stu_Card_ID[0],FLASH_SIZE - 10000 +4*i,sizeof(Student_ID[i].Stu_Card_ID[0]));
		W25QXX_Write(&Student_ID[i].Stu_Card_ID[1],FLASH_SIZE - 10000 +4*i + 1 ,sizeof(Student_ID[i].Stu_Card_ID[1]));
		W25QXX_Write(&Student_ID[i].Stu_Card_ID[2],FLASH_SIZE - 10000 +4*i + 1 + 1,sizeof(Student_ID[i].Stu_Card_ID[2]));
		W25QXX_Write(&Student_ID[i].Stu_Card_ID[3],FLASH_SIZE - 10000 +4*i + 1 + 1 + 1,sizeof(Student_ID[i].Stu_Card_ID[3]));
	}
}


void Read_Flash_ID(void)        //��ȡԭ���Ĵ���Flash��ID
{
	
	u8 i;
	
	W25QXX_Read(&temp,FLASH_SIZE - 1000,sizeof(temp));//��������
    
	PC_Usart((unsigned char*)"FLASH ID:%x \r\n",W25QXX_TYPE);
	PC_Usart((unsigned char*)"FLASH����Ŀ���Ϊ%d\r\n\r\n",temp);

	for(i = 0;i<temp;i++)
	{
		W25QXX_Read(READ_Buffer,FLASH_SIZE-40000+i*4,4);
		Student_ID[i].Stu_Card_ID[0] = READ_Buffer[0];
		Student_ID[i].Stu_Card_ID[1] = READ_Buffer[1];
		Student_ID[i].Stu_Card_ID[2] = READ_Buffer[2];
		Student_ID[i].Stu_Card_ID[3] = READ_Buffer[3];

	}
}

void Add_Student_ID(void)       //����һ��ѧ������  �ؽ���
{
	u8 i,j;
	
	while (MI_OK!=PcdRequest(0x52, Card_Type_s)); //һֱѭ��ֱ��������
	status = PcdAnticoll(Card_ID);//����ײ
	status = PcdSelect(Card_ID);  //ѡ��

	if(status==MI_OK)//�x���ɹ�
	{
		Read_Flash_ID();//��ȡԭ���Ŀ���
							
		for(i = temp;i<temp+1;i++)   //д��
		{
			for(j = 0;j<4;j++)
			{
				W25QXX_Write((u8*)Card_ID,FLASH_SIZE-40000 + temp*4 , 4);
			}
		}
		temp = temp + 1;  //������+1
		W25QXX_Write(&temp,FLASH_SIZE - 1000,sizeof(temp));		
	}
}

void Del_card_ID(void)          //�����п�������
{
   	u8 i;

  	TEXT_Buffer[0] = 0;
	TEXT_Buffer[1] = 0;
	TEXT_Buffer[2] = 0;
	TEXT_Buffer[3] = 0;
 
	W25QXX_Read(&temp,FLASH_SIZE - 1000,sizeof(temp));
	for(i = 0;i < temp;i++)
	{
		W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE - 1000 + temp*4 , 4);
	}
	temp = 0;	
	W25QXX_Write(&temp,FLASH_SIZE - 1000,sizeof(temp));
}

void Del_Card_Record(void)      //���ˢ����¼//��������
{
	u8 i ;
	
	W25QXX_Write(&time_flag,FLASH_SIZE - 1100,sizeof(time_flag));
				
	Swiping_Time.year1 = 0;
	Swiping_Time.year2 = 0;
	Swiping_Time.month = 0;
	Swiping_Time.data = 0;
	Swiping_Time.hour = 0;
	Swiping_Time.min = 0;
	Swiping_Time.sec = 0;
		
	for(i = 0;i<20;i++)
	{				
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 1000000 + 11*time_flag ,  sizeof(Stu_Card_Id[0]));
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 1000000 + 11*time_flag + 1 ,  sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 1000000 + 11*time_flag + 1 + 1 ,  sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 1000000 + 11*time_flag + 1 + 1 + 1,  sizeof(Stu_Card_Id[3]));
		
		W25QXX_Write(&Swiping_Time.year1, FLASH_SIZE -  1000000 + 11*time_flag  + 4 ,  sizeof(Swiping_Time.year1));
		W25QXX_Write(&Swiping_Time.year2, FLASH_SIZE -  1000000 + 11*time_flag  + 4 + 1 ,  sizeof(Swiping_Time.year2));
		W25QXX_Write(&Swiping_Time.month,FLASH_SIZE  -  1000000 + 11*time_flag  + 4 + 1 + 1, sizeof(calendar.w_month));
		W25QXX_Write(&Swiping_Time.data, FLASH_SIZE  -  1000000 + 11*time_flag  + 4 + 1 + 1 + 1 , sizeof(calendar.w_date));
		W25QXX_Write(&Swiping_Time.hour ,FLASH_SIZE  -  1000000 + 11*time_flag  + 4 + 1 + 1 + 1 + 1 , sizeof(calendar.hour));
		W25QXX_Write(&Swiping_Time.min, FLASH_SIZE   -  1000000 + 11*time_flag  + 4 + 1 + 1 + 1 + 1 + 1, sizeof(calendar.min));
		W25QXX_Write(&Swiping_Time.sec, FLASH_SIZE   -  1000000 + 11*time_flag  + 4 + 1 + 1 + 1 + 1 + 1 + 1, sizeof(calendar.sec));
	}
}




void Read_Day_Stu_Id(void)      //��ȡѧ��������һ���
{
	u8 i;
	if(day1 == 1)   //��һ������һ
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 600000+2,sizeof(read_card_num));   //��ȡ��������
		for(i = 0;i<read_card_num;i++)      //������
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 600000+2+1+4*i,sizeof(Read_Day_Stu_ID[i].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 600000+2+1+4*i+1,sizeof(Read_Day_Stu_ID[i].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 600000+2+1+4*i+1+1,sizeof(Read_Day_Stu_ID[i].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 600000+2+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[i].Stu_Card_ID[3]));
		}
 
	}
	if(day1 == 2)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 600000+202,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 600000+202+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 600000+202+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 600000+202+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 600000+202+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day1 == 3)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 600000+402,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 600000+402+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 600000+402+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 600000+402+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 600000+402+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}	
	if(day1 == 4)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 600000+602,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 600000+602+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 600000+602+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 600000+602+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 600000+602+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day1 == 5)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 600000+802,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 600000+802+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 600000+802+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 600000+802+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 600000+802+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day1 == 6)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 600000+1002,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 600000+1002+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 600000+1002+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 600000+1002+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 600000+1002+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day1 == 7)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 600000+1202,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 600000+1202+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 600000+1202+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 600000+1202+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 600000+1202+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day2 == 8)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 800000+2,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 800000+2+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 800000+2+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 800000+2+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 800000+2+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day2 == 9)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 800000+202,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 800000+202+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 800000+202+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 800000+202+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 800000+202+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}	
	if(day2 == 10)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 800000+402,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 800000+402+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 800000+402+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 800000+402+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 800000+402+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day2 == 11)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 800000+602,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 800000+602+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 800000+602+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 800000+602+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 800000+602+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day2 == 12)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 800000+802,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 800000+802+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 800000+802+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 800000+802+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 800000+802+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day2 == 13)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 800000+1002,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 800000+1002+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 800000+1002+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 800000+1002+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 800000+1002+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day2 == 14)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 800000+1202,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 800000+1202+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 800000+1202+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 800000+1202+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 800000+1202+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day3 == 15)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 1000000+2,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE - 1000000+2+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE - 1000000+2+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE - 1000000+2+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE - 1000000+2+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day3 == 16)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 1000000+202,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE -1000000+202+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE -1000000+202+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE -1000000+202+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE -1000000+202+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day3 == 17)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 1000000+402,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE -1000000+402+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE -1000000+402+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE -1000000+402+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE -1000000+402+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day3 == 18)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 1000000+602,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE -1000000+602+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE -1000000+602+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE -1000000+602+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE -1000000+602+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day3 == 19)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 1000000+802,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE -1000000+802+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE -1000000+802+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE -1000000+802+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE -1000000+802+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day3 == 20)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 1000000+1002,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE -1000000+1002+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE -1000000+1002+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE -1000000+1002+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE -1000000+1002+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
	if(day3 == 21)
	{
		W25QXX_Read(&read_card_num,FLASH_SIZE - 1000000+1202,sizeof(read_card_num));
		for(i = 0;i<read_card_num;i++)
		{
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[0],FLASH_SIZE -1000000+1202+1+4*i,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[0]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[1],FLASH_SIZE -1000000+1202+1+4*i+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[1]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[2],FLASH_SIZE -1000000+1202+1+4*i+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[2]));
			W25QXX_Read(&Read_Day_Stu_ID[i].Stu_Card_ID[3],FLASH_SIZE -1000000+1202+1+4*i+1+1+1,sizeof(Read_Day_Stu_ID[0].Stu_Card_ID[3]));
		}
	}
}




void Write_Day_Stu_Id(void)
{
	if(day1 == 1)
	{
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 600000+2+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 600000+2+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 600000+2+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 600000+2+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 600000+2,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}

	if(day1 == 2)
	{
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 600000+202+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 600000+202+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 600000+202+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 600000+202+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 600000+202,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}

	if(day1 == 3)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 600000+402+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 600000+402+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 600000+402+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 600000+402+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 600000+402,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day1 == 4)
	{
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 600000+602+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 600000+602+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 600000+602+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 600000+602+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 600000+602,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day1 == 5)
	{
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 600000+802+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 600000+802+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 600000+802+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 600000+802+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 600000+802,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day1 == 6)
	{
		W25QXX_Write(&read_card_num,FLASH_SIZE - 600000+1002,sizeof(read_card_num));//д������һ�����ŵ���Ŀ

		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 600000+1002+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 600000+1002+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 600000+1002+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 600000+1002+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
	}
	if(day1 == 7)
	{
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 600000+1202+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 600000+1202+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 600000+1202+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 600000+1202+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 600000+1202,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day2 == 8)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 800000+2+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 800000+2+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 800000+2+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 800000+2+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 800000+2,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day2 == 9)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 800000+202+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 800000+202+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 800000+202+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 800000+202+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 800000+202,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day2 == 10)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 800000+402+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 800000+402+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 800000+402+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 800000+402+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 800000+402,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day2 == 11)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 800000+602+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 800000+602+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 800000+602+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 800000+602+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 800000+602,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day2 == 12)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 800000+802+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 800000+802+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 800000+802+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 800000+802+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 800000+802,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day2 == 13)
	{
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 800000+1002+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 800000+1002+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 800000+1002+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 800000+1002+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 800000+1002,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day2 == 14)
	{
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 800000+1202+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 800000+1202+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 800000+1202+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 800000+1202+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 800000+1202,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day3 == 15)
	{
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 1000000+2+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 1000000+2+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 1000000+2+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 1000000+2+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 1000000+2,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day3 == 16)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 1000000+202+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 1000000+202+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 1000000+202+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 1000000+202+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 1000000+202,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day3 == 17)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 1000000+402+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 1000000+402+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 1000000+402+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 1000000+402+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 1000000+402,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day3 == 18)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 1000000+602+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 1000000+602+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 1000000+602+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 1000000+602+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 1000000+602,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day3 == 19)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 1000000+802+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 1000000+802+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 1000000+802+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 1000000+802+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 1000000+802,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day3 == 20)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 1000000+1002+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 1000000+1002+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 1000000+1002+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 1000000+1002+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 1000000+1002,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
	if(day3 == 21)
	{	
		W25QXX_Write(&Stu_Card_Id[0],FLASH_SIZE - 1000000+1202+1+4*read_card_num,sizeof(Stu_Card_Id[0]));//д�뿨��
		W25QXX_Write(&Stu_Card_Id[1],FLASH_SIZE - 1000000+1202+1+4*read_card_num+1,sizeof(Stu_Card_Id[1]));
		W25QXX_Write(&Stu_Card_Id[2],FLASH_SIZE - 1000000+1202+1+4*read_card_num+1+1,sizeof(Stu_Card_Id[2]));
		W25QXX_Write(&Stu_Card_Id[3],FLASH_SIZE - 1000000+1202+1+4*read_card_num+1+1+1,sizeof(Stu_Card_Id[3]));
		
		read_card_num++;
		W25QXX_Write(&read_card_num,FLASH_SIZE - 1000000+1202,sizeof(read_card_num));//д������һ�����ŵ���Ŀ
	}
}
