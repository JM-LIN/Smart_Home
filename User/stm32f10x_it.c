/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <string.h>

/* Drivers */
#include "wifi.h"
#include "WIFI_BufferPool.h"
#include "ZigBee_BufferPool.h"
#include "opp.h"
#include "card_records.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Start of ZigBee Variable */
extern  __IO u8 CLI_ID;
volatile int Esp8266_flag = 0;                                      // 溢出标记													
extern volatile unsigned char flowsize;							    // 溢出大小																							
extern volatile unsigned char wLoc;								    // 当前写入位置
extern volatile unsigned char _buffer[Flow_Size_Max];			    // 缓冲池的缓冲区
extern volatile uint8_t ucTcpClosedFlag;						    // ESP8266中Tcp关闭标志
/* End of ZigBee Variable */

/* Start of ZigBee Variable */
volatile int ZigBee_flag = 0;							
extern volatile unsigned char ZigBee_wLoc;							// 写入位置
extern volatile unsigned char ZigBee_flowsize;
extern volatile unsigned char ZigBee_buffer[ZigBee_Flow_Size_Max];
/* End of ZigBee Variable */

/* Start of Breathing light Variable */
volatile unsigned int Pwm_led_status = 0;				// 呼吸灯状态
unsigned int Brightness_Level = 145;			// 呼吸灯亮度等级  0~145
extern uint8_t OpenWave[];
/* End of Breathing light Variable */

/* Private function prototypes -----------------------------------------------*/
extern void TimingDelay_Decrement(void);                            // SysTick系统定时

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)						
{						
	volatile char data;							
							
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)								// 读数据寄存器非空
	{							
		data = USART_ReceiveData(USART1); 												// 接收串口得到的数据
		USART_SendData(USART3, data);					
		
/****************************************以下是接收ESP8266数据操作******************************/		
		if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //预留1个字节写结束符
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = data;		

/****************************************以下是接收传感器数据操作******************************/		
		if( data == 0x23 )																// 遇到'#'结束接收
		{						// || BPCheckFlow()				
			Esp8266_flag = 0;												
			wLoc = 0;																	// 复位写入位置				
//			flowflag = 0;
		}						
							
		if (Esp8266_flag)    															// 若前面识别到了"：",则现在开始识别接收的数据
		{					
			WIFI_BPWriteData(data);															// 写入数据
//			USART_SendData(USART3, ch);
		}
		if (data == 0x3a )   															// 从":"后面开始接收数据
		{
			Esp8266_flag = 1; 
			wLoc = 0;
			memset((void *)_buffer,'\0',sizeof(_buffer));
		}	
	}
	
	if ( USART_GetITStatus( USART1, USART_IT_IDLE ) == SET )                            //数据帧接收完毕
	{
		strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;		
		data = USART_ReceiveData( USART1 );                                             //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)	
		ucTcpClosedFlag = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED" ) ? 1 : 0;		
	} 	
	USART_ClearFlag(USART1,USART_FLAG_TC);												// 清除中断标志.
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)															// 用于ZigBee通信
{													
	volatile char data;								
								
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)								// 读数据寄存器非空
	{							
		data = USART_ReceiveData(USART2); 												// 接收串口得到的数据
									
		if( data == 0x23 )																// 结束符'#'
		{											
			ZigBee_flag = 0;																			
			ZigBee_wLoc = 0;															// 复位写入位置				
		}											
																			
		if (ZigBee_flag)    															// 若前面识别到了"@",则现在开始识别接收的数据
		{														
			ZigBee_BPWriteData(data);													// 写入数据
//			USART_SendData(USART3, data);											
		}											
	    if (data == 0x40 )   															// 从"@"后面开始接收数据
		{							
			ZigBee_flag = 1; 							
			ZigBee_wLoc = 0;															// 复位写入位置	
			memset((void *)ZigBee_buffer,'\0',sizeof(ZigBee_buffer));
		}			
	}	 	
	USART_ClearFlag(USART2,USART_FLAG_TC);												// 清除中断标志.	 
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)															// 用于PC机通信
{							
	volatile uint8_t ch;							
								
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)							
	{ 				 				
		//ch = USART1->DR;							
		ch = USART_ReceiveData(USART3);							
//		USART_SendData(USART2, ch);   													// 将接收到的数据分别送到ZigBee、ESP8266中
		USART_SendData(USART1, ch);							
	}  							
	USART_ClearFlag(USART3,USART_FLAG_TC);												// 清除中断标志.
}	

/**
  * @brief  呼吸灯
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void)		
{
    static uint8_t pwm_index = 0;			//用于PWM查表
	static uint8_t period_cnt = 0;		//用于计算周期数
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)	//TIM_IT_Update
 	{			
		period_cnt++;
		if(period_cnt >= 10)										//若输出的周期数大于10，输出下一种脉冲宽的PWM波
		{
			
			TIM1->CCR1 = OpenWave[pwm_index];	//根据PWM表修改定时器的比较寄存器值
            TIM1->CCR2 = OpenWave[pwm_index];
            TIM1->CCR3 = OpenWave[pwm_index];
			pwm_index++;												//标志PWM表的下一个元素
		
			if( pwm_index >=  40)								//若PWM脉冲表已经输出完成一遍，重置PWM查表标志
			{
//                PC_Usart((unsigned char*)"fuck\n\n");
				pwm_index=0;								
			}
			
			period_cnt=0;												//重置周期计数标志
		}
		TIM_ClearITPendingBit (TIM1, TIM_IT_Update);	//必须要清除中断标志位
	}
    /*
	static uint16_t pwm_index = 0;									// 用于PWM查表
	static uint8_t period_cnt = 0;									// 用于计算周期数
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)									//TIM_IT_Update
 	{			
		switch(Pwm_led_status)
        {
			case 1 :
				period_cnt++;					
				if(period_cnt >= 40)											// 输出的周期数大于20，输出下一种脉冲宽的PWM波
				{									
					if(pwm_index < Brightness_Level)	
					{
						TIM1->CCR1 = OpenWave[pwm_index];						// 根据PWM表修改定时器的比较寄存器值
//						TIM1->CCR2 = OpenWave[pwm_index];
//						TIM1->CCR3 = OpenWave[pwm_index];
						pwm_index++;											// 标志PWM表的下一个元素
					}																	
					else if(pwm_index >= Brightness_Level)
					{
						TIM1->CCR1 = OpenWave[Brightness_Level];
//						TIM1->CCR2 = OpenWave[Brightness_Level];
//						TIM1->CCR3 = OpenWave[Brightness_Level];
					}   
					period_cnt=0;												// 重置周期计数标志
				}
				break;

			case 0 :
				period_cnt++;					
				if(period_cnt >= 40)											// 输出的周期数大于20，输出下一种脉冲宽的PWM波
				{							
					TIM1->CCR1 = OpenWave[pwm_index];							// 根据PWM表修改定时器的比较寄存器值
//					TIM1->CCR2 = OpenWave[pwm_index];
//					TIM1->CCR3 = OpenWave[pwm_index];
					if(pwm_index > 0)								
						pwm_index--;											// 标志PWM表的下一个元素
					else
					{
						TIM1->CCR1 = 0;
//						TIM1->CCR2 = 0;
//						TIM1->CCR3 = 0;
					}   
					period_cnt=0;												// 重置周期计数标志
				}
				break;

			default:
				break;
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);									//必须要清除中断标志位
	}
    */
}


/**
  * @brief  风扇
  * @param  None
  * @retval None
  */

void TIM4_IRQHandler(void)
{
	static uint16_t pwm_index = 0;									// 用于PWM查表
	static uint8_t period_cnt = 0;									// 用于计算周期数
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)									//TIM_IT_Update
 	{			
		switch(Pwm_led_status)
		{
			case 1 :		
				period_cnt++;					
				if(period_cnt >= 40)											// 输出的周期数大于20，输出下一种脉冲宽的PWM波
				{									
					if(pwm_index < Brightness_Level)	
					{
						TIM4->CCR2 = OpenWave[pwm_index];						// 根据PWM表修改定时器的比较寄存器值
						pwm_index++;											// 标志PWM表的下一个元素
					}
																	
					if(pwm_index >= Brightness_Level)
					{
						TIM4->CCR2 = OpenWave[Brightness_Level];
					}   
					period_cnt=0;												// 重置周期计数标志
				}
				break;
			
			case 0 :
				period_cnt++;					
				if(period_cnt >= 40)											// 输出的周期数大于20，输出下一种脉冲宽的PWM波
				{							
					TIM4->CCR2 = OpenWave[pwm_index];							// 根据PWM表修改定时器的比较寄存器值
					if(pwm_index > 0)								
						pwm_index--;											// 标志PWM表的下一个元素
					else
					{
						TIM4->CCR2 = 0;
					}   
					period_cnt=0;												// 重置周期计数标志
				}
				break;
			
			default:
				break;
		}
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);									//必须要清除中断标志位
	}
}

/**
  * @brief  按键中断
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler (void)															// KEY2
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

void EXTI4_IRQHandler (void)															// KEY3
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

void EXTI1_IRQHandler (void)															// KEY4
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
	
void EXTI3_IRQHandler (void)															// KEY5
{							
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) 											// 确保是否产生了EXTI Line中断
	{	
        
//		ZigBee1_ON;																		// Zigbee Coordinator供电
//		ZigBee2_ON;																		// Zigbee End_Device1、2供电
		
		
        
        PC_Usart((unsigned char*)"key5 test\n");
        EXTI_ClearITPendingBit(EXTI_Line3);     										// 清除中断标志位
	}  										
}




/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
