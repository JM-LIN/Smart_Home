#ifndef __KEY_H
#define	__KEY_H


#include "stm32f10x.h"
#include "usart.h"

#define KEY_ON	1
#define KEY_OFF	0


void    Key_Init                    (void);
void    EXTI0_IRQHandler_Routine    (void);
void    EXTI4_IRQHandler_Routine    (void);
void    EXTI1_IRQHandler_Routine    (void);
void    EXTI3_IRQHandler_Routine    (void);
uint8_t Key_Scan                    (GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __EXTI_H */
