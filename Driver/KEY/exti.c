#include "exti.h"

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 配置中断源 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  配置 PC4/PC5 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline clock and AFIO clock */
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO),ENABLE);
												
	/* config the NVIC */
	NVIC_Configuration();

	/* EXTI line gpio config*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       				// key1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 				// 下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;						// key2       
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;						// key3      
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;						// key4       
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* EXTI line mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			// 上升沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			// 上升沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			// 上升沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			// 上升沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}

/*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}

void EXTI0_IRQHandler_Routine (void)															// KEY2
{							
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) 											// 确保是否产生了EXTI Line中断
	{																							
//		ZigBee_Usart((unsigned char*)"So1");
//		Door_flag = !Door_flag;
//		Light_ON_flag = !Light_ON_flag;
//        TIM_SetCompare4(TIM4,15);
//         Pwm_led_status = 1;
//        add();																			// 添加卡号	
        
        PC_Usart((unsigned char*)"key2 test\n");
		EXTI_ClearITPendingBit(EXTI_Line0);     										// 清除中断标志位
	}  										
}

void EXTI4_IRQHandler_Routine (void)															// KEY3
{							
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) 											// 确保是否产生了EXTI Line中断
	{	
//		ZigBee_Usart((unsigned char*)"So2");
        
//        TIM_SetCompare4(TIM4,7);
        
//       Pwm_led_status = 0;
//		Light_OFF_flag = !Light_OFF_flag;
//		Read_Flash_ID();
        
//		Humidi_TOGGLE;
//		Fan_TOGGLE;
        
        PC_Usart((unsigned char*)"key3 test\n");
		EXTI_ClearITPendingBit(EXTI_Line4);     										// 清除中断标志位
	}  										
}

void EXTI1_IRQHandler_Routine (void)															// KEY4
{							
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) 											// 确保是否产生了EXTI Line中断
	{																	
//		ZigBee1_OFF;
//		ZigBee2_OFF;
//		OLED_RST();																		// OLED刷新		
//		Del_card_ID();       											// 清空全部卡号
        
        PC_Usart((unsigned char*)"key4 test\n");
        EXTI_ClearITPendingBit(EXTI_Line1);     										// 清除中断标志位
	}  										
}	

void EXTI3_IRQHandler_Routine (void)															// KEY5
{							
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) 											// 确保是否产生了EXTI Line中断
	{	
        
//		ZigBee1_ON;																		// Zigbee Coordinator供电
//		ZigBee2_ON;																		// Zigbee End_Device1、2供电

        PC_Usart((unsigned char*)"key5 test\n");
        EXTI_ClearITPendingBit(EXTI_Line3);     										// 清除中断标志位
	}  										
}

/*********************************************END OF FILE**********************/
