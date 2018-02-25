#include "exti.h"
#include "sys.h"
#include "key.h"
#include "delay.h"
#include "usart.h"


extern u8 KEY_SE;


void KEY_EXTI_Init()
{
	//����һ�������жϽṹ��
		EXTI_InitTypeDef EXTI_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//����ʱ�Ӹ��ù��
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2); //KEY2 IO��ΪPE2
		EXTI_InitStructure.EXTI_Line=EXTI_Line2;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//�ⲿ�ж�����
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//��ʼ�������ж�
	
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3); //KEY1 IO��ΪPE3
		EXTI_InitStructure.EXTI_Line=EXTI_Line3;	//KEY1
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//�ⲿ�ж�����
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//��ʼ�������ж�
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4); //KEY1 IO��ΪPE3
		EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEY0
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//�ⲿ�ж�����
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//��ʼ�������ж�
	
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); //WK_UP IO��ΪPA0
		EXTI_InitStructure.EXTI_Line=EXTI_Line0;	//KEY0
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//�ⲿ�ж�����
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;;//�½����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//��ʼ�������ж�
}


void KEY_NVIC_Init()
{
		
		NVIC_InitTypeDef NVIC_InitStructure;

		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
		
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
		
}



////�ⲿ�ж�0������� 
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);//����
//	if(WK_UP==1)	 	 //WK_UP����
//	{				 
//		BEEP=!BEEP;	
//	}
//	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ  
//}
 




//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==0)	  //����KEY2
	{
		KEY_SE = KEY2_PRES;
		
		printf("\n\r");
		printf("enter the key2 successfully");
		printf("\n\r");
	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE2�ϵ��жϱ�־λ  
}




//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY1==0)	 //����KEY1
	{				 
		KEY_SE = KEY1_PRES;
		printf("\n\r");
		printf("enter the key1 successfully");
		printf("\n\r");
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
}




void EXTI4_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY0==0)	 //����KEY0
	{
		KEY_SE = KEY0_PRES;
		printf("\n\r");
		printf("enter the key3 successfully");
		printf("\n\r");
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ  
}
