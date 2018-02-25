/************************************************************************
* Multi-Include-Prevent Section     
************************************************************************/
#ifndef _WIFI_FUNCTION_H
#define _WIFI_FUNCTION_H


/************************************************************************
* Include File Section              
************************************************************************/
#include "stm32f10x.h"
#include <stdbool.h>
#include <string.h> 
#include <stdio.h>  

#include "SysTick.h"
//#include <stdlib.h>
//#include "usart.h"
#include "OLED.h"


/************************************************************************
* Macro Define Section              
************************************************************************/
#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


/********************************** �û���Ҫ���õĲ���**********************************/
#define     User_ESP8266_ApSsid           "Smart_Phone" //Ҫ���ӵ��ȵ�����ƣ���WIFI����
#define     User_ESP8266_ApPwd            "123321556abc" //Ҫ���ӵ��ȵ����Կ
#define     User_ESP8266_TcpServer_IP     "144.168.61.37" //Ҫ���ӵķ������� IP�������Ե�IP
#define     User_ESP8266_TcpServer_Port    "12346"  //Ҫ���ӵķ������Ķ˿�
#define     TURE 		1
#define     FALSE 		0
       
#define     ESP8266_CH_HIGH_LEVEL()             GPIO_SetBits( GPIOA, GPIO_Pin_0 )
#define     ESP8266_CH_LOW_LEVEL()              GPIO_ResetBits( GPIOA, GPIO_Pin_0 )      
#define     ESP8266_RST_HIGH_LEVEL()            GPIO_SetBits( GPIOA, GPIO_Pin_1 )
#define     ESP8266_RST_LOW_LEVEL()             GPIO_ResetBits( GPIOA, GPIO_Pin_1 )

#define     RX_BUF_MAX_LEN                      1024                

//����봮���жϽ��գ��벻Ҫע�����º궨��//�����ջ����ֽ���
#define     ESP8266_Usart_Prot 					USART1
#define     ESP8266_Usart( fmt, ... )           USART_printf ( ESP8266_Usart_Prot, fmt, ##__VA_ARGS__ ) 
#define     PC_Usart_Prot 						USART3
#define     PC_Usart( fmt, ... )                USART_printf ( PC_Usart_Prot, fmt, ##__VA_ARGS__ ) 

/************************************************************************
* Struct Define Section             
************************************************************************/
typedef enum{			//ESP8266ʹ��ģʽ��һ������STA_APģʽ
	STA,
	AP,
	STA_AP
}ENUM_Net_ModeTypeDef;

typedef enum{   //ʹ��TCP����UDP
	enumTCP,
	enumUDP
}ENUM_NetPro_TypeDef;

typedef enum{
	OPEN 			= 0,
	WEP 			= 1,
	WPA_PSK 		= 2,
	WPA2_PSK 		= 3,
	WPA_WPA2_PSK 	= 4,
} ENUM_AP_PsdMode_TypeDef;

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0   = 5,
}ENUM_ID_NO_TypeDef;

extern struct  STRUCT_USARTx_Fram                                  //��������֡�Ĵ���ṹ��
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];	
	union {
	__IO u16 InfAll;
	struct {
		  __IO u16 FramLength       :15;                               // 14:0    ���Ϊ15bit
		  __IO u16 FramFinishFlag   :1;                                // 15 	  ���Ϊ1bit
	  } InfBit;
	}; 	
} strEsp8266_Fram_Record;



/************************************************************************
* Prototype Declare Section         
************************************************************************/
void 					 WiFi_Init							 (ENUM_Net_ModeTypeDef mode);
void                     ESP8266_Init                        ( void );
void                     ESP8266_Rst                         ( void );
bool                     ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, u32 waittime );
void                     ESP8266_AT_Test                     ( void );
bool                     ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
bool                     ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
bool                     ESP8266_BuildAP                     ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool                     ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
bool                     ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool                     ESP8266_StartOrShutServer           ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t                  ESP8266_Get_LinkStatus              ( void );
uint8_t                  ESP8266_Get_IdLinkStatus            ( void );
uint8_t                  ESP8266_Inquire_ApIp                ( char * pApIp, uint8_t ucArrayLength );
bool                     ESP8266_UnvarnishSend               ( void );
void                     ESP8266_ExitUnvarnishSend           ( void );
bool                     ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, unsigned char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
char *                   ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );

#endif


