#include "timer.h"
#include "led.h"
#include "rtc.h"
#include "spi.h"
#include "flash.h"
#include "usart.h"





//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!


extern u8 week1,week2,week3;	
extern u8 day1,day2,day3;
extern long int sec1,sec2,sec3;//定时器1,2,3的
extern u32 FLASH_SIZE;//flash的总大小空间


u8 del_data_flag1 = 0;
u8 del_data_flag2 = 0;
u8 del_data_flag3 = 0;


u8 write_month;//写入flash的月份
u8 write_data;//写入flash的日期
	
	
u8 sec_num;
	
void Week1_Del(void)
{
	u8 Week1_Del = 0;
	vu16 i;
	for(i = 0;i<1300;i++)
	{
		W25QXX_Write(&Week1_Del,FLASH_SIZE - 600000+i,sizeof(Week1_Del));
	}
}

void Week2_Del(void)
{
	u8 Week2_Del = 0;
	vu16 i;
	for(i = 0;i<1300;i++)
	{
		W25QXX_Write(&Week2_Del,FLASH_SIZE - 800000+i,sizeof(Week2_Del));
	}
}

void Week3_Del(void)
{
	u8 Week3_Del = 0;
	vu16 i;
	for(i = 0;i<1300;i++)
	{
		W25QXX_Write(&Week3_Del,FLASH_SIZE - 1000000+i,sizeof(Week3_Del));
	}
}

	
void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 
}

//定时器2中断服务程序
//void TIM2_IRQHandler(void)   //TIM2中断
//{
//	sec1++;
//	if(sec1 == 172800)
//	{
//		sec1 = 0;
//		day1++;
//		day2++;
//		day3++;
//		
//		if(day1 == 14)  //两个星期清理一次
//		{
//			del_data_flag1 = 1;
//		}
//		if(day2 == 28)
//		{
//			del_data_flag2 = 1;
//		}
//		if(day3 == 42)
//		{
//			del_data_flag3 = 1;
//		}
//		if(del_data_flag1 == 1)
//		{
////			printf("进入第一周的擦除");
////			printf("\n\r");
//			Week1_Del();
//			del_data_flag1 =0;
////			printf("第一周清理成功\n\r");
////			printf("\n\r");
//		}
//		if(del_data_flag2 == 1)
//		{
//	//		printf("进入第二周的擦除\n\r");
////			printf("\n\r");
//			Week2_Del();
//			del_data_flag2 =0;
//	//		printf("第二周清理成功\n\r");
////			printf("\n\r");
//		}
//		if(del_data_flag3 == 1)
//		{
////			printf("进入第三周的擦除\n\r");
////			printf("\n\r");
//			Week3_Del();
//			del_data_flag3 =0;
////			printf("第三周清理成功\n\r");
////			printf("\n\r");
//		}
//		if(day1 == 42) 
//		{
//			day1 = 1;
//		}
//		if(day2 == 42) 
//		{
//			day2 = 1;
//		}
//		if(day3 == 42) 
//		{
//			day3 = 1;
//		}
//	}
//	if(sec1 == 2)      //测试是否当天0点
//	{
//		if( (calendar.sec == 0) && (calendar.min ==0) && (calendar.hour==0) )
//		{
//			if(day1 == 1)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000 + 1,sizeof(write_data));//写入日期
//			}
//			if(day1 == 2)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+200 + 1,sizeof(write_data));//写入日期
//			}
//			if(day1 == 3)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+400 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+400 + 1,sizeof(write_data));//写入日期
//			}
//			if(day1 == 4)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+600 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+600 + 1,sizeof(write_data));//写入日期
//			}
//			if(day1 == 5)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+800 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+800 + 1,sizeof(write_data));//写入日期
//			}
//			if(day1 == 6)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+1000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+1000 + 1,sizeof(write_data));//写入日期
//			}
//			if(day1 == 7)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-600000+1200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-600000+1200 + 1,sizeof(write_data));//写入日期
//			}
//			
//			if(day2 == 8)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000 + 1,sizeof(write_data));//写入日期
//			}
//			if(day2 == 9)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+200 + 1,sizeof(write_data));//写入日期
//			}
//			if(day2 == 10)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+400 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+400 + 1,sizeof(write_data));//写入日期
//			}
//			if(day2 == 11)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+600 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+600 + 1,sizeof(write_data));//写入日期
//			}
//			if(day2 == 12)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+800 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+800 + 1,sizeof(write_data));//写入日期
//			}
//			if(day2 == 13)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+1000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+1000 + 1,sizeof(write_data));//写入日期
//			}
//			if(day2 == 14)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-800000+1200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-800000+1200 + 1,sizeof(write_data));//写入日期
//			}
//			if(day3 == 15)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000 + 1,sizeof(write_data));//写入日期
//			}
//			if(day3 == 16)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+200 + 1,sizeof(write_data));//写入日期
//			}
//			if(day3 == 17)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+400 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+400 + 1,sizeof(write_data));//写入日期
//			}
//			if(day3 == 18)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000 +600+ 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000 +600+ 1,sizeof(write_data));//写入日期
//			}
//			if(day3 == 19)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+800 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+800 + 1,sizeof(write_data));//写入日期
//			}
//			if(day3 == 20)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+1000 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+1000 + 1,sizeof(write_data));//写入日期
//			}
//			if(day3 == 21)
//			{
//				write_month = calendar.w_month;
//				write_data = calendar.w_date;
//				W25QXX_Write(&write_month,FLASH_SIZE-1000000+1200 + 0,sizeof(write_month));
//				W25QXX_Write(&write_data, FLASH_SIZE-1000000+1200 + 1,sizeof(write_data));//写入日期
//			}
//		}		
//	}
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
//	{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
//	}
//}
	


void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);// 清除计数器中断标志位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}



//void TIM5_Int_Init(u16 arr,u16 psc)
//{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
//	
//	//定时器TIM3初始化
//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

//	//中断优先级NVIC设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


//	TIM_Cmd(TIM5, ENABLE);  //使能TIMx					 
//}





void PWM_Init(u16 arr,u16 psc)
{   
	
	/********************* TIM4_CH1 -- CH4   PWM 配置  ***********************/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//使能定时器1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
		//设置该引脚为复用输出功能,输出TIM4 CH1~CH4  的PWM脉冲波形	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
  //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM4 Channel2 PWM模式	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC1
//	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC3
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
	
		/********************* TIM2_CH4   PWM 配置  ***********************/


//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//使能定时器2时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	    
// 
//	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2  , ENABLE);
//	//设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形	GPIOB.11
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM4_CH1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
// 
// 
// 
//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	//初始化TIM4 Channel2 PWM模式	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
//	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC1
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
// 
//	TIM_Cmd(TIM2, ENABLE);  //使能TIM4
//	TIM_CtrlPWMOutputs(TIM2, ENABLE);
}





