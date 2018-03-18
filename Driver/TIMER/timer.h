#ifndef TIMER_H__
#define TIMER_H__

#include "stm32f10x.h"
#include "SIM7600.h"
#include "SYN7318.h"


/**************************º¯ÊýÉùÃ÷********************************/
static void     TIM6_NVIC_Configuration             (void);
void            TIM6_Configuration                  (void);
void            TIM6_IRQHandler_Routine             (void);


#endif
