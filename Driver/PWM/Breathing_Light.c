/**
  ******************************************************************************
  * @file    Breathing.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   tim1�����·pwm bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f10x.h"




/* LED���ȵȼ� PWM�� */
uint8_t OpenWave[] = {0,1,2,3,4,5,5,6,7,8,9,9,
	10,11,12,13,13,14,15,16,17,17,18,19,20,21,21,22,23,24,25,25,26,27,28,28,29,30,31,32,32,33,34,35,36,36,37,
	38,39,40,40,41,42,43,44,44,45,46,47,48,48,49,50,51,52,52,53,54,55,56,56,57,58,59,60,60,61,62,63,64,64,65,
	66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,
	100,106,112,118,124,125,131,137,143,149,150,151,157,163,170,176,182,188,194,200,206,212,218,224,230,236,
	242,248,254,255};

 /**
  * @brief  ����TIM1�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIM1_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA clock enable */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE );

    /* ���ú������õ������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 ;             // PE9
	GPIO_Init( GPIOE, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 ;            // PE11
	GPIO_Init( GPIOE, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 ;            // PE13
	GPIO_Init( GPIOE, &GPIO_InitStructure );
}


/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Config_PWM(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����TIM1_CC_IRQn�ж�Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;          // TIM1_UP_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ����TIM1�����PWM�źŵ�ģʽ�������ڡ�����
  * @param  ��
  * @retval ��
  */

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> �ж� & TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 *
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * 
 */

/*    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 */
static void TIM1_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				

	/* ����TIM2CLK ʱ��Ϊ72MHZ */				
	RCC_APB1PeriphClockCmd ( RCC_APB2Periph_TIM1, ENABLE );						//ʹ��TIM1ʱ��

	/* ������ʱ������ */		 
	TIM_TimeBaseStructure.TIM_Period = 35;       							  	//����ʱ����0������255����Ϊ266�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;	    							//����Ԥ��Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;					//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  				//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* PWMģʽ���� */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    					//����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;				//ʹ�����
	TIM_OCInitStructure.TIM_Pulse = 0;										  	//���ó�ʼPWM������Ϊ0	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  				//����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ

	TIM_OC1Init( TIM1, &TIM_OCInitStructure );	 								//ʹ��ͨ��1
    TIM_OC2Init( TIM1, &TIM_OCInitStructure );	 								//ʹ��ͨ��2
    TIM_OC3Init( TIM1, &TIM_OCInitStructure );	 								//ʹ��ͨ��3

	TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Enable );							//ʹ��Ԥװ��	
    TIM_OC2PreloadConfig( TIM1, TIM_OCPreload_Enable );
    TIM_OC3PreloadConfig( TIM1, TIM_OCPreload_Enable );

	TIM_ARRPreloadConfig(TIM1, ENABLE);			 								//��TIM1���ؼĴ���ARR	
	
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);									//ʹ��update�ж�
		
	NVIC_Config_PWM();															//�����ж����ȼ�	

	/* TIM1 disble counter */
	TIM_Cmd(TIM1, ENABLE);                   									//�رն�ʱ��1
}

/**
  * @brief  TIM2 �����Ƴ�ʼ��
  *         ����PWMģʽ��GPIO
  * @param  ��
  * @retval ��
  */
void Breathing_Light_Init(void)
{
	TIM1_GPIO_Config();
	TIM1_Mode_Config();	
}


/*********************************************END OF FILE**********************/

