#ifndef __GPIO_H_
#define __GPIO_H_

#include "stm32f10x.h"
#include "Systick.h"
					
/* Ö±½Ó²Ù×÷¼Ä´æÆ÷µÄ·½·¨¿ØÖÆIO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//ÉèÖÃÎª¸ßµçÆ½		
#define digitalLo(p,i)			{p->BRR=i;}				//Êä³öµÍµçÆ½
#define digitalToggle(p,i)		{p->ODR ^=i;}			//Êä³ö·´×ª×´Ì

/* ¶¨Òå¿ØÖÆIOµÄºê */
#define LED1_TOGGLE()		digitalToggle(GPIOB,GPIO_Pin_1)
#define LED1_OFF()		    digitalHi(GPIOB,GPIO_Pin_1)
#define LED1_ON()			digitalLo(GPIOB,GPIO_Pin_1)

#define LED2_TOGGLE()		digitalToggle(GPIOC,GPIO_Pin_5)
#define LED2_OFF()		    digitalHi(GPIOC,GPIO_Pin_5)
#define LED2_ON()			digitalLo(GPIOC,GPIO_Pin_5)

#define ZigBee1_OFF()		digitalLo(GPIOC,GPIO_Pin_1)
#define ZigBee1_ON()		digitalHi(GPIOC,GPIO_Pin_1)

#define ZigBee2_OFF()		digitalLo(GPIOC,GPIO_Pin_0)
#define ZigBee2_ON()		digitalHi(GPIOC,GPIO_Pin_0)

#define ZigBee3_OFF()		digitalLo(GPIOF,GPIO_Pin_5)
#define ZigBee3_ON()		digitalHi(GPIOF,GPIO_Pin_5)

#define Humidifier_OFF()	digitalLo(GPIOC,GPIO_Pin_2)
#define Humidifier_ON()		digitalHi(GPIOC,GPIO_Pin_2)

#define WIFI_OFF()		    digitalLo(GPIOC,GPIO_Pin_9)
#define WIFI_ON()		    digitalHi(GPIOC,GPIO_Pin_9)

#define Humidi_TOGGLE()	    digitalToggle(GPIOC,GPIO_Pin_2)
#define Humidi_OFF()		digitalLo(GPIOC,GPIO_Pin_2)
#define Humidi_ON()		    digitalHi(GPIOC,GPIO_Pin_2)

#define Fan_TOGGLE()		digitalToggle(GPIOB,GPIO_Pin_10)
#define Fan_OFF()			digitalLo(GPIOB,GPIO_Pin_10)
#define Fan_ON()			digitalHi(GPIOB,GPIO_Pin_10)

#define Beep_TOGGLE()		digitalToggle(GPIOB,GPIO_Pin_2)
#define Beep_OFF()		    digitalHi(GPIOB,GPIO_Pin_2)
#define Beep_ON()			digitalLo(GPIOB,GPIO_Pin_2)

void LED_Init(void);
void Beep_Init(void);
void ZigBee_Power_Init(void);
void Humidifier_Init(void);
void Infrared_Init(void);

#endif

