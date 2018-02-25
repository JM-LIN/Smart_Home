#include "exti.h"
#include "sys.h"
#include "key.h"
#include "delay.h"
#include "usart.h"


extern u8 KEY_SE;


void KEY_EXTI_Init()
{
	//¶¨ÒåÒ»¸ö°´¼üÖĞ¶Ï½á¹¹Ìå
		EXTI_InitTypeDef EXTI_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//¿ªÆôÊ±ÖÓ¸´ÓÃ¹¦Ä
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2); //KEY2 IO¿ÚÎªPE2
		EXTI_InitStructure.EXTI_Line=EXTI_Line2;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//Íâ²¿ÖĞ¶ÏÀàĞÍ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//ÏÂ½µÑØÖĞ¶Ï
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//³õÊ¼»¯°´¼üÖĞ¶Ï
	
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3); //KEY1 IO¿ÚÎªPE3
		EXTI_InitStructure.EXTI_Line=EXTI_Line3;	//KEY1
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//Íâ²¿ÖĞ¶ÏÀàĞÍ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//ÏÂ½µÑØÖĞ¶Ï
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//³õÊ¼»¯°´¼üÖĞ¶Ï
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4); //KEY1 IO¿ÚÎªPE3
		EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEY0
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//Íâ²¿ÖĞ¶ÏÀàĞÍ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//ÏÂ½µÑØÖĞ¶Ï
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//³õÊ¼»¯°´¼üÖĞ¶Ï
	
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); //WK_UP IO¿ÚÎªPA0
		EXTI_InitStructure.EXTI_Line=EXTI_Line0;	//KEY0
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//Íâ²¿ÖĞ¶ÏÀàĞÍ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;;//ÏÂ½µÑØÖĞ¶Ï
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//³õÊ¼»¯°´¼üÖĞ¶Ï
}


void KEY_NVIC_Init()
{
		
		NVIC_InitTypeDef NVIC_InitStructure;

		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//Ê¹ÄÜ°´¼üWK_UPËùÔÚµÄÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//ÇÀÕ¼ÓÅÏÈ¼¶2£¬ 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//×ÓÓÅÏÈ¼¶3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//Ê¹ÄÜ°´¼üKEY2ËùÔÚµÄÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//ÇÀÕ¼ÓÅÏÈ¼¶2£¬ 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//×ÓÓÅÏÈ¼¶2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//Ê¹ÄÜ°´¼üKEY1ËùÔÚµÄÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//ÇÀÕ¼ÓÅÏÈ¼¶2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//×ÓÓÅÏÈ¼¶1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_Init(&NVIC_InitStructure);  	  //¸ù¾İNVIC_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷

		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//Ê¹ÄÜ°´¼üKEY0ËùÔÚµÄÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//ÇÀÕ¼ÓÅÏÈ¼¶2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//×ÓÓÅÏÈ¼¶0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
		
  	NVIC_Init(&NVIC_InitStructure);  	  //¸ù¾İNVIC_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷
		
		
}



////Íâ²¿ÖĞ¶Ï0·şÎñ³ÌĞò 
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);//Ïû¶¶
//	if(WK_UP==1)	 	 //WK_UP°´¼ü
//	{				 
//		BEEP=!BEEP;	
//	}
//	EXTI_ClearITPendingBit(EXTI_Line0); //Çå³ıLINE0ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
//}
 




//Íâ²¿ÖĞ¶Ï2·şÎñ³ÌĞò
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//Ïû¶¶
	if(KEY2==0)	  //°´¼üKEY2
	{
		KEY_SE = KEY2_PRES;
		
		printf("\n\r");
		printf("enter the key2 successfully");
		printf("\n\r");
	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //Çå³ıLINE2ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
}




//Íâ²¿ÖĞ¶Ï3·şÎñ³ÌĞò
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//Ïû¶¶
	if(KEY1==0)	 //°´¼üKEY1
	{				 
		KEY_SE = KEY1_PRES;
		printf("\n\r");
		printf("enter the key1 successfully");
		printf("\n\r");
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //Çå³ıLINE3ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
}




void EXTI4_IRQHandler(void)
{
	delay_ms(10);//Ïû¶¶
	if(KEY0==0)	 //°´¼üKEY0
	{
		KEY_SE = KEY0_PRES;
		printf("\n\r");
		printf("enter the key3 successfully");
		printf("\n\r");
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //Çå³ıLINE4ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
}
