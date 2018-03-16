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

#define     SIM7600

#ifdef      SIM7600
#define     SIM7600_Usart_Port 					USART1
#define     SIM7600_Usart( fmt, ... )           USART_printf ( SIM7600_Usart_Port, fmt, ##__VA_ARGS__ ) 
#define     ESP8266_Usart( fmt, ... )           {}
#else
#define     ESP8266_Usart_Port 					USART1
#define     ESP8266_Usart( fmt, ... )           USART_printf ( ESP8266_Usart_Port, fmt, ##__VA_ARGS__ ) 
#define     SIM7600_Usart( fmt, ... )           {}
#endif

#define     ZigBee_Usart_Port 					USART2
#define     Voice_Usart_Port 					USART3
#define     __Usart_Port 					    UART4
#define     PC_Usart_Port 					    UART5

#define 	ZigBee_Usart( fmt, ... )		    USART_printf ( ZigBee_Usart_Port, fmt, ##__VA_ARGS__ )
#define 	Voice_Usart( fmt, ... )		        USART_printf ( Voice_Usart_Port, fmt, ##__VA_ARGS__ )
#define 	__Usart( fmt, ... )		            USART_printf ( __Usart_Port, fmt, ##__VA_ARGS__ )
#define     PC_Usart( fmt, ... )        	    USART_printf ( PC_Usart_Port, fmt, ##__VA_ARGS__ ) 


#define     USART_REC_LEN  			        200  	        //定义最大接收字节数 200
#define     EN_USART1_RX 			        1		        //使能（1）/禁止（0）串口1接收
	  	
       
/************************************************************************
* Prototype Declare Section         
************************************************************************/
 
extern void     USART_Configuration         (void);
extern void     USART_printf                (USART_TypeDef* USARTx, uint8_t *Data, ...);
static void     USART1_Config               (void);
static void     USART2_Config               (void);
void            USART3_Config               (void);
static void     UART4_Config                (void);
static void     UART5_Config                (void);
static void     USART1_NVIC_Configuration   (void);
static void     USART2_NVIC_Configuration   (void);
static void     USART3_NVIC_Configuration   (void);
static void     UART4_NVIC_Configuration    (void);
static void     UART5_NVIC_Configuration    (void);

static char     *itoa                       (int value, char *string, int radix);
int             fputc                       (int ch, FILE *f);
int             fgetc                       (FILE *f);

static void     Usart_SendByte              ( USART_TypeDef * pUSARTx, uint8_t ch );
static uint8_t  USART_Receive_Byte          ( USART_TypeDef * pUSARTx );
void            Usart_SendStr_length        ( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void            Usart_ReceiveStr_length     ( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void            Usart_SendString            ( USART_TypeDef * pUSARTx, uint8_t *str );

#endif


