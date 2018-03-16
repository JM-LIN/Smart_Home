#ifndef _SYN7318_H_
#define _SYN7318_H_

#include "stm32f10x.h"
#include "Systick.h"
#include "Breathing_Light.h"
#include "sim7600.h"

#define     Stop_Play                       0x02//ֹͣ����
#define     Pause_Play                      0x03//��ͣ����
#define     Recover_Play                    0x04//�ָ�����
#define     SYN7318_RST_HIGH_LEVEL()        GPIO_SetBits( GPIOF, GPIO_Pin_4 )
#define     SYN7318_RST_LOW_LEVEL()         GPIO_ResetBits( GPIOF, GPIO_Pin_4 )    // ��λ

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
void SYN_TTS(char *Pst); //�����ϳɲ��ź���
void SYN_MP3(char *Pst); //����MP3
void Start_ASR(char Dict); //��ʼʶ����
void Dict_TTS(char Dict); //������������
void Stop_ASR(void); //ֹͣ����ʶ����
void Start_WakeUp(char Wake_ID); //�������Ѻ���
void Three_One(char Dict, char Wake_ID, char Sound, char *Pst); //����һ����
void Status_Query(void); //ģ��״̬��ѯ����

void SYN_Buffer_Save(char *Pst, unsigned char Block);	//SYN�����ϳɻ��汣�溯��
void SYN_Buffer_TTS(unsigned char Times);	//SYN�����ϳɻ��沥�ź���
void SYN_Play_Control(unsigned char command);	//SYN���ſ��������
void SYN_VOL_Control(unsigned char Vol);	//SYN����������������

void Stop_Three_One(void);	//ֹͣ����һ����
void Stop_WakeUp(void);	//ֹͣ���Ի��Ѻ���

#endif
