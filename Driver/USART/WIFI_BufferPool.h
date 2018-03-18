/*
 *		����ء��Զ�����Ļ�����
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
#define 	Flow_Size_Max 			24							// �������������С


/************************************************************************
* Prototype Declare Section     
************************************************************************/
void 		WIFI_BufferPool_Init			(void);						// ��ʼ�������
void 		WIFI_BPWriteData				(unsigned char dat);      	// д������
void 		WIFI_BPReadData				    (unsigned char* datas);		// ��������,datas�ǵ����߷�������飬�������ݽ����Ƶ�����
char 		WIFI_BPCheckFlow				(void);						// ����Ƿ��Ѿ�������������1��
char 		WIFI_BPCheckCancelCmd		    (void);						// ����Ƿ���ȡ����������������������1��
void 		WIFI_SetFlowSize				(unsigned char val);		// ���������С


		



#endif


