/************************************************************************
* Include File Section              
************************************************************************/
#include "bsp_init.h"


/************************************************************************
* Function Define Section               
************************************************************************/




static void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);							// LED1
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_5);							// LED2
}

static void Beep_Init(void)									// ������
{
	GPIO_InitTypeDef GPIO_Initstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);							
}

static void ZigBee_Power_Init(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_1);							// ZigBee 1
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);							// ZigBee 2
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);						// Humidification
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_Initstructure);
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);						// Fan
}

static void Misc_Init(void)     // �������豸��ʼ��
{
    ZigBee_Power_Init();
    Beep_Init();
    LED_Init();
    Key_Init();
    
    Breathing_Light_Init();		                                    // ������
    Servo_Door_Init(199, 7199);									    // �� PWM��ʼ������Ϊ20ms��50HZ
//    Fan_Init(255, 1999);
}

void BSP_Init(void)
{
    SysTick_Init();             // ϵͳ�δ�ʱ�ӳ�ʼ��
    
    WIFI_BufferPool_Init();
    ZigBee_BufferPool_Init();    	    
    USART_Configuration();
    
     
    
    InitRc522();													// ˢ��ģ���ʼ��
    W25QXX_Init();													// Flash��W25Q128�ĳ�ʼ��
//	Del_card_ID();       											// ���ȫ������													
	Read_Flash_ID();												// ������
  
    Misc_Init();  
    
    
    
    OLED_Init();
    LCD_P8x16Str(0,0,(unsigned char*)"Initialization !");
    
//    WiFi_Init(STA_AP);
    
    
    
    LCD_CLS();
	LCD_P8x16Str(24,2,(unsigned char*)"Init done!");
	delay_ms (100);
    LCD_CLS();
    
}
