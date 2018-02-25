/*
 *		����ء��Զ�����Ļ�����
*/

#include "WIFI_BufferPool.h"


/************************************************************************
* Global Variable Declare Section   
************************************************************************/
		 unsigned char			*Server_ID 				= NULL;
volatile unsigned char			_buffer[Flow_Size_Max] 	= {0,0};	// ����صĻ�����
volatile unsigned char			flowsize 				= 0;		// �����С							
volatile unsigned char			flowflag 				= 0;		// ������									
volatile unsigned char			cancelflag 				= 0;		// ȡ�����									
volatile unsigned char			wLoc 					= 0;		// ��ǰд��λ��
	
/*************WIFI_Data_Flag �˻�������־**************/
volatile unsigned char Restart_flag 		= 0;
volatile unsigned char Aeration_flag 		= 0;
volatile unsigned char Light_ON_flag 		= 0;
volatile unsigned char Light_OFF_flag 		= 0;
volatile unsigned char Socket1_flag 		= 0;
volatile unsigned char Door_flag 			= 0;
volatile unsigned char Humidification_flag 	= 0;
volatile unsigned char Curtain_flag 		= 2;
volatile unsigned char Socket2_flag 		= 0;
volatile unsigned char Pattern_flag 		= 0;
/***************WIFI_Data_Flag �˻�������־***************/	

volatile u32 Send_Data_time = 0;
volatile u32 Send_timeout = 0;
volatile uint8_t ucTcpClosedFlag = 0;						// ESP8266��Tcp�رձ�־

					
/************************************************************************
* Function Define Section               
************************************************************************/
void WIFI_BufferPool_Init(void)															// ��ʼ�������
{					
	char i;					
	for(i=0; i<Flow_Size_Max; i++)					
		_buffer[i] = 0;					
						
	flowsize = Flow_Size_Max;														// ��ʼ�������С
														
	flowflag = 0;																	// ��ʼ��������
													
	cancelflag = 0;																	// ��ʼ��ȡ�����
													
	wLoc = 0;																		// ��ʼ����ǰд��λ��

    WIFI_SetFlowSize(10);                                                           // д���ֵ
}					
					
					
// д������					
void WIFI_BPWriteData(unsigned char data)					
{							
	static int i=0;
	_buffer[wLoc++] = data;															// д�뻺����
										
	if(wLoc == flowsize)flowflag = 1;												// ����Ƿ����
										
	if(wLoc == 3)																	// ���ȡ�����
	{
		if(_buffer[0] == 'c' && _buffer[1] == 'a' && _buffer[2] == 'n' )			// ȡ������
		{
			cancelflag = 1;
			flowflag = 1;		
			wLoc = 0;																// ��λд��λ��
		}
	}
	
//	if(wLoc == 6)																	// ���ȡ�����
//	{
//		if(_buffer[0] == 'i' && _buffer[1] == 'd' && _buffer[2] == '=' )			// ȡ������
//		{			
//			Server_ID[0] = _buffer[0];
//			Server_ID[1] = _buffer[1];
//			Server_ID[2] = _buffer[2];
//			Server_ID[3] = _buffer[3];
//			Server_ID[4] = _buffer[4];
//			Server_ID[5] = _buffer[5];		
//			LCD_P6x8Str(31,3,(unsigned char*)"Connected !");	
//			LCD_P6x8Str(48,4,(unsigned char*)Server_ID);
//			flowflag = 1;		
//			wLoc = 0;
//		}
//	}
		
	if(wLoc == 4)									
	{
		if(_buffer[0] == 'S' && _buffer[1] == 'w' && _buffer[3] == 't' )			// �л�ģʽ
		{
			Pattern_flag = !Pattern_flag;		
		}
		
		if(_buffer[0] == 'A' && _buffer[1] == 'e' && _buffer[3] == 'a' )			// ��������
		{
			Aeration_flag = !Aeration_flag;		
		}
		
		if(_buffer[0] == 'L' && _buffer[1] == 'i' && _buffer[3] == 'h' )			// ��������
		{
			if(i == 0)
				Light_ON_flag = !Light_ON_flag;	
			if(i == 1)
				Light_OFF_flag = !Light_OFF_flag;
			i++;
			if(i>=2)
				i = 0;		
		}
		
		if(_buffer[0] == 'R' && _buffer[1] == 'e' && _buffer[3] == 't' )			// ��������
		{
			Restart_flag = !Restart_flag;	
		}
		
		if(_buffer[0] == 'C' && _buffer[1] == 'u' && _buffer[3] == 't' )			// ��������
		{
			Curtain_flag++;		
			if(Curtain_flag >2)
				Curtain_flag = 0;
		}
		
		if(_buffer[0] == 'D' && _buffer[1] == 'o' && _buffer[3] == 'r' )			// �Ž�����
		{
			Door_flag = !Door_flag;			
		}
		
		if(_buffer[0] == 'H' && _buffer[1] == 'u' && _buffer[3] == 'i' )			// ��ʪ����
		{
			Humidification_flag = !Humidification_flag;		
		}
		
		if(_buffer[0] == 'S' && _buffer[1] == 'o' && _buffer[3] == '1' )			// �Ų�1����
		{
			Socket1_flag = !Socket1_flag;	
			ZigBee_Usart((unsigned char*)"So1");
		}
		
		if(_buffer[0] == 'S' && _buffer[1] == 'o' && _buffer[3] == '2' )			// �Ų�2����
		{
			Socket2_flag = !Socket2_flag;	
			ZigBee_Usart((unsigned char*)"So2");
		}
	}
}


void WIFI_BPReadData(unsigned char* data)							// ��������
{
	char i;
	wLoc = 0;													// ����д��λ��

	flowflag = 0;												// ����������

	for(i=0; i<flowsize; i++)									// ��������
		data[i] = _buffer[i];
}


char WIFI_BPCheckFlow(void)											// ����Ƿ��Ѿ����
{										
	return flowflag;										
}										
										
																// ����Ƿ���ȡ�������������
char WIFI_BPCheckCancelCmd(void)
{
	if(cancelflag)
	{
		cancelflag = 0; 										// ����ȡ�����
		flowflag   = 0; 										// ����������
		return 1;
	}
	else return 0;
}


void WIFI_SetFlowSize(unsigned char val)								// ���������С
{
	flowsize = val;
}
