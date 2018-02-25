#ifndef _OLED_H
#define _OLED_H

#include <stm32f10x.h>


#define byte unsigned char  
#define word unsigned int   
	
#define LCD_SCL_H 			GPIO_SetBits(GPIOB,GPIO_Pin_5) 
#define LCD_SCL_L 			GPIO_ResetBits(GPIOB,GPIO_Pin_5) 
			
#define LCD_SDA_H 			GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define LCD_SDA_L 			GPIO_ResetBits(GPIOB,GPIO_Pin_6)
			
#define LCD_RST_H 			GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LCD_RST_L 			GPIO_ResetBits(GPIOB,GPIO_Pin_7)
			
#define LCD_DC_H  			GPIO_SetBits(GPIOB,GPIO_Pin_8) 
#define LCD_DC_L  			GPIO_ResetBits(GPIOB,GPIO_Pin_8) 

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)	  
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xcf 				//0xCF 

extern unsigned char *Server_ID;

extern void OLED_Init(void);
extern void OLED_Show(void);
extern void OLED_RST(void);

void LCD_Init(void);
void LCD_CLS(void);
void LCD_CLS_LINE(byte y);
void LCD_DLY_ms(word ms);
void adjust(byte c);
void LCD_P6x8Str(byte x,byte y,byte ch[]);  
void LCD_P8x16Str(byte x,byte y,byte ch[]);
void LCD_P14x16Str(byte x,byte y,byte ch[]);
void LCD_Print(byte x, byte y, byte ch[]);      // y 6~0     
void LCD_PutPixel(byte x,byte y);               //»­µã
void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
void OLED_DrawBMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 
void LCD_Fill(byte dat);

void LED_P6x8Char(int8_t x,int8_t y,int8_t ch);                         
void LED_PrintValueC(uint8_t x, uint8_t y, int8_t data);
void LED_PrintValueI(uint8_t x, uint8_t y, int16_t data);
void LED_PrintValueFP(uint8_t x, uint8_t y, uint16_t data, uint8_t num);
void LED_PrintValueFP_my(uint8_t x, uint8_t y, uint16_t data, uint8_t num);
void LED_PrintValueF(uint8_t x, uint8_t y, float data, uint8_t num);
void LCD_show_Data(u8 x,u8 line,u16 var);
void LCD_signal_num(uint8_t x,uint8_t line,uint8_t var);


 
#endif

