#ifndef _SIM900A_H_
#define _SIM900A_H_

#include "stm32f10x.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "SysTick.h"
#include "usart.h"



typedef enum{
    SIM900A_TRUE,
    SIM900A_FALSE,
    
}sim900a_res_e;



					
#define     SIM900A_CLEAN_RX()              clean_rebuff()
#define     sim900a_ate0()                  SIM7600_CMD("ATE0\r","OK",100)              //�رջ���
#define     SIM900A_TX(cmd)                	SIM7600_Usart((unsigned char*)"%s",cmd)
#define     SIM900A_IS_RX()                 (USART_GetFlagStatus(SIM7600_Usart_Prot, USART_FLAG_RXNE) != RESET)
#define     SIM900A_RX(len)                 ((char *)get_rebuff(&(len)))
#define     SIM900A_DELAY(time)             delay_ms(time)                              //��ʱ
#define     SIM900A_SWAP16(data)    		__REVSH(data)

#define     SIM7600_RST_HIGH_LEVEL()        GPIO_SetBits( GPIOD, GPIO_Pin_8 )
#define     SIM7600_RST_LOW_LEVEL()         GPIO_ResetBits( GPIOD, GPIO_Pin_8 )


void SIM7600_IRQHandler_Routine(void);
void SIM7600_Init(void);
static void SIM7600_Reset_Init(void);
static char *get_rebuff(uint8_t *len);
static void clean_rebuff(void);
uint8_t     SIM7600_CMD         	(char *cmd, char *reply,uint32_t waittime );
uint8_t     SIM7600_CMD_Check   	(char *reply);

/*************************** �绰 ���� ***************************/
void		SIM7600_Config			(void);								        //��ʼ�������ģ��
void        SIM7600_Call          	(char *num);                                //���𲦴�绰
        
/***************************  ���Ź���  ****************************/       
void        SIM7600_SMS             (char *num,char *smstext);                  //���Ͷ���
char *      SIM7600_Waitask         (uint8_t waitask_hook(void));               //�ȴ�������Ӧ�𣬷��ؽ��ջ�������ַ          
        
/*************************** GPRS ���� ***************************/
void        SIM7600_Data_Send       (void);
void        SIM7600_GPRS_Send       (char *str);                               //��������
void		server_connect			(char *ipaddr,char *port);															
char		server_connect_tcp		(char *ipaddr,char *port);															//TCP����
static void 		bufcheck		(void);
#endif

