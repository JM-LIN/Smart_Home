/************************************************************************
* Include File Section              
************************************************************************/
#include "bsp_init.h"


/************************************************************************
* Function Define Section               
************************************************************************/

static void Misc_Init(void)     // 杂项类设备初始化
{
    ZigBee_Power_Init();
    Humidifier_Init();
    LED_Init();
    Beep_Init();
    Key_Init();
	Infrared_Init();
    
	
    Breathing_Light_Init();		                            // 呼吸灯
	Servo_Window_Init(199, 7199);			                // 窗帘舵机PWM初始化	
    Servo_Door_Init(199, 7199);								// 门 PWM初始化周期为20ms，50HZ
    Fan_Init(999, 0);                                       // 定时器计数1000次
}

void BSP_Init(void)
{
    SysTick_Init();             // 系统滴答时钟初始化
    
    
    WIFI_BufferPool_Init();
    ZigBee_BufferPool_Init();    	    
    USART_Configuration();
    
	
	
    SYN7318_Reset_Init();           // 语音模块复位
    
    Misc_Init();
    
    InitRc522();													// 刷卡模块初始化
    W25QXX_Init();													// Flash的W25Q128的初始化
//	Del_card_ID();       											// 清空全部卡号													
	Read_Flash_ID();												// 读人数
  

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
