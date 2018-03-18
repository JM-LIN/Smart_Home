/************************************************************************
* Include File Section              
************************************************************************/
#include "bsp_init.h"


/************************************************************************
* Function Define Section               
************************************************************************/

static void Misc_Init(void)     // �������豸��ʼ��
{
    ZigBee_Power_Init();
    Humidifier_Init();
    LED_Init();
    Beep_Init();
    Key_Init();
	Infrared_Init();
    
	
    Breathing_Light_Init();		                            // ������
	Servo_Window_Init(199, 7199);			                // �������PWM��ʼ��	
    Servo_Door_Init(199, 7199);								// �� PWM��ʼ������Ϊ20ms��50HZ
    Fan_Init(999, 0);                                       // ��ʱ������1000��
}

void BSP_Init(void)
{
    SysTick_Init();             // ϵͳ�δ�ʱ�ӳ�ʼ��
    
    
    WIFI_BufferPool_Init();
    ZigBee_BufferPool_Init();    	    
    USART_Configuration();
    
	
	
    SYN7318_Reset_Init();           // ����ģ�鸴λ
    
    Misc_Init();
    
    InitRc522();													// ˢ��ģ���ʼ��
    W25QXX_Init();													// Flash��W25Q128�ĳ�ʼ��
//	Del_card_ID();       											// ���ȫ������													
	Read_Flash_ID();												// ������
  

    OLED_Init();
    LCD_P8x16Str(0,0,(unsigned char*)"Initialization !");
    
#ifdef SIM7600
    SIM7600_Init();
#else
    WiFi_Init(STA_AP);
#endif
    TIM6_Configuration();
    
    LCD_CLS();
	LCD_P8x16Str(24,2,(unsigned char*)"Init done!");
	delay_ms (1000);
    LCD_CLS();
    
}
