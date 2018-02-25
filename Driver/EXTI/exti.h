#ifndef __EXTI_H_
#define __EXTI_H_
#include "sys.h"


void RTC_NVIC_Config(void);
void KEY_EXTI_Init(void);
void KEY_NVIC_Init(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);



#endif



