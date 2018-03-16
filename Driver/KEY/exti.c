#include "exti.h"

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* �����ж�Դ */
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
  * @brief  ���� PC4/PC5 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 				// ��������
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
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			// �������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			// �������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			// �������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			// �������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}

/*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}

void EXTI0_IRQHandler_Routine (void)															// KEY2
{							
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) 											// ȷ���Ƿ������EXTI Line�ж�
	{																							
//		ZigBee_Usart((unsigned char*)"So1");
//		Door_flag = !Door_flag;
//		Light_ON_flag = !Light_ON_flag;
//        TIM_SetCompare4(TIM4,15);
//         Pwm_led_status = 1;
//        add();																			// ��ӿ���	
        
        PC_Usart((unsigned char*)"key2 test\n");
		EXTI_ClearITPendingBit(EXTI_Line0);     										// ����жϱ�־λ
	}  										
}

void EXTI4_IRQHandler_Routine (void)															// KEY3
{							
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) 											// ȷ���Ƿ������EXTI Line�ж�
	{	
//		ZigBee_Usart((unsigned char*)"So2");
        
//        TIM_SetCompare4(TIM4,7);
        
//       Pwm_led_status = 0;
//		Light_OFF_flag = !Light_OFF_flag;
//		Read_Flash_ID();
        
//		Humidi_TOGGLE;
//		Fan_TOGGLE;
        
        PC_Usart((unsigned char*)"key3 test\n");
		EXTI_ClearITPendingBit(EXTI_Line4);     										// ����жϱ�־λ
	}  										
}

void EXTI1_IRQHandler_Routine (void)															// KEY4
{							
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) 											// ȷ���Ƿ������EXTI Line�ж�
	{																	
//		ZigBee1_OFF;
//		ZigBee2_OFF;
//		OLED_RST();																		// OLEDˢ��		
//		Del_card_ID();       											// ���ȫ������
        
        PC_Usart((unsigned char*)"key4 test\n");
        EXTI_ClearITPendingBit(EXTI_Line1);     										// ����жϱ�־λ
	}  										
}	

void EXTI3_IRQHandler_Routine (void)															// KEY5
{							
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) 											// ȷ���Ƿ������EXTI Line�ж�
	{	
        
//		ZigBee1_ON;																		// Zigbee Coordinator����
//		ZigBee2_ON;																		// Zigbee End_Device1��2����

        PC_Usart((unsigned char*)"key5 test\n");
        EXTI_ClearITPendingBit(EXTI_Line3);     										// ����жϱ�־λ
	}  										
}

/*********************************************END OF FILE**********************/
