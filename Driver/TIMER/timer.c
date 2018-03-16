#include "timer.h"

volatile u32 time6_num;

// TIMx,x[6,7]�ж����ȼ�����
static void TIM6_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             // �����ж���Ϊ4
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             // �����ж���Դ	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;   // ���������ȼ�Ϊ 13  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	        // ������ռ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         ����
 *	TIM_CounterMode			 TIMx,x[6,7]û�У���������
 *  TIM_Period            ����
 *  TIM_ClockDivision     TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */

void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);		// ����TIMx_CLK,x[6,7],���ڲ�ʱ��CK_INT=72M	
    TIM_TimeBaseStructure.TIM_Period=1000;                      // �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)   1ms
                                                                // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Prescaler= 71;	                // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M

    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             // ��ʼ����ʱ��TIMx, x[6,7]	
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);                       // ����������жϱ�־λ	  			
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                    // �����������ж�				
    TIM_Cmd(TIM6, ENABLE);			                            // ʹ�ܼ�����															   
		
    TIM6_NVIC_Configuration();
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, DISABLE);      // ��ʱ�ر�TIMx,x[6,7]��ʱ�ӣ��ȴ�ʹ��
}

void TIM6_IRQHandler_Routine (void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		time6_num++;
        if(time6_num == 10000)      // ��ʱ10s�ϴ�������������Ϣ
            SIM7600_Data_Send();
        
		TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);  		 
	}		 	
}
