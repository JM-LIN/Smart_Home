/*************************************************************************
* File Name : main.c
* Copyright : 2003-2008 XXXX Corporation, All Rights Reserved.
* Module Name : Draw Engine/Display
*
* CPU : STM32F103
* RTOS : None
*
* Create Date : 2017/10/04
* Author/Corporation : JM.Lin
*
* Abstract Description : Smart Yacht
*
*-----------------------Revision History---------------------------------
* No Version Date Revised By Item Description
* 1 V0.95 17.10.04 JM.Lin abcdefghijklm WhatUDo
*
************************************************************************/

/************************************************************************
* Include File Section          
************************************************************************/
#include "stm32f10x.h"
#include "main.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
int main(void)
{	
	BSP_Init();
    TIM_Cmd(TIM4, ENABLE);

    TIM_SetCompare2(TIM4, 500);
    while(1)
    {
        Swiping_Card();
        Voice_Control();
        FPM_Running();
    }
    
    return 0;
}
/*********************************************END OF FILE**********************/
