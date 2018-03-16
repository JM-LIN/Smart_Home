#ifndef __CARD_RECORDS_H_
#define __CARD_RECORDS_H_

#include "stm32f10x.h"
#include "rc522.h"
#include "flash.h"
#include "rtc.h"
#include "OLED.h"
#include "usart.h"

void Swiping_Card       (void);
void Add_Flash_ID       (void);     //һ��ʼ���ѧ������
void Read_Flash_ID      (void);    //��ȡԭ���Ĵ���Flash��ID
void Add_Student_ID     (void);   //����һ��ѧ������  �ؽ���
void Del_card_ID        (void);      //�����п�������
void Del_Card_Record    (void);  //���ˢ����¼//��������
void Read_Day_Stu_Id    (void);  //��ȡѧ��������һ���
void Write_Day_Stu_Id   (void);

#endif
