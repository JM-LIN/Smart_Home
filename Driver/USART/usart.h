/************************************************************************
* Multi-Include-Prevent Section     
************************************************************************/
#ifndef __USART_H
#define __USART_H


/************************************************************************
* Include File Section              
************************************************************************/
#include "stm32f10x.h"
#include "stdio.h"	
#include <stdarg.h> 


/************************************************************************
* Macro Define Section              
************************************************************************/
#pragma anon_unions

#define     ESP8266_Usart( fmt, ... )       USART_printf ( USART1, fmt, ##__VA_ARGS__ ) 
#define 	ZigBee_Usart( fmt, ... )		USART_printf ( USART2, fmt, ##__VA_ARGS__ )
#define     PC_Usart( fmt, ... )        	USART_printf ( USART3, fmt, ##__VA_ARGS__ ) 


#define     USART_REC_LEN  			        200  	        //定义最大接收字节数 200
#define     EN_USART1_RX 			        1		        //使能（1）/禁止（0）串口1接收
	  	
       



/************************************************************************
* Prototype Declare Section         
************************************************************************/
 
extern void     USART_Configuration         (void);
extern void     USART_printf                (USART_TypeDef* USARTx, uint8_t *Data, ...);
static void     USART1_Config               (void);
static void     USART2_Config               (void);
static void     USART3_Config               (void);
static void     USART1_NVIC_Configuration   (void);
static void     USART2_NVIC_Configuration   (void);
static void     USART3_NVIC_Configuration   (void);
static char     *itoa                       (int value, char *string, int radix);
int             fputc                       (int ch, FILE *f);
int             fgetc                       (FILE *f);


#endif


