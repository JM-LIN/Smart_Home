#ifndef _OPP_H_
#define _OPP_H_

#include "stm32f10x.h"
#include <stdbool.h>

#define weekone 600000     //����
#define weektwo 800000
#define weekthe 1000000
#define day(x)   (x-1)*200       //����

void get(void);    //��ȡʱ����Ϣ
void del(uint8_t ch3[4]);    //ɾ��
void add(void);    //���     
void BuildATOrder (int data);
char Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);    //�ַ����Ƚ�
void WriteBufferArea (void);
void text(void);
#endif
