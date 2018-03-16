/*
 *		ZigBee����ء��Զ�����Ļ�����
 */
 
#ifndef __ZigBee_BufferPool_H_
#define __ZigBee_BufferPool_H_

/************************************************************************
* Include File Section         
************************************************************************/
#include "stm32f10x.h"
#include <string.h>
#include "OLED.h"
#include "led.h"

/************************************************************************
* Macro Define Section         
************************************************************************/
#define     ZigBee_Flow_Size_Max        32                          // �������������С
#define     APP_Size_Max		        99                          // ����APP_size


/************************************************************************
* Prototype Declare Section     
************************************************************************/
        void        ZigBee_IRQHandler_Routine   (void);
        void        ZigBee_BufferPool_Init      (void);                     // ��ʼ�������
        void        ZigBee_Data_Display         (void);                     // ��ZigBee��������STM32��ʾ������
        void        ZigBee_BPWriteData          (unsigned char data);       // д������
        void        ZigBee_BPReadData           (unsigned char *data);      // ��������,datas�ǵ����߷�������飬�������ݽ����Ƶ�����
        void        ZigBee_SetFlowSize          (unsigned char val);        // ���������С
        char        ZigBee_BPCheckFlow          (void);                     // ����Ƿ��Ѿ�������������1��
static  char        ZigBee_BPCheckCancelCmd     (void);                     // ����Ƿ���ȡ����������������������1��
static  void        ZigBee_Data_Display_Init    (void);


	


#endif


