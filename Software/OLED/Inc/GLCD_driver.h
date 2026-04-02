/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLCD_DRIVER_H
#define __GLCD_DRIVER_H


#include "stm32l0xx_hal.h"
//#include "i2c.h"

#define 	oled_addr 			0x78
#define 	Contrast_volue 	0xFF
#define  	Column   64
#define  	Row      48
#define  	segment  32	    //IC start segment line
//volatile 	uint8_t i = 0;
//uint8_t		OledBuf[8];

extern const unsigned char FONT1[95][5];

void LCD_OLED_Write_command (uint8_t valor);
void LCD_OLED_Write_data (uint8_t valor);
void LCD_OLED_Set_Linha_Coluna (uint8_t linha, uint8_t coluna); 
void LCD_OLED_show_pic(const unsigned char *p);
void LCD_OLED_Init(void);
void LCD_OLED_Monta_Linha_Coluna_Texto(unsigned char linha, unsigned char coluna, char dados[]);

void LCD_OLED_Draw_bitmap(unsigned char xb, unsigned char x_size, unsigned char yb, unsigned char y_size, unsigned char *bmp_img);
void LCD_OLED_Clear_screen(void);
void LCD_OLED_Fill_screen(void);

void LCD_OLED_Set_font(unsigned char * font, unsigned char font_height,  unsigned char font_width);

unsigned char LCD_OLED_Le_dado(unsigned char linha, unsigned char coluna);

void LCD_OLED_escreve_texto( unsigned char linha, unsigned char coluna, unsigned char data[] );

void LCD_OLED_Write_buffer(unsigned char* buffer);

void LCD_OLED_Flip (void);

#endif /*__GLCD_DRIVER_H*/
