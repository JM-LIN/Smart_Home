#include "timer.h"

volatile u32 time6_num;

// TIMx,x[6,7]中断优先级配置
static void TIM6_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             // 设置中断组为4
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             // 设置中断来源	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;   // 设置主优先级为 13  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	        // 设置抢占优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         都有
 *	TIM_CounterMode			 TIMx,x[6,7]没有，其他都有
 *  TIM_Period            都有
 *  TIM_ClockDivision     TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */

void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);		// 开启TIMx_CLK,x[6,7],即内部时钟CK_INT=72M	
    TIM_TimeBaseStructure.TIM_Period=1000;                      // 自动重装载寄存器周的值(计数值)   1ms
                                                                // 累计 TIM_Period个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Prescaler= 71;	                // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M

    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             // 初始化定时器TIMx, x[6,7]	
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);                       // 清除计数器中断标志位	  			
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                    // 开启计数器中断				
    TIM_Cmd(TIM6, ENABLE);			                            // 使能计数器															   
		
    TIM6_NVIC_Configuration();
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, DISABLE);      // 暂时关闭TIMx,x[6,7]的时钟，等待使用
}

void TIM6_IRQHandler_Routine (void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		time6_num++;
        if(time6_num == 10000)      // 定时10s上传服务器数据信息
            SIM7600_Data_Send();
        
		TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);  		 
	}		 	
}
