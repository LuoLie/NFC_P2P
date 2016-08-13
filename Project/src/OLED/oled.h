/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: oled.h
  Author: Giayee       Version : V1.0        Date:15/07/15
  Description:
    This file provides all oled information functions.
  User can call function of below to get information you need.
***********************************************************/

#include "stm32f10x.h"
#include "bsp.h"

enum font_t
{
	normal = 0,
	font_1 = 1,
	font_2 = 2,	
};

#define SPI_CS_H	GPIO_SetBits(OLED_PORT, SPI_CS)
#define SPI_CS_L	GPIO_ResetBits(OLED_PORT, SPI_CS)

#define SPI_CLK_H	GPIO_SetBits(OLED_PORT, SPI_CLK)
#define SPI_CLK_L	GPIO_ResetBits(OLED_PORT, SPI_CLK)

#define SPI_SI_H	GPIO_SetBits(OLED_PORT, SPI_SI)
#define SPI_SI_L	GPIO_ResetBits(OLED_PORT, SPI_SI)

/*************************************************
  Function:      OLED_ShowNum
  Description:   display a number
  Input:         x(0~127), y(0~63) : the index in the OLED
				 num:value(0~4294967295)
				 len :length of the number
				 size : select the font 12/16/32
  Output:         None
  Return:		  None
*************************************************/
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);

/*************************************************
  Function:      OLED_Shownum
  Description:   display a number
  Input:         x(0~127), y(0~63) : the index in the OLED
				 num:value(0~4294967295)
				 len :length of the number
				 font:font of the number
				 		normal：the normal font 
						font_1: truetype.ttf font
				 size : select the font 16
  Output:         None
  Return:		  None
*************************************************/
void OLED_Shownum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, enum font_t font, uint8_t size);
/*************************************************
  Function:      OLED_ShowString
  Description:   display a string
  Input:         x(0~127), y(0~63) : the index in the OLED
				 *p : pointer of the string
				 size : select the font 12/16/32
  Output:         None
  Return:		  None
*************************************************/
void OLED_ShowString(uint8_t x, uint8_t y, const char *p, uint8_t size);
/*************************************************
  Function:      OLED_Showstring
  Description:   display a string
  Input:         x(0~127), y(0~63) : the index in the OLED
				 *p : pointer of the string
				 font:font of the words
						normal：the normal font 
						font_1: truetype.ttf font
				 size : select the font 12/16/32
  Output:         None
  Return:		  None
*************************************************/
void OLED_Showstring(uint8_t x, uint8_t y, const char *p, enum font_t font, uint8_t size);

/*************************************************
  Function:      OLED_ShowImage
  Description:   display a image
  Input:         x(0~127), y(0~63) : the index in the OLED
				 len : length of the image, should be a multiple of 8
				 wid : width of the image, should be a multiple of 8
				 *p : pointer of the image array
				 mode: to judge the output is "1" or "0"
  Output:         None
  Return:		  None
*************************************************/
void OLED_ShowImage(uint8_t x,uint8_t y,uint8_t len,uint8_t wid,const uint8_t *p,uint8_t mode);

/*************************************************
  Function:      OLED_ShowRect
  Description:   Draw rectangles
  Input:         x(0~120),y(0~60): where to draw rectangles
                 len,wid: the length and width of the rectangle.
                 mode:1,filling the rectangle. 0,don't fill the rectangle.
				 clear:1 clear the Rectangle
					   0 show the Rectangle
  Output:         None
  Return:		  None
  ATTENTION PLEASE: THE RECTANGLES IS DRAWING FROM THE BOTTOM UP.
*************************************************/
void OLED_ShowRect(uint8_t x, uint8_t y, uint8_t len, uint8_t wid, uint8_t mode, uint8_t clear);

/*************************************************
  Function:      OLED_ShowPower
  Description:   show power of the nfc watch.
  Input:          per : the percentage of power.
  Output:         None
  Return:		  None
*************************************************/
void OLED_ShowPower(uint8_t per);

void OLED_Init(void);
/*************************************************
  Function:      OLED_Clear
  Description:   clear the whole OLED
  Input:          None
  Output:         None
  Return:		  None
*************************************************/
void OLED_Clear(void);

/*************************************************
  Function:      OLED_All_Screen
  Description:   light up the whole OLED
  Input:          None
  Output:         None
  Return:		  None
*************************************************/
void OLED_All_Screen(void);

/*************************************************
  Function:      OLED_Refresh_Gram
  Description:   refresh the value of OLED_GRAM
  Input:          None
  Output:         None
  Return:		  None
*************************************************/
void OLED_Refresh_Gram(void);

/*************************************************
  Function:       OLED_test
  Description:    to test if the OLED is OK
  Input:          None
  Output:         None
  Return:		  None
*************************************************/
void OLED_test(void);

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t mode);
void show_uid(uint8_t x, uint8_t y, uint8_t uid[], uint8_t uid_len);
