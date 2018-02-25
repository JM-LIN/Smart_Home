/*
		ZigBee缓冲池 自动管理的缓冲器
*/
/************************************************************************
* Include File Section         
************************************************************************/
#include "ZigBee_BufferPool.h"


/************************************************************************
* Global Variable Declare Section   
************************************************************************/
volatile unsigned char APP_buffer[APP_Size_Max]                 = {0,0};		// 用于存储APP显示信息
volatile unsigned char ZigBee_buffer[ZigBee_Flow_Size_Max]      = {0,0};        // 缓冲池的缓冲区
volatile unsigned char ZigBee_flowsize                          = 0;            // 溢出大小
volatile unsigned char ZigBee_flowflag                          = 0;            // 溢出标记
volatile unsigned char ZigBee_cancelflag                        = 0;            // 取消标记
volatile unsigned char ZigBee_wLoc                              = 0;		    // 当前写入位置
volatile unsigned char Socket_Status;   


/************************************************************************
* Function Define Section               
************************************************************************/
// 写入数据
void ZigBee_BPWriteData(unsigned char data)
{
	ZigBee_buffer[ZigBee_wLoc++] = data;        // 写入缓冲区
	if(ZigBee_wLoc == ZigBee_flowsize)	        // 检查是否溢出
        ZigBee_flowflag = 1;    
	ZigBee_Data_Display();	
}

// 读出数据
void ZigBee_BPReadData(unsigned char *data)
{
	char i;
	
	ZigBee_wLoc = 0;                            // 重置写入位置
	ZigBee_flowflag = 0;                        // 重置溢出标记
    
	for(i=0; i<ZigBee_flowsize; i++)            // 复制数据
		data[i] = ZigBee_buffer[i];
}

// 检查是否已经溢出
char ZigBee_BPCheckFlow(void)
{
	return ZigBee_flowflag;
}

// 检查是否是取消命令引起溢出
static char ZigBee_BPCheckCancelCmd(void)
{
	if(ZigBee_cancelflag)
	{
		ZigBee_cancelflag = 0;                   // 重置取消标记
		ZigBee_flowflag   = 0;                   // 重置溢出标记
		return 1;
	}
	else return 0;
}

// 设置溢出大小
void ZigBee_SetFlowSize(unsigned char val)
{
	ZigBee_flowsize = val;
}

// 初始化缓冲池
void ZigBee_BufferPool_Init(void)
{
	char i;
	for(i=0; i<ZigBee_Flow_Size_Max; i++)
		ZigBee_buffer[i] = '0';
		
	ZigBee_flowsize = ZigBee_Flow_Size_Max;         // 初始化溢出大小	
	ZigBee_flowflag = 0;                            // 初始化溢出标记	
	ZigBee_cancelflag = 0;                          // 初始化取消标记
	ZigBee_wLoc = 0;                                // 初始化当前写入位置
    
    ZigBee_SetFlowSize(7);                          // 写溢出值
    ZigBee_Data_Display_Init();
}

static void ZigBee_Data_Display_Init(void)
{
	memset(APP_buffer,'0',sizeof(APP_buffer));
	
	// 温湿度
	APP_buffer[26] = 'T';
	APP_buffer[27] = 'M';
	APP_buffer[30] = 'C';
	APP_buffer[31] = '_';
	APP_buffer[32] = 'H';
	APP_buffer[33] = 'M';
	APP_buffer[36] = 'P';
	APP_buffer[37] = '_';
	
	// 气体浓度
	APP_buffer[38] = 'S';
	APP_buffer[39] = 'M';
	APP_buffer[40] = 'S';
	APP_buffer[44] = 'p';
	APP_buffer[45] = 'p';
	APP_buffer[46] = 'm';
	APP_buffer[47] = '_';
	
	// 光强
	APP_buffer[48] = 'L';
	APP_buffer[49] = 'S';
	APP_buffer[55] = 'l';
	APP_buffer[56] = 'x';
	APP_buffer[57] = '_';
	
	// 天气
	APP_buffer[58] = 'W';
	APP_buffer[59] = 'E';
	APP_buffer[65] = '_';
	
	// 插座1
	APP_buffer[66] = 'S';
	APP_buffer[67] = 'S';
	APP_buffer[68] = 'O';
	APP_buffer[72] = '_';
	
	// 插座2
	APP_buffer[73] = 'S';
	APP_buffer[74] = 'S';
	APP_buffer[75] = 'T';
	APP_buffer[79] = '_';
	
	// 加湿
	APP_buffer[80] = 'H';
	APP_buffer[81] = 'T';
	APP_buffer[85] = '_';
	
	// 换气
	APP_buffer[86] = 'A';
	APP_buffer[87] = 'R';
	APP_buffer[91] = '_';
	
	// 模式
	APP_buffer[92] = 'P';
	APP_buffer[93] = 'M';
	APP_buffer[98] = '\0';
}

// 送入显示缓存中
void ZigBee_Data_Display(void)
{
	if(ZigBee_wLoc == 7)			// 温湿度
	{
		if( ZigBee_buffer[0] == 'T' )
		{			
			LCD_P6x8Str(0,2,(unsigned char*)ZigBee_buffer);						
			APP_buffer[28] = ZigBee_buffer[1];
			APP_buffer[29] = ZigBee_buffer[2];
			
			APP_buffer[34] = ZigBee_buffer[5];
			APP_buffer[35] = ZigBee_buffer[6];			
		}   
	}
	if(ZigBee_wLoc == 4)			// 气体浓度
	{
		if( ZigBee_buffer[0] == 'M' )
		{	
			LCD_P6x8Str(0,3,(unsigned char*)ZigBee_buffer);			
			APP_buffer[41] = ZigBee_buffer[1];
			APP_buffer[42] = ZigBee_buffer[2];
			APP_buffer[43] = ZigBee_buffer[3];			
		}
	}
	if(ZigBee_wLoc == 7)			// 光照强度
	{
		if( ZigBee_buffer[0] == 'L' )
		{					
			LCD_P6x8Str(0,4,(unsigned char*)ZigBee_buffer); 
			APP_buffer[50] = ZigBee_buffer[2];
			APP_buffer[51] = ZigBee_buffer[3]; 
			APP_buffer[52] = ZigBee_buffer[4];
			APP_buffer[53] = ZigBee_buffer[5];
			APP_buffer[54] = ZigBee_buffer[6];			
		}
	}
	if(ZigBee_wLoc == 2)
	{
		if( ZigBee_buffer[0] == 'Q' )										// MQ135
		{		
            LCD_P6x8Str(0,5,(unsigned char*)ZigBee_buffer);			
			if( ZigBee_buffer[1] == 'D' )						
				Socket_Status = 'D';
			if( ZigBee_buffer[1] == 'S' )
				Socket_Status = 'S';
		}

		if( ZigBee_buffer[0] == 'S' && ZigBee_buffer[1] == 'U')			    // 雨滴模块
		{		
            LCD_P6x8Str(18,5,(unsigned char*)ZigBee_buffer);
			APP_buffer[60] = 'S';
			APP_buffer[61] = 'u';
			APP_buffer[62] = 'n';
			APP_buffer[63] = 'n';
			APP_buffer[64] = 'y';
		}
		if( ZigBee_buffer[0] == 'R' && ZigBee_buffer[1] == 'A')
		{	
            LCD_P6x8Str(18,5,(unsigned char*)ZigBee_buffer);			
			APP_buffer[60] = 'R';
			APP_buffer[61] = 'a';
			APP_buffer[62] = 'i';
			APP_buffer[63] = 'n';
			APP_buffer[64] = 'y';
		}
		
		if( ZigBee_buffer[0] == 'S')			                            // 人体感应
		{	
            if( ZigBee_buffer[1] == '1')
            {
                LCD_P6x8Str(66,6,(unsigned char*)ZigBee_buffer);
            }
            if( ZigBee_buffer[1] == '2')
            {
                LCD_P6x8Str(66,7,(unsigned char*)ZigBee_buffer);
            }
		}
		if( ZigBee_buffer[0] == 'N')
		{				
            
            if( ZigBee_buffer[1] == '1')
            {
                LCD_P6x8Str(66,6,(unsigned char*)ZigBee_buffer);
            }
            if( ZigBee_buffer[1] == '2')
            {
                LCD_P6x8Str(66,7,(unsigned char*)ZigBee_buffer);
            }
		}
	}
	if(ZigBee_wLoc == 3)								                    // 继电器
	{
		if( ZigBee_buffer[0] == 'R' )
		{				
			if( ZigBee_buffer[1] == '1' )
			{
				if( ZigBee_buffer[2] == 'O' )
				{
                    LCD_P6x8Str(0,6,(unsigned char*)"Relay1:ON ");
					APP_buffer[69] = 'O';
					APP_buffer[70] = 'N';
					APP_buffer[71] = ' ';
				}
				else
				{	
                    LCD_P6x8Str(0,6,(unsigned char*)"Relay1:OFF");					
					APP_buffer[69] = 'O';
					APP_buffer[70] = 'F';
					APP_buffer[71] = 'F';										
				}
			}
			else
			{
				if( ZigBee_buffer[2] == 'O' )
				{
                    LCD_P6x8Str(0,7,(unsigned char*)"Relay2:ON ");
					APP_buffer[76] = 'O';
					APP_buffer[77] = 'N';
					APP_buffer[78] = ' ';
				}
				else
				{
                    LCD_P6x8Str(0,7,(unsigned char*)"Relay2:OFF");
					APP_buffer[76] = 'O';
					APP_buffer[77] = 'F';
					APP_buffer[78] = 'F';					
				}
			}
		}
	}	
}

