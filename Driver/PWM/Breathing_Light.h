#ifndef _BREATHING_LIGHT_H_
#define _BREATHING_LIGHT_H_

#include "stm32f10x.h"

#define CLOSE   0
#define OPEN    1

extern volatile unsigned int Pwm_led_status;				// ºôÎüµÆ×´Ì¬
extern uint8_t OpenWave[];

void Breathing_Light_Init(void);

#endif
