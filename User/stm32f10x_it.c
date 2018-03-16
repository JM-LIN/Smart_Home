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
#include "usart.h"
#include "wifi.h"
#include "WIFI_BufferPool.h"
#include "ZigBee_BufferPool.h"
#include "opp.h"
#include "card_records.h"
#include "Breathing_Light.h"
#include "timer.h"
#include "exti.h"
#include "SIM7600.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



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
    PC_Usart((unsigned char*)"Hard Fault exception occurs !!!\n");
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
    PC_Usart((unsigned char*)"Memory Manage exception occurs !!!\n");
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
    PC_Usart((unsigned char*)"Bus Fault exception occurs !!!\n");
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
    PC_Usart((unsigned char*)"Usage Fault exception occurs !!!\n");
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
#ifdef SIM7600
    SIM7600_IRQHandler_Routine();
#else
    WIFI_IRQHandler();
#endif
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)															// 用于ZigBee通信
{													
	ZigBee_IRQHandler_Routine();
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*
void USART3_IRQHandler(void)															// 用于PC机通信
{							
	volatile uint8_t ch;							
								
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)							
	{ 				 				
		//ch = USART1->DR;							
		ch = USART_ReceiveData(USART3);							
//		USART_SendData(USART2, ch);   													// 将接收到的数据分别送到ZigBee、ESP8266中
//		USART_SendData(USART1, ch);							
	}  							
	USART_ClearFlag(USART3,USART_FLAG_TC);												// 清除中断标志.
}	
*/

/**
  * @brief  This function handles PPP interrupt request. PC_Usart
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
    volatile uint8_t ch;
    
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{ 	             
        ch = USART_ReceiveData(USART1);
        USART_SendData(UART5, ch);  
	} 	 
    USART_ClearFlag(UART5,USART_FLAG_TC);												// 清除中断标志.
}


/**
  * @brief  呼吸灯
  * @param  None
  * @retval None
  */

void TIM1_UP_IRQHandler(void)		
{
	Breathing_Light_IROHandler_Routine();
}



/**
  * @brief  风扇
  * @param  None
  * @retval None
  */

void TIM4_IRQHandler(void)
{
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)									//TIM_IT_Update
 	{			
		
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);									//必须要清除中断标志位
	}
}

/**
  * @brief  This function handles TIM6 interrupt request. 定时器6 计时
  * @param  None
  * @retval None
  */
void  TIM6_IRQHandler (void)
{
	TIM6_IRQHandler_Routine();	 	
}

/**
  * @brief  按键中断 
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler (void)	// key2														// 
{							
	EXTI0_IRQHandler_Routine();									
}

void EXTI4_IRQHandler (void)	// key3														// KEY3
{							
	EXTI4_IRQHandler_Routine();								
}

void EXTI1_IRQHandler (void)	// key4														// KEY4
{							
	EXTI1_IRQHandler_Routine();										
}										
	
void EXTI3_IRQHandler (void)	// key5														// KEY5
{							
	EXTI3_IRQHandler_Routine();								
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
