#include "exti.h"
#include "sys.h"
#include "key.h"
#include "delay.h"
#include "usart.h"


extern u8 KEY_SE;


void KEY_EXTI_Init()
{
	//定义一个按键中断结构体
		EXTI_InitTypeDef EXTI_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//开启时钟复用功�
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2); //KEY2 IO口为PE2
		EXTI_InitStructure.EXTI_Line=EXTI_Line2;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//外部中断类型
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//初始化按键中断
	
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3); //KEY1 IO口为PE3
		EXTI_InitStructure.EXTI_Line=EXTI_Line3;	//KEY1
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//外部中断类型
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//初始化按键中断
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4); //KEY1 IO口为PE3
		EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEY0
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//外部中断类型
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//初始化按键中断
	
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); //WK_UP IO口为PA0
		EXTI_InitStructure.EXTI_Line=EXTI_Line0;	//KEY0
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//外部中断类型
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;;//下降沿中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//初始化按键中断
}


void KEY_NVIC_Init()
{
		
		NVIC_InitTypeDef NVIC_InitStructure;

		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
		
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		
}



////外部中断0服务程序 
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);//消抖
//	if(WK_UP==1)	 	 //WK_UP按键
//	{				 
//		BEEP=!BEEP;	
//	}
//	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
//}
 




//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY2==0)	  //按键KEY2
	{
		KEY_SE = KEY2_PRES;
		
		printf("\n\r");
		printf("enter the key2 successfully");
		printf("\n\r");
	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE2上的中断标志位  
}




//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键KEY1
	{				 
		KEY_SE = KEY1_PRES;
		printf("\n\r");
		printf("enter the key1 successfully");
		printf("\n\r");
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}




void EXTI4_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY0==0)	 //按键KEY0
	{
		KEY_SE = KEY0_PRES;
		printf("\n\r");
		printf("enter the key3 successfully");
		printf("\n\r");
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}
