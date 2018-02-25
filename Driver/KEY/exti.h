#ifndef __KEY_H
#define	__KEY_H


#include "stm32f10x.h"

#define KEY_ON	1
#define KEY_OFF	0

extern void Key_Init(void);
extern uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __EXTI_H */
