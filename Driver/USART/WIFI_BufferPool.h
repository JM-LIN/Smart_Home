/*
 *		缓冲池。自动管理的缓冲器
 */

#ifndef __WIFI_BufferPool__
#define __WIFI_BufferPool__


/************************************************************************
* Include File Section          
************************************************************************/
#include "stm32f10x.h"
#include "main.h"
#include "OLED.h"
#include "usart.h"
#include "GPIO.h"


/************************************************************************
* Macro Define Section          
************************************************************************/
#define 	OPEN					1
#define		CLOSE					0
#define 	STOP					2
#define 	Flow_Size_Max 			24							// 定义最大的溢出大小


/************************************************************************
* Prototype Declare Section     
************************************************************************/
void 		WIFI_BufferPool_Init			(void);						// 初始化缓冲池
void 		WIFI_BPWriteData				(unsigned char dat);      	// 写入数据
void 		WIFI_BPReadData				    (unsigned char* datas);		// 读出数据,datas是调用者分配的数组，接收数据将复制到其中
char 		WIFI_BPCheckFlow				(void);						// 检查是否已经溢出（溢出返回1）
char 		WIFI_BPCheckCancelCmd		    (void);						// 检查是否是取消命令引起溢出（溢出返回1）
void 		WIFI_SetFlowSize				(unsigned char val);		// 设置溢出大小


		



#endif


