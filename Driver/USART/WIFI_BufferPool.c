/*
 *		缓冲池。自动管理的缓冲器
*/

#include "WIFI_BufferPool.h"


/************************************************************************
* Global Variable Declare Section   
************************************************************************/
		 unsigned char			*Server_ID 				= NULL;
volatile unsigned char			_buffer[Flow_Size_Max] 	= {0,0};	// 缓冲池的缓冲区
volatile unsigned char			flowsize 				= 0;		// 溢出大小							
volatile unsigned char			flowflag 				= 0;		// 溢出标记									
volatile unsigned char			cancelflag 				= 0;		// 取消标记									
volatile unsigned char			wLoc 					= 0;		// 当前写入位置
	
/*************WIFI_Data_Flag 人机交互标志**************/
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
/***************WIFI_Data_Flag 人机交互标志***************/	

volatile u32 Send_Data_time = 0;
volatile u32 Send_timeout = 0;
volatile uint8_t ucTcpClosedFlag = 0;						// ESP8266中Tcp关闭标志

					
/************************************************************************
* Function Define Section               
************************************************************************/
void WIFI_BufferPool_Init(void)															// 初始化缓冲池
{					
	char i;					
	for(i=0; i<Flow_Size_Max; i++)					
		_buffer[i] = 0;					
						
	flowsize = Flow_Size_Max;														// 初始化溢出大小
														
	flowflag = 0;																	// 初始化溢出标记
													
	cancelflag = 0;																	// 初始化取消标记
													
	wLoc = 0;																		// 初始化当前写入位置

    WIFI_SetFlowSize(10);                                                           // 写溢出值
}					
					
					
// 写入数据					
void WIFI_BPWriteData(unsigned char data)					
{							
	static int i=0;
	_buffer[wLoc++] = data;															// 写入缓冲区
										
	if(wLoc == flowsize)flowflag = 1;												// 检查是否溢出
										
	if(wLoc == 3)																	// 检查取消标记
	{
		if(_buffer[0] == 'c' && _buffer[1] == 'a' && _buffer[2] == 'n' )			// 取消命令
		{
			cancelflag = 1;
			flowflag = 1;		
			wLoc = 0;																// 复位写入位置
		}
	}
	
//	if(wLoc == 6)																	// 检查取消标记
//	{
//		if(_buffer[0] == 'i' && _buffer[1] == 'd' && _buffer[2] == '=' )			// 取消命令
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
		if(_buffer[0] == 'S' && _buffer[1] == 'w' && _buffer[3] == 't' )			// 切换模式
		{
			Pattern_flag = !Pattern_flag;		
		}
		
		if(_buffer[0] == 'A' && _buffer[1] == 'e' && _buffer[3] == 'a' )			// 换气命令
		{
			Aeration_flag = !Aeration_flag;		
		}
		
		if(_buffer[0] == 'L' && _buffer[1] == 'i' && _buffer[3] == 'h' )			// 照明命令
		{
			if(i == 0)
				Light_ON_flag = !Light_ON_flag;	
			if(i == 1)
				Light_OFF_flag = !Light_OFF_flag;
			i++;
			if(i>=2)
				i = 0;		
		}
		
		if(_buffer[0] == 'R' && _buffer[1] == 'e' && _buffer[3] == 't' )			// 重启命令
		{
			Restart_flag = !Restart_flag;	
		}
		
		if(_buffer[0] == 'C' && _buffer[1] == 'u' && _buffer[3] == 't' )			// 窗帘命令
		{
			Curtain_flag++;		
			if(Curtain_flag >2)
				Curtain_flag = 0;
		}
		
		if(_buffer[0] == 'D' && _buffer[1] == 'o' && _buffer[3] == 'r' )			// 门禁命令
		{
			Door_flag = !Door_flag;			
		}
		
		if(_buffer[0] == 'H' && _buffer[1] == 'u' && _buffer[3] == 'i' )			// 加湿命令
		{
			Humidification_flag = !Humidification_flag;		
		}
		
		if(_buffer[0] == 'S' && _buffer[1] == 'o' && _buffer[3] == '1' )			// 排插1命令
		{
			Socket1_flag = !Socket1_flag;	
			ZigBee_Usart((unsigned char*)"So1");
		}
		
		if(_buffer[0] == 'S' && _buffer[1] == 'o' && _buffer[3] == '2' )			// 排插2命令
		{
			Socket2_flag = !Socket2_flag;	
			ZigBee_Usart((unsigned char*)"So2");
		}
	}
}


void WIFI_BPReadData(unsigned char* data)							// 读出数据
{
	char i;
	wLoc = 0;													// 重置写入位置

	flowflag = 0;												// 重置溢出标记

	for(i=0; i<flowsize; i++)									// 复制数据
		data[i] = _buffer[i];
}


char WIFI_BPCheckFlow(void)											// 检查是否已经溢出
{										
	return flowflag;										
}										
										
																// 检查是否是取消命令引起溢出
char WIFI_BPCheckCancelCmd(void)
{
	if(cancelflag)
	{
		cancelflag = 0; 										// 重置取消标记
		flowflag   = 0; 										// 重置溢出标记
		return 1;
	}
	else return 0;
}


void WIFI_SetFlowSize(unsigned char val)								// 设置溢出大小
{
	flowsize = val;
}
