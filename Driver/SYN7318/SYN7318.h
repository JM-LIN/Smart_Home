#ifndef _SYN7318_H_
#define _SYN7318_H_

#include "stm32f10x.h"
#include "Systick.h"
#include "Breathing_Light.h"
#include "sim7600.h"

#define     Stop_Play                       0x02//停止播放
#define     Pause_Play                      0x03//暂停播放
#define     Recover_Play                    0x04//恢复播放
#define     SYN7318_RST_HIGH_LEVEL()        GPIO_SetBits( GPIOF, GPIO_Pin_4 )
#define     SYN7318_RST_LOW_LEVEL()         GPIO_ResetBits( GPIOF, GPIO_Pin_4 )    // 复位

extern unsigned char Back[4];
extern unsigned char ASR[6];
extern unsigned char S[4];

extern char D_MP3_1[] ;
extern char D_MP3_2[] ;
extern char D_MP3_3[] ;
extern char TF_MP3_1[];
extern char Test[] ;
extern char Test1[] ;



void SYN7318_Reset_Init(void);
void Voice_Control(void);		
void SYN_TTS(char *Pst); //语音合成播放函数
void SYN_MP3(char *Pst); //播放MP3
void Start_ASR(char Dict); //开始识别函数
void Dict_TTS(char Dict); //词条索引函数
void Stop_ASR(void); //停止语音识别函数
void Start_WakeUp(char Wake_ID); //语音唤醒函数
void Three_One(char Dict, char Wake_ID, char Sound, char *Pst); //三合一函数
void Status_Query(void); //模块状态查询函数

void SYN_Buffer_Save(char *Pst, unsigned char Block);	//SYN语音合成缓存保存函数
void SYN_Buffer_TTS(unsigned char Times);	//SYN语音合成缓存播放函数
void SYN_Play_Control(unsigned char command);	//SYN播放控制命令函数
void SYN_VOL_Control(unsigned char Vol);	//SYN播放音量设置命令

void Stop_Three_One(void);	//停止三合一函数
void Stop_WakeUp(void);	//停止语言唤醒函数

#endif
