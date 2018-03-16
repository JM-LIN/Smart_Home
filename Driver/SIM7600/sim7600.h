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
#define     sim900a_ate0()                  sim900a_cmd("ATE0\r","OK",100)              //关闭回显
#define     SIM900A_TX(cmd)                	SIM7600_Usart((unsigned char*)"%s",cmd)
#define     SIM900A_IS_RX()                 (USART_GetFlagStatus(SIM7600_Usart_Prot, USART_FLAG_RXNE) != RESET)
#define     SIM900A_RX(len)                 ((char *)get_rebuff(&(len)))
#define     SIM900A_DELAY(time)             delay_ms(time)                              //延时
#define     SIM900A_SWAP16(data)    		__REVSH(data)

#define     SIM7600_RST_HIGH_LEVEL()        GPIO_SetBits( GPIOD, GPIO_Pin_8 )
#define     SIM7600_RST_LOW_LEVEL()         GPIO_ResetBits( GPIOD, GPIO_Pin_8 )

static void SIM7600_Reset_Init(void);
void SIM7600_IRQHandler(void);
void SIM7600_Init(void);

/*************************** 电话 功能 ***************************/
extern 	void		sim900a_init			(void);								        //初始化并检测模块
extern  void        sim900a_call          	(char *num);                                //发起拨打电话
        
/***************************  短信功能  ****************************/       
extern  void        sim900a_sms             (char *num,char *smstext);                  //发送短信
extern  char *      sim900a_waitask         (uint8_t waitask_hook(void));               //等待有数据应答，返回接收缓冲区地址          
        
/*************************** GPRS 功能 ***************************/
extern  void        sim900a_gprs_send        (char *str);                               //发送数据
extern 	void		server_connect			 (char *ipaddr,char *port);															
extern	char		server_connect_tcp		 (char *ipaddr,char *port);															//TCP连接
extern 	void 		bufcheck				 (void);
#endif

