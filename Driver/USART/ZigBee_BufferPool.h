/*
 *		ZigBee缓冲池。自动管理的缓冲器
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
#define     ZigBee_Flow_Size_Max        32                          // 定义最大的溢出大小
#define     APP_Size_Max		        99                          // 定义APP_size


/************************************************************************
* Prototype Declare Section     
************************************************************************/
        void        ZigBee_IRQHandler_Routine   (void);
        void        ZigBee_BufferPool_Init      (void);                     // 初始化缓冲池
        void        ZigBee_Data_Display         (void);                     // 将ZigBee数据送入STM32显示缓存中
        void        ZigBee_BPWriteData          (unsigned char data);       // 写入数据
        void        ZigBee_BPReadData           (unsigned char *data);      // 读出数据,datas是调用者分配的数组，接收数据将复制到其中
        void        ZigBee_SetFlowSize          (unsigned char val);        // 设置溢出大小
        char        ZigBee_BPCheckFlow          (void);                     // 检查是否已经溢出（溢出返回1）
static  char        ZigBee_BPCheckCancelCmd     (void);                     // 检查是否是取消命令引起溢出（溢出返回1）
static  void        ZigBee_Data_Display_Init    (void);


	


#endif


