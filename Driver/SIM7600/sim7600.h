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

extern  uint8_t     sim900a_cmd         	(char *cmd, char *reply,uint32_t waittime );
extern  uint8_t     sim900a_cmd_check   	(char *reply);

					
#define     SIM900A_CLEAN_RX()              clean_rebuff()
#define     sim900a_ate0()                  sim900a_cmd("ATE0\r","OK",100)              //�رջ���
#define     SIM900A_TX(cmd)                	SIM7600_Usart((unsigned char*)"%s",cmd)
#define     SIM900A_IS_RX()                 (USART_GetFlagStatus(SIM7600_Usart_Prot, USART_FLAG_RXNE) != RESET)
#define     SIM900A_RX(len)                 ((char *)get_rebuff(&(len)))
#define     SIM900A_DELAY(time)             delay_ms(time)                              //��ʱ
#define     SIM900A_SWAP16(data)    		__REVSH(data)

#define     SIM7600_RST_HIGH_LEVEL()        GPIO_SetBits( GPIOD, GPIO_Pin_8 )
#define     SIM7600_RST_LOW_LEVEL()         GPIO_ResetBits( GPIOD, GPIO_Pin_8 )

static void SIM7600_Reset_Init(void);
void SIM7600_IRQHandler(void);
void SIM7600_Init(void);

/*************************** �绰 ���� ***************************/
extern 	void		sim900a_init			(void);								        //��ʼ�������ģ��
extern  void        sim900a_call          	(char *num);                                //���𲦴�绰
        
/***************************  ���Ź���  ****************************/       
extern  void        sim900a_sms             (char *num,char *smstext);                  //���Ͷ���
extern  char *      sim900a_waitask         (uint8_t waitask_hook(void));               //�ȴ�������Ӧ�𣬷��ؽ��ջ�������ַ          
        
/*************************** GPRS ���� ***************************/
extern  void        sim900a_gprs_send        (char *str);                               //��������
extern 	void		server_connect			 (char *ipaddr,char *port);															
extern	char		server_connect_tcp		 (char *ipaddr,char *port);															//TCP����
extern 	void 		bufcheck				 (void);
#endif

