#include "SYN7318.h"
#include "usart.h"
#include "string.h"
#include "OLED.h"

unsigned char Back[4] = {0};   //��������ش�������
unsigned char ASR[6] = {0};    //����ʶ�����ش�������
unsigned char S[4] = {0};      //����ģ�鵱ǰ����״̬�ش�������

char D_MP3_1[] = {"D:\\Mp3\\Wifi_�״ο���.mp3"};
char D_MP3_2[] = {"D:\\Mp3\\Wifi_������.mp3"}; 
char D_MP3_3[] = {"D:\\Mp3\\Prompt_��.mp3"}; 
char D_MP3_4[] = {"D:\\Mp3\\Prompt_������.mp3"};
char TF_MP3_1[] = {"E:\\1-����\\һ�����㲻����.mp3"};  

char Test[] = {"�׺�"};

char *Dict0[] = {"������","������","�������","��С����","����ʾ��","�ر���ʾ��","�뿪��","��ص�",
                "����һ��","����һ��","��С����","�������"};           //00�ʵ�

char ** DICT[10] = {Dict0}; //������������ʵ�


void Voice_Control(void)
{
    const char num[]=   "15768088230";   
    const char mynum[]= "13232963434";
    
    static uint16_t fan_speed = 0;
    
    
    USART3_Config();	//ȥ�����ֲ�����
    
    // 0x00 �Ʊ�
    // 0x1F ���ڸ���
    //������,���ѳɹ��󲥷š������ء�,֮�����ʶ��0x00�ʵ��еĴ���
    Three_One(0x03, 0x1F, 0x02, D_MP3_4);   
    switch (ASR[3])
    {
        case 0x06:  // �뿪��
            TIM_Cmd(TIM1, ENABLE);              // �򿪶�ʱ��1
            Pwm_led_status = OPEN;              // ����
            SYN_TTS("Ϊ�㿪��");
            delay_ms(2000);
            break;
       
        case 0x07:  // ��ص�
            TIM_Cmd(TIM1, ENABLE);
            Pwm_led_status = CLOSE;              // �ص�
            SYN_TTS("Ϊ��ص�");
            delay_ms(2000);
            break;
       
        case 0x11:  // �򿪷���
            TIM_Cmd(TIM4, ENABLE);
            SYN_TTS("�򿪻���");
            TIM_SetCompare2(TIM4, fan_speed);
            delay_ms(2000);
            break;
        case 0x13:  // �رշ���
            TIM_Cmd(TIM4, ENABLE);
            SYN_TTS("�رջ���");
            TIM_SetCompare2(TIM4, 0);
            delay_ms(2000);
            break;
        case 0x16:  // ����ת��
            SYN_TTS("�����Ӵ�");
            TIM_SetCompare2(TIM4, fan_speed+500);
            delay_ms(2000);
            break;

        case 0x1D:  // ����
            TIM_SetCompare3(TIM4, 50);		// �������ת��
            SYN_TTS("�������ڴ�");		// �������� �������ڴ�
            delay_ms(2000); 
            TIM_SetCompare3(TIM4, 53);		// �������ֹͣ
            break;
        case 0x1E:  // �ش�
            TIM_SetCompare3(TIM4, 56); 		// �������ת��
            SYN_TTS("�������ڹر�");		// �������� �������ڹر�
            delay_ms(2000);
            TIM_SetCompare3(TIM4, 53);		// �������ֹͣ
            break;

        case 0x1F:  // ����
            TIM_SetCompare4(TIM4, 15);
            SYN_TTS("���ڿ���");
            delay_ms(1500);	break;         
        case 0x20:  // ����
            TIM_SetCompare4(TIM4, 7);
            SYN_TTS("���ڹ���");
            delay_ms(1500);	break;
            break;
        case 0x21:  // ̫������
            SYN_TTS("�򿪼�ʪ��");
            delay_ms(2000);
            break;
        case 0x22:  // �رռ�ʪ��
            SYN_TTS("�رռ�ʪ��");
            delay_ms(2000);
            break;
        
        case 0x23:  // �һ�����
            SYN_TTS("��ӭ�ؼ�");
            delay_ms(2000);
            break;
        case 0x24:  // �ҳ�����
            SYN_TTS("�õ�����");
         	
            delay_ms(2000);
            break;
        case 0x25:  // û����
            SYN_TTS("�õ�");
            delay_ms(1000);
            break;
        case 0x26:  // ģ������
            SYN_TTS("����Ϊ������");
            delay_ms(2500);
            break;
        case 0x27:  // ���ָ��
            SYN_TTS("�����ָ����");
            delay_ms(2000);
            break;
        case 0x28:  // ����绰
            sim900a_call((char *)mynum);
            SYN_TTS("���ڲ���");
            delay_ms(2000);
            break;
        case 0x29:  // Ԥ��
            SYN_TTS("Ԥ��ģʽ");
            delay_ms(2000);
            break;
        case 0x2A:  // ����
            SYN_TTS("����ģʽ");
            delay_ms(2000);
            break;
        case 0x2B:  // ������Ϣ
            SYN_TTS("��Ϣ������");
            sim900a_sms((char *)mynum,"This is a test");
            delay_ms(2000);
            break;
        case 0x2C:  // ������������
            SYN_TTS("GPRS���ڴ�");
            sim900a_gprs_send("TMwhat_HMthe_SMSfuck_LSare_WEyou_SSOtalking_SSTabout_HT?_ARlll_PMccc");
            delay_ms(2000);
            break;
        default:
            SYN_TTS("�����ͨ����ͦ��ͨ��");
            delay_ms(2500);
            break;
    }
}

void SYN7318_Reset_Init(void)											// ������
{
	GPIO_InitTypeDef GPIO_Initstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_Initstructure);
	
    SYN7318_RST_LOW_LEVEL();
    delay_ms(50);
    SYN7318_RST_HIGH_LEVEL();
}

/****************************************************************************************
���� �� ������SYN_TTS----�����ϳɲ��ź���
������˵������Pst�����Ҫ�ϳɲ��ŵ��ı���������
����    ������SYN_TTS("��������"); //�ϳɲ��ű�������              
****************************************************************************************/
void SYN_TTS(char *Pst)
{
	uint16_t Length;
	char Frame[5];   //���淢�����������
	
	Length = strlen(Pst);
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x01;      //�����ϳɲ�������
	Frame[4] = 0x00;      //���ű����ʽΪ��GB2312��
	
    UART_Put_String(Frame,5);	
	UART_Put_String(Pst, Length);	
}


/*******************************************************************
���� �� ������SYN_MP3----MP3���ź���
������˵������Pst�����Ҫ���ŵ�MP3��������
����    ������char D_MP3_2[] = {"D:\\Mp3\\Wifi_������.mp3"}; 
              SYN_MP3(D_MP3_2); //����MP3
*******************************************************************/
void SYN_MP3(char *Pst)
{
	uint16_t Length;
	char Frame[5];   //���淢�����������
	
	Length = strlen(Pst);
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x61;      //MP3��������
	Frame[4] = 0x00;      //���ű����ʽΪ��GB2312��
	
    UART_Put_String(Frame, 5);	
	UART_Put_String(Pst, Length);	
}


/*********************************************************************
���� �� ������Start_ASR----��ʼ����ʶ����
������˵������Dict���ʵ���
����    ������Start_ASR(0x00); //ʶ��0x00�ʵ��еĴ���
*********************************************************************/
void Start_ASR(char Dict)
{
	char Frame[5];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x10;      //��ʼ����ʶ������
	Frame[4] = Dict;      //�ʵ���  �������޸���Ҫʶ��Ĵʵ���
	
	ASR[3] = 0;
	
  UART_Put_String(Frame, 5);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		 UART_Get_String(Back,3);  //����ʶ������ش����
		if(Back[0] == 0xfc) 
		{
			 UART_Get_String(ASR,Back[2]);
		}
	}
}

/***************************************************************************
���� �� ������Dict_TTS----ʶ��ʵ�����������
������˵������Dict���ʵ���
����    ������Start_ASR(0x00); //ʶ��0x00�ʵ��еĴ���
              Dict_TTS(0x00);  //���ʶ��ɹ�,��ʶ�𵽵Ĵ����������� 
***************************************************************************/
void Dict_TTS(char Dict)
{
	switch(ASR[0])
	{
		case 0x01:
		case 0x02:
		{
			SYN_TTS(DICT[Dict][ASR[3]]);  //ASR[3]�д�Ŵ���ID
		}break;
		case 0x03:
		{
			SYN_TTS("�û�������ʱ"); 
		}break;
		case 0x04:
		{
			SYN_TTS("�û�������ʱ");
		}break;
		case 0x05:
		case 0x07:
		{
			SYN_TTS("ʶ���ʶ");
		}break;
		case 0x06:
		{
			SYN_TTS("ʶ���ڲ�����");
		}break;
	}
}

/*********************************************************************
���� �� ������Stop_ASR----ֹͣ����ʶ����
������˵�������޲�
*********************************************************************/
void Stop_ASR(void)
{
	char Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x11;      //ֹͣ����ʶ������
	
  UART_Put_String(Frame, 4);
}



/*********************************************************************************************************************************************************
���� �� ������Start_WakeUp----��ʼ�������Ѻ���
������˵������Wake_ID��������ID��----0x00���Ʊ�  0x02��С��   0x04������  0x06�������ܼ�  0x07������ܼ�  0x08��С���ܼ�  0x09����ܼ�  0x1F���Զ���
����    ������Start_WakeUp(0x09);//������Ϊ��ܼ�,���ѳɹ�֮�󲥱��������⡱
**********************************************************************************************************************************************************/
void Start_WakeUp(char Wake_ID)
{
	char Frame[5];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x51;      //��ʼ������������
	Frame[4] = Wake_ID;   
	
  UART_Put_String(Frame, 5);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 UART_Get_String(Back,4);  //����ʶ������ش����
		if(Back[3] == 0x21)
		{
			 SYN_MP3("D:\\Mp3\\Wifi_������.mp3");
		}
	}
}

/*********************************************************************************************************************************************************
���� �� ������Three_One----����һ�����������������ѣ����Ѻ󲥷���ʾ���������������ʶ��
������˵������Dict���ʵ���
              Wake_ID��������ID��----0x00���Ʊ�  0x02��С��   0x04������  0x06�������ܼ�  0x07������ܼ�  0x08��С���ܼ�  0x09����ܼ�  0x1F���Զ���
              Sound����ʾ������  0x00������ʾ��  0x01���ı�����  0x02������MP3����
              Pst����ʾ��������
����    ������Three_One(0x00, 0x00, 0x02, D_MP3_2);//������Ϊ�Ʊ�,���ѳɹ��󲥷š������⡱,֮�����ʶ��0x00�ʵ��еĴ��� 
              Dict_TTS(0x00); //���Dict_TTS������ʶ������������
**********************************************************************************************************************************************************/
void Three_One(char Dict, char Wake_ID, char Sound, char *Pst)
{
	char Frame[100] = {0};   //���淢�����������
	uint16_t Length,i;
	
	Frame[0] = 0xFD;      //֡ͷ
//	Frame[1] = 0x00;
//	Frame[2] = 0x07;    //���ڲ�ȷ���������ĳ��ȣ�����ѡ����ʱ����
	Frame[3] = 0x15;      //����һʶ������
	Frame[4] = Dict;      //�ʵ���
	Frame[5] = Wake_ID;   //������ID��
	Frame[6] = Sound;     //��ʾ������
	if(Sound == 0x00)     //����ʾ��
	{
		Frame[1] = 0x00;
		Frame[2] = 0x04;
	  UART_Put_String(Frame, 7);	
	}
	else                 //����ʾ��
	{
		Frame[7] = 0x01;      //�ı�������MP3��ʾ�������ʽ��0x00��GB2312����  0x01��GBK����  0x02��BIG5����  0x03��UnicodeСͷ  0x04�Unicode��ͷ
		Length = strlen(Pst); //�ı�������MP3��ʾ���ĳ���
		Frame[1] = 0x00;
		Frame[2] = Length + 5;
		for(i = 0; i < Length; i++)
		{
			Frame[8 + i] = Pst[i];
		}
	  UART_Put_String(Frame, Length + 8);	
	}
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		UART_Get_String(Back,3);  //����ʶ������ش����
		if(Back[0] == 0xfc)
		{
			 UART_Get_String(ASR,Back[2]);
		}
	}
}

/***************************************************************************
���� �� ������Status_Query----ģ��״̬��ѯ����
������˵�������޲�
����    ������Status_Query(); 
***************************************************************************/
void Status_Query(void)
{
	char Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;    //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x21;   //״̬��ѯ����
	
  UART_Put_String(Frame, 4);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 UART_Get_String(S,4);  //ģ�鵱ǰ����״̬�Ļش����
	}
}


/*****************************************����Ϊ�����Ĺ��ܣ������ԣ�*************************************************/

/************************SYN�����ϳɻ��汣��************************/
/*���ܣ����һЩ�Ƚϳ������ݿ����ȴ洢���������棬�ٽ��л��沥��      */
/*ע�⣺����洢�ͻ��沥��֮�䲻����������ָ�                     */
/*����ֵ��Pst�����Ҫ�ϳɲ��ŵ��ı���������                         */
/*       Block�������ϳɻ���洢��ѡȡ��ȡֵ0--15                  */
/*****************************************************************/
void SYN_Buffer_Save(char *Pst, unsigned char Block)
{
	uint16_t Length;
	char Frame[5];   //���淢�����������
	
	Length = strlen(Pst);
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x31;      //�����ϳɻ���洢ָ��
	Frame[4] = Block;      //�����ϳɻ���洢��ѡȡ��ȡֵ0--15
	
  UART_Put_String(Frame,5);			//����
	UART_Put_String(Pst, Length);	
}

/************************SYN�����ϳɻ��沥��************************/
/*���ܣ����һЩ�Ƚϳ������ݿ����ȴ洢���������棬�ٽ��л��沥��      */
/*ע�⣺����洢�ͻ��沥��֮�䲻����������ָ�                     */
/*����ֵ��Times�����Ŵ���                                         */
/*****************************************************************/
void SYN_Buffer_TTS(unsigned char Times)
{
	char Frame[5];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x02;
	Frame[3] = 0x32;      //�����ϳɻ���洢����ָ��
	Frame[4] = ((Times<<4)|0x00);      //��4λ�ǲ��Ŵ��� ����λ�Ǳ����ʽ
	
  UART_Put_String(Frame,5);			//����
}

/************************SYN���ſ�������***************************/
/*���ܣ����Զ����������������ֹͣ����ͣ�ͻָ�����                  */
/*      1�������ϳɲ�������                                       */
/*      2���ı����沥������                                       */
/*      3��MP3��������                                           */
/*����ֵ��command��                                              */
/*  			1��Stop_Play  ֹͣ����                                 */
/*        2��Pause_Play  ��ͣ����                                */
/*				3��Recover_Play  �ָ�����                              */
/****************************************************************/
void SYN_Play_Control(unsigned char command)
{
	char Frame[4];		//���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;
	Frame[3] = command;      //��������
	
	UART_Put_String(Frame,4);			//����
}

/**********************SYN����������������*************************/
/*���ܣ����Զ�������������Ĳ����������и���                       */
/*      1�������ϳɲ�������                                       */
/*      2���ı����沥������                                       */
/*      3��MP3��������                                           */
/*����ֵ��Vol��������С����ȡֵ0x00-0x09	                       */
/*****************************************************************/
void SYN_VOL_Control(unsigned char Vol)
{
	char Frame[5];		//���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x02;
	Frame[3] = 0x05;      //������������
	Frame[4] = Vol;				//��������0x00--0x09
	
	UART_Put_String(Frame,5);			//����
}

/*************************************����ֹͣ�Ĺ��ܣ������ԣ�******************************************/

/**************************ֹͣ���Ի���****************************/
/*���ܣ�ֹͣ��ǰ�����Ի���                                         */
/*����ֵ����                                                      */
/*****************************************************************/
void Stop_WakeUp(void)
{
	char Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;
	Frame[3] = 0x52;      //ֹͣ���Ի�������
	
  UART_Put_String(Frame,4);			//����
}

/**************************ֹͣ����һʶ��***************************/
/*���ܣ�ֹͣ��ǰ������һʶ��                                       */
/*����ֵ����                                                      */
/*****************************************************************/
void Stop_Three_One(void)
{
	char Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;
	Frame[3] = 0x16;      //ֹͣ����һ����
	
  UART_Put_String(Frame,4);			//����
}

