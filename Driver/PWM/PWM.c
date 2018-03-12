#include "PWM.h"



/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Config_PWM_TIM4(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����TIM4_IRQ�ж�Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//  ���
//  TIM4 PWM���ֳ�ʼ�� 
//  PWM�����ʼ��
//  arr���Զ���װֵ
//  psc��ʱ��Ԥ��Ƶ��
void Servo_Door_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;                            //GPIO�ṹ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;                 //������ʱ���Ľṹ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //ͨ���ṹ�������
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	        //ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	
   //���ø�����Ϊ�����������,���TIM4 CH4��PWM���岨��	GPIOB.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                       //LED0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //��ʼ��GPIO

   //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period = arr;                         //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                       //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channel4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);                        //����Tָ���Ĳ�����ʼ������TIM4 OC4

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);               //ʹ��TIM4��CCR4�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM4, ENABLE);                                          //ʹ��TIM4
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

void Servo_Window_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;                            //GPIO�ṹ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;                 //������ʱ���Ľṹ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //ͨ���ṹ�������
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	        //ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);           //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	
   //���ø�����Ϊ�����������,���TIM4 CH3��PWM���岨��	GPIOD.14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;                       //LED0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           
	GPIO_Init(GPIOD, &GPIO_InitStructure);                          //��ʼ��GPIO

   //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period = arr;                         //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                       //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channel3 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);                        //����Tָ���Ĳ�����ʼ������TIM4 OC3

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);               //ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM4, ENABLE);                                          //ʹ��TIM4
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

void Fan_Init(u16 arr,u16 psc)                                      // (255,1999) ��ʱ����0������255����Ϊ266�Σ�Ϊһ����ʱ����
{   
	GPIO_InitTypeDef GPIO_InitStructure;                            //GPIO�ṹ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;                 //������ʱ���Ľṹ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //ͨ���ṹ�������
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	        //ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);           //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	
   //���ø�����Ϊ�����������,���TIM4 CH2��PWM���岨��	GPIOD.13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                       //LED0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           
	GPIO_Init(GPIOD, &GPIO_InitStructure);                          //��ʼ��GPIO

    //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period = arr;                         //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                       //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //�Ƚ����ʹ��
//    TIM_OCInitStructure.TIM_Pulse = 0;								//���ó�ʼPWM������Ϊ0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);                        //����Tָ���Ĳ�����ʼ������TIM4 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);               //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
 
    TIM_ARRPreloadConfig(TIM4, ENABLE);			 					//��TIM4���ؼĴ���ARR
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);						//ʹ��update�ж�
    NVIC_Config_PWM_TIM4();
    
	TIM_Cmd(TIM4, ENABLE);                                          //ʹ��TIM4
}

