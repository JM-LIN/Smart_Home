/************************************************************************
* Multi-Include-Prevent Section     
************************************************************************/
#ifndef BSP_INIT_H
#define BSP_INIT_H

/************************************************************************
* Include File Section              
************************************************************************/
#include "stm32f10x.h"
#include "SysTick.h"
#include "usart.h"
#include "GPIO.h"
#include "wifi.h"
#include "WIFI_BufferPool.h"
#include "ZigBee_BufferPool.h"
#include "OLED.h"
#include "exti.h"
#include "PWM.h"
#include "timer.h"
#include "Breathing_Light.h"
#include "rc522.h"
#include "opp.h"
#include "SYN7318.h"
#include "SIM7600.h"
#include "fpmxx_modules.h"


/************************************************************************
* Prototype Declare Section     
************************************************************************/
extern void BSP_Init(void);
extern void Breathing_Light_Init(void);


#endif

