#ifndef __CARD_RECORDS_H_
#define __CARD_RECORDS_H_

#include "stm32f10x.h"
#include "rc522.h"
#include "flash.h"
#include "rtc.h"
#include "OLED.h"
#include "usart.h"

void Swiping_Card       (void);
void Add_Flash_ID       (void);     //一开始添加学生卡号
void Read_Flash_ID      (void);    //读取原来的存在Flash的ID
void Add_Student_ID     (void);   //增加一个学生卡号  重建版
void Del_card_ID        (void);      //把所有卡号清零
void Del_Card_Record    (void);  //清除刷卡记录//测试所用
void Read_Day_Stu_Id    (void);  //读取学生具体那一天的
void Write_Day_Stu_Id   (void);

#endif
