#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"

//#define LED0_ON 	PBout(0)
//#define LED0_OFF 	PBin(0)
//#define LED1_ON 	PBout(1)
//#define LED1_OFF 	PBin(1)
#define ON  0
#define OFF 1

#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_5)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_4)
					
#define Power1(a)	if (a)	\
					GPIO_ResetBits(GPIOC,GPIO_Pin_1);\
					else		\
					GPIO_SetBits(GPIOC,GPIO_Pin_1)
					
#define Power2(a)	if (a)	\
					GPIO_ResetBits(GPIOC,GPIO_Pin_0);\
					else		\
					GPIO_SetBits(GPIOC,GPIO_Pin_0)
					
#define Power3(a)	if (a)	\
					GPIO_ResetBits(GPIOC,GPIO_Pin_2);\
					else		\
					GPIO_SetBits(GPIOC,GPIO_Pin_2)
					
#define Beep(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_2);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_2)
					
/* Ö±½Ó²Ù×÷¼Ä´æÆ÷µÄ·½·¨¿ØÖÆIO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//ÉèÖÃÎª¸ßµçÆ½		
#define digitalLo(p,i)			{p->BRR=i;}				//Êä³öµÍµçÆ½
#define digitalToggle(p,i)		{p->ODR ^=i;}			//Êä³ö·´×ª×´Ì

/* ¶¨Òå¿ØÖÆIOµÄºê */
#define LED1_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_1)
#define LED1_OFF		digitalHi(GPIOB,GPIO_Pin_1)
#define LED1_ON			digitalLo(GPIOB,GPIO_Pin_1)

#define LED2_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_5)
#define LED2_OFF		digitalHi(GPIOC,GPIO_Pin_5)
#define LED2_ON			digitalLo(GPIOC,GPIO_Pin_5)

#define Power1_TOGGLE	digitalToggle(GPIOC,GPIO_Pin_1)
#define Power1_OFF		digitalLo(GPIOC,GPIO_Pin_1)
#define Power1_ON		digitalHi(GPIOC,GPIO_Pin_1)

#define Power2_TOGGLE	digitalToggle(GPIOC,GPIO_Pin_0)
#define Power2_OFF		digitalLo(GPIOC,GPIO_Pin_0)
#define Power2_ON		digitalHi(GPIOC,GPIO_Pin_0)

#define WIFI_TOGGLE	    digitalToggle(GPIOC,GPIO_Pin_9)
#define WIFI_OFF		digitalLo(GPIOC,GPIO_Pin_9)
#define WIFI_ON		    digitalHi(GPIOC,GPIO_Pin_9)

#define Humidi_TOGGLE	digitalToggle(GPIOC,GPIO_Pin_2)
#define Humidi_OFF		digitalLo(GPIOC,GPIO_Pin_2)
#define Humidi_ON		digitalHi(GPIOC,GPIO_Pin_2)

#define Fan_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_10)
#define Fan_OFF			digitalLo(GPIOB,GPIO_Pin_10)
#define Fan_ON			digitalHi(GPIOB,GPIO_Pin_10)

#define Beep_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_2)
#define Beep_OFF		digitalHi(GPIOB,GPIO_Pin_2)
#define Beep_ON			digitalLo(GPIOB,GPIO_Pin_2)



#endif

