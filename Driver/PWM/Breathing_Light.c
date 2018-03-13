/**
  ******************************************************************************
  * @file    Breathing.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   tim1输出四路pwm bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "Breathing_Light.h"

uint8_t OpenWave[] = {1,1,2,2,3,4,6,8,10,14,19,25,33,44,59,80,
	107,143,191,255,255,191,143,107,80,59,44,33,25,19,14,10,8,6,4,3,2,2,1,1};

/* LED亮度等级 PWM表 *//*
uint8_t OpenWave[] = {0,1,2,3,4,5,5,6,7,8,9,9,
	10,11,12,13,13,14,15,16,17,17,18,19,20,21,21,22,23,24,25,25,26,27,28,28,29,30,31,32,32,33,34,35,36,36,37,
	38,39,40,40,41,42,43,44,44,45,46,47,48,48,49,50,51,52,52,53,54,55,56,56,57,58,59,60,60,61,62,63,64,64,65,
	66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,
	100,106,112,118,124,125,131,137,143,149,150,151,157,163,170,176,182,188,194,200,206,212,218,224,230,236,
	242,248,254,255};
*/
 /**
  * @brief  配置TIM1复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIM1_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOE clock enable */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );

    /* 配置呼吸灯用到的引脚 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 ;             // PE9
	GPIO_Init( GPIOE, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 ;            // PE11
	GPIO_Init( GPIOE, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 ;            // PE13
	GPIO_Init( GPIOE, &GPIO_InitStructure );
}


/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Config_PWM(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 配置TIM1_UP_IRQn中断为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;          
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2
    ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  配置TIM1输出的PWM信号的模式，如周期、极性
  * @param  无
  * @retval 无
  */

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> 中断 & TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 *
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 
 */

/*    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 */
static void TIM1_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				

    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);                           // 定时器1完全重映射
	/* 设置TIM2CLK 时钟为72MHZ */				
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_TIM1, ENABLE );						//使能TIM1时钟
	

	/* 基本定时器配置 */		 
	TIM_TimeBaseStructure.TIM_Period = 255;       							  	//当定时器从0计数到255，即为256次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;	    							//设置预分频：
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;					//设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  				//向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* PWM模式配置 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    					//配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;				//使能输出
	TIM_OCInitStructure.TIM_Pulse = 0;										  	//设置初始PWM脉冲宽度为0	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  				//当定时器计数值小于CCR1_Val时为低电平
	TIM_OC1Init( TIM1, &TIM_OCInitStructure );	 								//使能通道1
    TIM_OC2Init( TIM1, &TIM_OCInitStructure );	 								//使能通道2
    TIM_OC3Init( TIM1, &TIM_OCInitStructure );	 								//使能通道3

	TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Enable );							//使能预装载	
    TIM_OC2PreloadConfig( TIM1, TIM_OCPreload_Enable );
    TIM_OC3PreloadConfig( TIM1, TIM_OCPreload_Enable );

	TIM_ARRPreloadConfig(TIM1, ENABLE);			 								//能TIM1重载寄存器ARR	

	/* TIM1 enble counter */
	TIM_Cmd(TIM1, ENABLE);                   									//打开定时器1
    
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);									//使能update中断
    NVIC_Config_PWM();															//配置中断优先级
    TIM_CtrlPWMOutputs(TIM1, ENABLE);                                            // 使能PWM输出
}

/**
  * @brief  TIM2 呼吸灯初始化
  *         配置PWM模式和GPIO
  * @param  无
  * @retval 无
  */
void Breathing_Light_Init(void)
{
	TIM1_GPIO_Config();
	TIM1_Mode_Config();	
}


/*********************************************END OF FILE**********************/

