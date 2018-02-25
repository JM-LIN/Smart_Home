#include "CheakCard.h"


void CheakCard()
{
          uint8_t i;
					status = PcdRequest(PICC_REQALL,Card_Type_s);/*尋卡*/
					cardType = (Card_Type_s[0]<<8)|Card_Type_s[1];
					if(status==MI_OK)//尋卡成功
					{
						status=MI_ERR;
						status = PcdAnticoll(Card_ID);/*防冲撞*/
					}
					if (status==MI_OK)//防衝撞成功
					{
						status=MI_ERR;
						status =PcdSelect(Card_ID);
					}
					if(status==MI_OK)//選卡成功
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
									
						
										Swiping_Time.year1 = calendar.w_year/100;//把刷卡时间读出来
										Swiping_Time.year2 = calendar.w_year%100;
										Swiping_Time.month = calendar.w_month;
										Swiping_Time.data  = calendar.w_date;
										Swiping_Time.hour  = calendar.hour;
										Swiping_Time.min   = calendar.min;
										Swiping_Time.sec   = calendar.sec;
									
										
									 
									PC_Usart("OK!\r");
										TIM_SetCompare4(TIM4,10);//驱动舵机
										delay_ms(3000);
										TIM_SetCompare4(TIM4,20);
										delay_ms(3000); 
										TIM_SetCompare4(TIM4,0);
								}
						}
						
						/**********************************记录部分********************************************************/
						if(temp1 == 1)  //记录部分
						{	
							
							
							Stu_Card_Id[0] = Card_ID[0];
							Stu_Card_Id[1] = Card_ID[1];
							Stu_Card_Id[2] = Card_ID[2];
							Stu_Card_Id[3] = Card_ID[3];
							
							
				
							Read_Day_Stu_Id();//读出当天有多少人，分别的卡号
							
							
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

                //测试用
                text();
								
							}
							
//							Text_Function();//测试所用的程序
							time_flag++;//增加一个人数
							
							
							W25QXX_Write(&time_flag,FLASH_SIZE - 1100,sizeof(time_flag));
							
							temp1 = 0;
							
						}
						/**********************************记录部分********************************************************/
						
						
					 }

}