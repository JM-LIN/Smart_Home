#include "timer.h"
#include "led.h"
#include "rtc.h"
#include "spi.h"
#include "flash.h"
#include "usart.h"





//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!


extern u8 week1,week2,week3;	
extern u8 day1,day2,day3;
extern long int sec1,sec2,sec3;//��ʱ��1,2,3��
extern u32 FLASH_SIZE;//flash���ܴ�С�ռ�


u8 del_data_flag1 = 0;
u8 del_data_flag2 = 0;
u8 del_data_flag3 = 0;


u8 write_month;//д��flash���·�
u8 write_data;//д��flash������
	
	
u8 sec_num;
	
void Week1_Del(void)
{
	u8 Week1_Del = 0;
	vu16 i;
	for(i = 0;i<1300;i++)
	{
		W25QXX_Write(&Week1_Del,FLASH_SIZE - 600000+i,sizeof(Week1_Del));
	}
}

void Week2_Del(void)
{
	u8 Week2_Del = 0;
	vu16 i;
	for(i = 0;i<1300;i++)
	{
		W25QXX_Write(&Week2_Del,FLASH_SIZE - 800000+i,sizeof(Week2_Del));
	}
}

void Week3_Del(void)
{
	u8 Week3_Del = 0;
	vu16 i;
	for(i = 0;i<1300;i++)
	{
		W25QXX_Write(&Week3_Del,FLASH_SIZE - 1000000+i,sizeof(Week3_Del));
	}
}

	
void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}

//��ʱ��2�жϷ������
//void TIM2_IRQHandler(void)   //TIM2�ж�
//{
//	sec1++;
//	if(sec1 == 172800)
//	{
//		sec1 = 0;
//		day1++;
//		day2++;
//		day3++;
//		
//		if(day1 == 14)  //������������һ��
//		{
//			del_data_flag1 = 1;
//		}
//		if(day2 == 28)
//		{
//			del_data_flag2 = 1;
//		}
//		if(day3 == 42)
//		{
//			del_data_flag3 = 1;
//		}
//		if(del_data_flag1 == 1)
//		{
////			printf("�����һ�ܵĲ���");
////			printf("\n\r");
//			Week1_Del();
//			del_data_flag1 =0;
////			printf("��һ������ɹ�\n\r");
////			printf("\n\r");
//		}
//		if(del_data_flag2 == 1)
//		{
//	//		printf("����ڶ��ܵĲ���\n\r");
////			printf("\n\r");
//			Week2_Del();
//			del_data_flag2 =0;
//	//		printf("�ڶ�������ɹ�\n\r");
////			printf("\n\r");
//		}
//		if(del_data_flag3 == 1)
//		{
////			printf("��������ܵĲ���\n\r");
////			printf("\n\r");
//			Week3_Del();
//			del_data_flag3 =0;
////			printf("����������ɹ�\n\r");
////			printf("\n\r");
//		}
//		if(day1 == 42) 
//		{
//			day1 = 1;
//		}
//		if(day2 == 42) 
//		{
//			day2 = 1;
//		}
//		if(day3 == 42) 
//		{
//			day3 = 1;
//		}
//	}
//	if(sec1 == 2)      //�����Ƿ���0��
//	{
//		if( (calendar.sec == 0) && (calendar.min ==0) && (calendar.hour==0) )
//		{
//			if(day1 == 1)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000 + 1,sizeof(write_data));//д������
//			}
//			if(day1 == 2)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+200 + 1,sizeof(write_data));//д������
//			}
//			if(day1 == 3)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+400 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+400 + 1,sizeof(write_data));//д������
//			}
//			if(day1 == 4)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+600 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+600 + 1,sizeof(write_data));//д������
//			}
//			if(day1 == 5)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+800 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+800 + 1,sizeof(write_data));//д������
//			}
//			if(day1 == 6)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+1000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+1000 + 1,sizeof(write_data));//д������
//			}
//			if(day1 == 7)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+1200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+1200 + 1,sizeof(write_data));//д������
//			}
//			
//			if(day2 == 8)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000 + 1,sizeof(write_data));//д������
//			}
//			if(day2 == 9)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+200 + 1,sizeof(write_data));//д������
//			}
//			if(day2 == 10)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+400 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+400 + 1,sizeof(write_data));//д������
//			}
//			if(day2 == 11)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+600 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+600 + 1,sizeof(write_data));//д������
//			}
//			if(day2 == 12)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+800 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+800 + 1,sizeof(write_data));//д������
//			}
//			if(day2 == 13)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+1000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+1000 + 1,sizeof(write_data));//д������
//			}
//			if(day2 == 14)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+1200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+1200 + 1,sizeof(write_data));//д������
//			}
//			if(day3 == 15)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000 + 1,sizeof(write_data));//д������
//			}
//			if(day3 == 16)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+200 + 1,sizeof(write_data));//д������
//			}
//			if(day3 == 17)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+400 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+400 + 1,sizeof(write_data));//д������
//			}
//			if(day3 == 18)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000 +600+ 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000 +600+ 1,sizeof(write_data));//д������
//			}
//			if(day3 == 19)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+800 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+800 + 1,sizeof(write_data));//д������
//			}
//			if(day3 == 20)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+1000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+1000 + 1,sizeof(write_data));//д������
//			}
//			if(day3 == 21)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+1200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+1200 + 1,sizeof(write_data));//д������
//			}
//		}		
//	}
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
//	{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
//	}
//}
	


void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);// ����������жϱ�־λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}



//void TIM5_Int_Init(u16 arr,u16 psc)
//{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
//	
//	//��ʱ��TIM3��ʼ��
//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// 
//	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

//	//�ж����ȼ�NVIC����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


//	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx					 
//}





void PWM_Init(u16 arr,u16 psc)
{   
	
	/********************* TIM4_CH1 -- CH4   PWM ����  ***********************/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
		//���ø�����Ϊ�����������,���TIM4 CH1~CH4  ��PWM���岨��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
  //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM4 Channel2 PWMģʽ	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC1
//	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC2
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC3
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
	
		/********************* TIM2_CH4   PWM ����  ***********************/


//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
//	    
// 
//	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2  , ENABLE);
//	//���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��	GPIOB.11
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM4_CH1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
// 
// 
// 
//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

//	//��ʼ��TIM4 Channel2 PWMģʽ	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
//	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC1
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
// 
//	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM4
//	TIM_CtrlPWMOutputs(TIM2, ENABLE);
}





