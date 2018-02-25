/*
		ZigBee����� �Զ�����Ļ�����
*/
/************************************************************************
* Include File Section         
************************************************************************/
#include "ZigBee_BufferPool.h"


/************************************************************************
* Global Variable Declare Section   
************************************************************************/
volatile unsigned char APP_buffer[APP_Size_Max]                 = {0,0};		// ���ڴ洢APP��ʾ��Ϣ
volatile unsigned char ZigBee_buffer[ZigBee_Flow_Size_Max]      = {0,0};        // ����صĻ�����
volatile unsigned char ZigBee_flowsize                          = 0;            // �����С
volatile unsigned char ZigBee_flowflag                          = 0;            // ������
volatile unsigned char ZigBee_cancelflag                        = 0;            // ȡ�����
volatile unsigned char ZigBee_wLoc                              = 0;		    // ��ǰд��λ��
volatile unsigned char Socket_Status;   


/************************************************************************
* Function Define Section               
************************************************************************/
// д������
void ZigBee_BPWriteData(unsigned char data)
{
	ZigBee_buffer[ZigBee_wLoc++] = data;        // д�뻺����
	if(ZigBee_wLoc == ZigBee_flowsize)	        // ����Ƿ����
        ZigBee_flowflag = 1;    
	ZigBee_Data_Display();	
}

// ��������
void ZigBee_BPReadData(unsigned char *data)
{
	char i;
	
	ZigBee_wLoc = 0;                            // ����д��λ��
	ZigBee_flowflag = 0;                        // ����������
    
	for(i=0; i<ZigBee_flowsize; i++)            // ��������
		data[i] = ZigBee_buffer[i];
}

// ����Ƿ��Ѿ����
char ZigBee_BPCheckFlow(void)
{
	return ZigBee_flowflag;
}

// ����Ƿ���ȡ�������������
static char ZigBee_BPCheckCancelCmd(void)
{
	if(ZigBee_cancelflag)
	{
		ZigBee_cancelflag = 0;                   // ����ȡ�����
		ZigBee_flowflag   = 0;                   // ����������
		return 1;
	}
	else return 0;
}

// ���������С
void ZigBee_SetFlowSize(unsigned char val)
{
	ZigBee_flowsize = val;
}

// ��ʼ�������
void ZigBee_BufferPool_Init(void)
{
	char i;
	for(i=0; i<ZigBee_Flow_Size_Max; i++)
		ZigBee_buffer[i] = '0';
		
	ZigBee_flowsize = ZigBee_Flow_Size_Max;         // ��ʼ�������С	
	ZigBee_flowflag = 0;                            // ��ʼ��������	
	ZigBee_cancelflag = 0;                          // ��ʼ��ȡ�����
	ZigBee_wLoc = 0;                                // ��ʼ����ǰд��λ��
    
    ZigBee_SetFlowSize(7);                          // д���ֵ
    ZigBee_Data_Display_Init();
}

static void ZigBee_Data_Display_Init(void)
{
	memset(APP_buffer,'0',sizeof(APP_buffer));
	
	// ��ʪ��
	APP_buffer[26] = 'T';
	APP_buffer[27] = 'M';
	APP_buffer[30] = 'C';
	APP_buffer[31] = '_';
	APP_buffer[32] = 'H';
	APP_buffer[33] = 'M';
	APP_buffer[36] = 'P';
	APP_buffer[37] = '_';
	
	// ����Ũ��
	APP_buffer[38] = 'S';
	APP_buffer[39] = 'M';
	APP_buffer[40] = 'S';
	APP_buffer[44] = 'p';
	APP_buffer[45] = 'p';
	APP_buffer[46] = 'm';
	APP_buffer[47] = '_';
	
	// ��ǿ
	APP_buffer[48] = 'L';
	APP_buffer[49] = 'S';
	APP_buffer[55] = 'l';
	APP_buffer[56] = 'x';
	APP_buffer[57] = '_';
	
	// ����
	APP_buffer[58] = 'W';
	APP_buffer[59] = 'E';
	APP_buffer[65] = '_';
	
	// ����1
	APP_buffer[66] = 'S';
	APP_buffer[67] = 'S';
	APP_buffer[68] = 'O';
	APP_buffer[72] = '_';
	
	// ����2
	APP_buffer[73] = 'S';
	APP_buffer[74] = 'S';
	APP_buffer[75] = 'T';
	APP_buffer[79] = '_';
	
	// ��ʪ
	APP_buffer[80] = 'H';
	APP_buffer[81] = 'T';
	APP_buffer[85] = '_';
	
	// ����
	APP_buffer[86] = 'A';
	APP_buffer[87] = 'R';
	APP_buffer[91] = '_';
	
	// ģʽ
	APP_buffer[92] = 'P';
	APP_buffer[93] = 'M';
	APP_buffer[98] = '\0';
}

// ������ʾ������
void ZigBee_Data_Display(void)
{
	if(ZigBee_wLoc == 7)			// ��ʪ��
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
	if(ZigBee_wLoc == 4)			// ����Ũ��
	{
		if( ZigBee_buffer[0] == 'M' )
		{	
			LCD_P6x8Str(0,3,(unsigned char*)ZigBee_buffer);			
			APP_buffer[41] = ZigBee_buffer[1];
			APP_buffer[42] = ZigBee_buffer[2];
			APP_buffer[43] = ZigBee_buffer[3];			
		}
	}
	if(ZigBee_wLoc == 7)			// ����ǿ��
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

		if( ZigBee_buffer[0] == 'S' && ZigBee_buffer[1] == 'U')			    // ���ģ��
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
		
		if( ZigBee_buffer[0] == 'S')			                            // �����Ӧ
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
	if(ZigBee_wLoc == 3)								                    // �̵���
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

