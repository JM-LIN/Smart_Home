#include "CheakCard.h"


void CheakCard()
{
          uint8_t i;
					status = PcdRequest(PICC_REQALL,Card_Type_s);/*����*/
					cardType = (Card_Type_s[0]<<8)|Card_Type_s[1];
					if(status==MI_OK)//�����ɹ�
					{
						status=MI_ERR;
						status = PcdAnticoll(Card_ID);/*����ײ*/
					}
					if (status==MI_OK)//���nײ�ɹ�
					{
						status=MI_ERR;
						status =PcdSelect(Card_ID);
					}
					if(status==MI_OK)//�x���ɹ�
					{
						cardType = (Card_Type_s[0]<<8)|Card_Type_s[1];
						for(i = 0;i<temp;i++)
						{
								W25QXX_Read(READ_Buffer,FLASH_SIZE-10000+i*4,4);
								Student_ID[i].Stu_Card_ID[0] = READ_Buffer[0];
								Student_ID[i].Stu_Card_ID[1] = READ_Buffer[1];
								Student_ID[i].Stu_Card_ID[2] = READ_Buffer[2];
								Student_ID[i].Stu_Card_ID[3] = READ_Buffer[3];
						}
						for(i = 0;i<temp;i++)
						{
								if( (Card_ID[0] == Student_ID[i].Stu_Card_ID[0]) && (Card_ID[1] == Student_ID[i].Stu_Card_ID[1]) && (Card_ID[2] == Student_ID[i].Stu_Card_ID[2]) && (Card_ID[3] == Student_ID[i].Stu_Card_ID[3]) )
								{	
										temp1 = 1;
									
						
										Swiping_Time.year1 = calendar.w_year/100;//��ˢ��ʱ�������
										Swiping_Time.year2 = calendar.w_year%100;
										Swiping_Time.month = calendar.w_month;
										Swiping_Time.data  = calendar.w_date;
										Swiping_Time.hour  = calendar.hour;
										Swiping_Time.min   = calendar.min;
										Swiping_Time.sec   = calendar.sec;
									
										
									 
									PC_Usart("OK!\r");
										TIM_SetCompare4(TIM4,10);//�������
										delay_ms(3000);
										TIM_SetCompare4(TIM4,20);
										delay_ms(3000); 
										TIM_SetCompare4(TIM4,0);
								}
						}
						
						/**********************************��¼����********************************************************/
						if(temp1 == 1)  //��¼����
						{	
							
							
							Stu_Card_Id[0] = Card_ID[0];
							Stu_Card_Id[1] = Card_ID[1];
							Stu_Card_Id[2] = Card_ID[2];
							Stu_Card_Id[3] = Card_ID[3];
							
							
				
							Read_Day_Stu_Id();//���������ж����ˣ��ֱ�Ŀ���
							
							
							for(i = 0;i<read_card_num;i++)
							{
								if( (Stu_Card_Id[0] == Read_Day_Stu_ID[i].Stu_Card_ID[0]) && (Stu_Card_Id[1] == Read_Day_Stu_ID[i].Stu_Card_ID[1]) && (Stu_Card_Id[2] == Read_Day_Stu_ID[i].Stu_Card_ID[2]) && (Stu_Card_Id[3] == Read_Day_Stu_ID[i].Stu_Card_ID[3]) )
								{
									same_id_day_flag = 1;
								}
							}
							if(same_id_day_flag == 1)
							{
								same_id_day_flag = 0;
								  return;
							}
							if(same_id_day_flag == 0)
							{
								Write_Day_Stu_Id();			

                //������
                text();
								
							}
							
//							Text_Function();//�������õĳ���
							time_flag++;//����һ������
							
							
							W25QXX_Write(&time_flag,FLASH_SIZE - 1100,sizeof(time_flag));
							
							temp1 = 0;
							
						}
						/**********************************��¼����********************************************************/
						
						
					 }

}