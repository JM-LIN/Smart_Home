#ifndef _OPP_H_
#define _OPP_H_

#include "stm32f10x.h"
#include <stdbool.h>

#define weekone 600000     //周期
#define weektwo 800000
#define weekthe 1000000
#define day(x)   (x-1)*200       //星期

void get(void);    //获取时间信息
void del(uint8_t ch3[4]);    //删除
void add(void);    //添加     
void BuildATOrder (int data);
char Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);    //字符串比较
void WriteBufferArea (void);
void text(void);
#endif
