	/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: oled.c
  Author: Giayee       Version : V1.0        Date:15/07/15
  Description:
    This file provides all oled information functions.
***********************************************************/
#include "string.h"
#include "stm32f10x_spi.h"
#include "oled.h"
#include "font.h"
#include "tim.h"
#include "battery.h"
#include "flash.h"

//OLED display buffer
//format:
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
uint8_t OLED_GRAM[8][128];
uint32_t oled_flag = 0;
uint16_t oled_init = 0;
#define DATA_ADDR			0x08002FFC

void SPI_WriteByte(uint8_t byte)
{
#ifdef USE_HW_SPI
	/* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    /* Send byte through the SPI2 peripheral */
    SPI_I2S_SendData(SPI2, byte);
#else
	uint8_t i, value = byte;

	for(i=0; i<8; i++)
	{
		SPI_CLK_L;
			asm("NOP");asm("NOP");
		asm("NOP");asm("NOP");
		//		_nop_();_nop_();
		if(value & 0x80)	SPI_SI_H;
		else				SPI_SI_L;
		asm("NOP");asm("NOP");
		asm("NOP");asm("NOP");
		asm("NOP");asm("NOP");
		asm("NOP");asm("NOP");
		asm("NOP");asm("NOP");
		asm("NOP");asm("NOP");
		SPI_CLK_H;
		asm("NOP");asm("NOP");
		asm("NOP");asm("NOP");
//		_nop_();_nop_();
		value = value<<1;//_crol_(value, 1);
	}
#endif
}

void Write_Command(uint8_t cmd)
{
#ifdef USE_HW_SPI
	SPI_CS_L;
	SPI_A0_L;
	SPI_WriteByte(cmd);
	SPI_CS_H;
#else
	SPI_CS_L;

	SPI_CLK_L;
	asm("NOP");asm("NOP");
//		_nop_();_nop_();
	SPI_SI_L;
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	SPI_CLK_H;
	asm("NOP");asm("NOP");
//		_nop_();_nop_();
	SPI_WriteByte(cmd);

	SPI_CS_H;
#endif
}

void Write_Data(uint8_t data)
{
#ifdef USE_HW_SPI
	SPI_CS_L;
	SPI_A0_H;
	SPI_WriteByte(data);
	SPI_CS_H;
#else
	SPI_CS_L;

	SPI_CLK_L;
	asm("NOP");asm("NOP");
//		_nop_();_nop_();
	SPI_SI_H;
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");
	SPI_CLK_H;
	asm("NOP");asm("NOP");
//		_nop_();_nop_();
	SPI_WriteByte(data);

	SPI_CS_H;
#endif
}

void Init_IC()
{
	Write_Command(0xAE);     //Set Display Off

	Write_Command(0xd5);     //display divide ratio/osc. freq. mode
	Write_Command(0x80);     //

	Write_Command(0xA8);     //multiplex ration mode:63
	Write_Command(0x3F);

	Write_Command(0xD3);     //Set Display Offset
	Write_Command(0x00);

	Write_Command(0x40);     //Set Display Start Line

	//for 1106 controller
	Write_Command(0xAD);     //DC-DC Control Mode Set 8D 14
	Write_Command(0x8B);     //DC-DC ON/OFF Mode Set

	//for 1306 controller
	Write_Command(0x8D);     //DC-DC Control Mode Set
	Write_Command(0x14);     //DC-DC ON/OFF Mode Set

	Write_Command(0xA1);     //Segment Remap

	Write_Command(0xC8);     //Sst COM Output Scan Direction

	Write_Command(0xDA);     //common pads hardware: alternative
	Write_Command(0x12);

	Write_Command(0x81);     //contrast control
	Write_Command(0xFF);

	Write_Command(0xD9);	    //set pre-charge period
	Write_Command(0x22);

	Write_Command(0xDB);     //VCOM deselect level mode
	Write_Command(0x40);	    //40

	Write_Command(0xA4);     //Set Entire Display On/Off

	Write_Command(0xA6);     //Set Normal Display

	Write_Command(0xAF);     //Set Display On
}

void init_ug_ic()
{
	Write_Command(0xae);//--turn off oled panel
	Write_Command(0x8d);//--set Charge Pump enable/disable
	Write_Command(0x10);//--set(0x10) disable
//	Write_Command(0x00);//---set low column address
//	Write_Command(0x10);//---set high column address
//	Write_Command(0x40);//--set start line address
	Write_Command(0x81);//--set contrast control register
	Write_Command(0xcf);
	Write_Command(0xa1);//--set segment re-map 95 to 0
	Write_Command(0xa6);//--set normal display
	Write_Command(0xa8);//--set multiplex ratio(1 to 64)
	Write_Command(0x3f);//--1/64 duty
	Write_Command(0xd3);//-set display offset
	Write_Command(0x00);//-not offset
	Write_Command(0xd5);//--set display clock divide ratio/oscillator frequency
	Write_Command(0x80);//--set divide ratio
	Write_Command(0xd9);//--set pre-charge period
	Write_Command(0xf1);
	Write_Command(0xC8);     //Sst COM Output Scan Direction
	Write_Command(0xda);//--set com pins hardware configuration
	Write_Command(0x12);
	Write_Command(0xdb);//--set vcomh
	Write_Command(0x40);
	Write_Command(0x8d);//--set Charge Pump enable/disable
	Write_Command(0x14);//--set(0x10) disable
	//for 1106 controller
	Write_Command(0xAD);     //DC-DC Control Mode Set 8D 14
	Write_Command(0x8B);     //DC-DC ON/OFF Mode Set
	delay_ms(100);
	Write_Command(0xaf);//--turn on oled panel
}

void OLED_Clear()
{
	uint8_t page_number, column_number;
	for(page_number=0; page_number<8; page_number++)
	{
		for(column_number=0; column_number<128; column_number++)
		{
			OLED_GRAM[page_number][column_number] = 0x00;
		}
	}
}

void OLED_All_Screen()
{
	uint8_t page_number, column_number;
	for(page_number = 0; page_number < 8; page_number++)
	{
		for(column_number = 0; column_number < 128; column_number++)
		{
			OLED_GRAM[page_number][column_number] = 0xff;
		}
	}
}

uint32_t read_word_data(uint32_t addr)
{
	return *(uint32_t *)addr;
}


void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t mode)
{
	uint8_t page, temp;
	if (x>127 || y>63)		return;
	page = y / 8;
	temp = 1 << (y%8);
	if(mode)	OLED_GRAM[page][x] |= temp;
	else 		OLED_GRAM[page][x] &= ~temp;
}

/*new showchar function, can show new font type*/
uint8_t OLED_Showchar(uint8_t x, uint8_t y, uint8_t chr, enum font_t font, uint8_t size, uint8_t mode)
{

	uint8_t temp, t, t1;
	uint8_t y0 = y;

	chr = chr - ' ';//get the offset

	if (x>127 || y>63)		return 0;

	if(font == normal)
	{
		for (t = 0; t < size; t++)
		{
			if (size == 12)			temp = asc2_1206[chr][t];  		//font:1206
			else if (size == 16) 	temp = asc2_1608[chr][t];	 	//font:1608
			else					temp = asc2_3208[chr][t];		//font:3208

			for (t1 = 0; t1 < 8; t1++)
			{
				if (temp&0x80)	OLED_DrawPoint(x, y, mode);
				else 			OLED_DrawPoint(x, y, !mode);

				temp<<=1;
				y++;
				if ((y-y0) == size)
				{
					y=y0;
					x++;
					break;
				}
			}
		}
	}else
	{
		for (t = 0; t < (size*size/8); t++)
		{
			if(font == font_1)
			{
				if (size == 16) 	temp = font1_1616[chr][t];	 	//font_1:1616
				else if(size == 32) temp = font1_3232[chr][t]; //font_1:3232				
			}
			if(font == font_2)
			{
				if(size == 16) temp = font2_1616[chr][t]; //font_2:1616
				else if (size == 32) temp = font2_3232[chr][t]; //font_2:3232
			}
			for (t1 = 0; t1 < 8; t1++)
			{
			  if (temp&0x80)	OLED_DrawPoint(x, y, mode);
				else 			OLED_DrawPoint(x, y, !mode);

				temp<<=1;
				y++;
				if ((y-y0) == size)
				{
					y=y0;
					x++;
					break;
				}
			}
		}
	return size;
	}
	return 8;
}

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
	uint8_t temp, t, t1;
	uint8_t y0 = y;
	chr = chr - ' ';//get the offset

	if (x>127 || y>63)		return;

    for (t = 0; t < size; t++)
    {
			if (size == 12)			temp = asc2_1206[chr][t];  		//font:1206
			else if (size == 16) 	temp = asc2_1608[chr][t];	 	//font:1608
			else					temp = asc2_3208[chr][t];		//font:3208

			for (t1 = 0; t1 < 8; t1++)
			{
				if (temp&0x80)	OLED_DrawPoint(x, y, mode);
				else 			OLED_DrawPoint(x, y, !mode);

				temp<<=1;
				y++;
				if ((y-y0) == size)
				{
					y=y0;
					x++;
					break;
				}
			}
    }
}

uint32_t mypow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
	{
		result *= m;
	}
	return result;
}


void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
	uint8_t t,temp;
	uint8_t enshow=0;

	if (x>127 || y>63)		return;

	for (t = 0; t < len; t++)
	{
		temp=(num/mypow(10, len-t-1))%10;
		if (enshow==0&&t<(len-1))
		{
			if (temp==0)
			{
				OLED_ShowChar(x+8*t, y, '0', size, 1);
				continue;
			}else enshow=1;
		}
	 	OLED_ShowChar(x+8*t, y, temp+'0', size, 1);
	}
}

/* new shownum function,can show new font type*/
void OLED_Shownum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, enum font_t font, uint8_t size)
{
	uint8_t t,temp;
	uint8_t enshow=0;

	if (x>127 || y>63)		return;

	for (t = 0; t < len; t++)
	{
		temp=(num/mypow(10, len-t-1))%10;
		if (enshow==0&&t<(len-1))
		{
			if (temp==0)
			{
				OLED_Showchar(x+8*t, y, '0', font, size, 1);
				continue;
			}else enshow=1;
		}
	 	OLED_Showchar(x+8*t, y, temp+'0',font, size, 1);
	}
}

void show_uid(uint8_t x, uint8_t y, uint8_t uid[], uint8_t uid_len)
{
	if(uid_len == 4)
	{
		for(uint8_t i=0; i<uid_len; i++)
		{
			OLED_ShowNum(x + i*24, y, uid[i], 2, 16);
		}
	}
	if(uid_len == 7)
	{
		for(uint8_t i=0; i<uid_len; i++)
		{
			OLED_ShowNum(x + 18*i, 48, uid[i], 2, 16);
		}
	}
	OLED_Refresh_Gram();
}


/*new showstring function, can show new font type */
void OLED_Showstring(uint8_t x, uint8_t y, const char *p, enum font_t font, uint8_t size)
{
#define MAX_CHAR_POSX (128-8)
#define MAX_CHAR_POSY (64 - size)
uint8_t offset;
	if (x>127 || y>63)		return;

    while (*p != '\0')
    {
        if (x>MAX_CHAR_POSX)
		{
			x=0;
			y += size;
		}
        if (y>MAX_CHAR_POSY)
		{
			y=x=0;
			OLED_Clear();
		}
        offset = OLED_Showchar(x, y, *p, font, size, 1);
				if(font == normal) x += 8;
				else				
        x += offset;
        p++;
    }
}

void OLED_ShowString(uint8_t x, uint8_t y, const char *p, uint8_t size)
{
#define MAX_CHAR_POSX (128-8)
#define MAX_CHAR_POSY (64 - size)

	if (x>127 || y>63)		return;

    while (*p != '\0')
    {
        if (x>MAX_CHAR_POSX)
		{
			x=0;
			y += size;
		}
        if (y>MAX_CHAR_POSY)
		{
			y=x=0;
			OLED_Clear();
		}
        OLED_ShowChar(x, y, *p, size, 1);
        x += 8;
        p++;
    }
}

void OLED_ShowImage(uint8_t x, uint8_t y, uint8_t len, uint8_t wid, const uint8_t *p, uint8_t mode)
{
	int temp, t1, t2;
	uint8_t y0 = y;

	if (x > 127 || y > 63)		return;

	for (t2 = 0; t2 < (len*wid/8); t2++)
	{
		temp = *p;
		p++;

		for (t1=0; t1<8; t1++)
		{
			if (temp & 0x80)	OLED_DrawPoint(x, y, mode);
			else 			OLED_DrawPoint(x, y, !mode);

			temp<<=1;
			y++;
			if((y-y0) == wid)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

void OLED_ShowRect(uint8_t x, uint8_t y, uint8_t len, uint8_t wid, uint8_t mode, uint8_t clear)
{
  int m, n;
  if (x>120 || y>60)		return;

  if (mode)
  {
	  for (n=y; n<y+wid; n++)
		for (m=x; m < x+len; m++)
		  OLED_DrawPoint(m, n,1);
	}
	else
	{
	  for (n=y; n<y+wid; n++)
		for (m=x; m < x+len; m++)
		  if(n==y||n==(y+wid-1))
			OLED_DrawPoint(m, n, 1);
		  else
		  {
			OLED_DrawPoint(m, n, 1);
			if(m==x+len-1) break;
			else m=x+len-2;
		  }
	}
  }

void OLED_ShowPower(uint8_t per)
{
    int i, y = 58;
	static uint8_t last_per = 0;

	if(GPIOA->IDR >> 15)
	{
		if(last_per != 0)
		{
			if(per > last_per)
			{
				per = last_per;
			}
		}
	}
	else								//On charging: battery is only raising
	{		
		if(last_per != 0)
		{
			if(per < last_per)
			{
				per = last_per;
			}
		}
	}
	last_per = per;

	if(per >= 100) 
	{
		per = 99;
	}
	per /= 10;

	for(i=0; i<=per; ++i)
	{
		OLED_ShowRect(0, y, 8, 4, 1, 0);                     //the rectangle's length is 8 pixels and wid is 4 pixels
		y=y-6;
	}
	for(; i<10; i++)
	{
		OLED_ShowRect(0, y, 8, 4, 1, 1);                     //the rectangle's length is 8 pixels and wid is 4 pixels
		y=y-6;
	}
}

void OLED_reset(void)
{
	GPIO_ResetBits(OLED_RST_PORT, LCD_RST);
	delay_ms(100);
	GPIO_SetBits(OLED_RST_PORT, LCD_RST);
	delay_ms(10);
}

#define OLED_STATUS	0x0003
void OLED_Refresh_Gram(void)
{
	uint8_t page_number, column_number;
	for (page_number = 0; page_number < 8; page_number++)
	{
		Write_Command(0xb0+page_number);
		Write_Command(0x10);
		if(((oled_flag>>16) == 0x5678) && (oled_init == OLED_STATUS))	//红标屏幕
		{
			Write_Command(0x02);
		}
		for (column_number = 0; column_number < 128; column_number++)
		{
			Write_Data(OLED_GRAM[page_number][column_number]);
		}
	}
}

void OLED_Init(void)
{
	OLED_reset();
//	Init_IC();
	init_ug_ic();
	delay_ms(10);
	oled_flag = read_word_data(DATA_ADDR);
	oled_init = (uint16_t)(oled_flag);
	OLED_Clear();
	OLED_Refresh_Gram();
	
	if(oled_init != OLED_STATUS)		//没有初始化OLED_ID
	{
		OLED_test();
	}
}

#include "stm32f10x_flash.h"
#define STM32_FLASH_BASE	0x08000000
#define STM_SECTOR_SIZE 	1024			//byte
static void write_data(uint32_t writeAddr, uint16_t *pBuffer, uint16_t len)
{
	uint32_t secpos;	//sector address
	uint16_t i;
	uint32_t offaddr;		//offset to 0x08000000
	
//	if((writeAddr < 0x08013000) || (writeAddr >= 0x08020000))	return ;
	
	FLASH_Unlock();
	
	offaddr = writeAddr - STM32_FLASH_BASE;
	secpos = offaddr / STM_SECTOR_SIZE;
	
	FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);	//erasa page
	
	uint32_t writePos = writeAddr;
	for(i=0; i<len; i++)
	{
		FLASH_ProgramHalfWord(writePos,pBuffer[i]);
		writePos+=2;
	}
	FLASH_Lock();
}

extern uint8_t key_flag;
static uint16_t flash_data[512];
void OLED_test(void)
{
#define OLED_DATA_ADDR 	0x8002c00
	uint16_t i;
//	uint8_t flash_data[1024];
	uint32_t readAddr = 0x8002c00;
	
//	read_flash_data(0x8002c00, (uint16_t*)flash_data, 512);
	
	for(i=0; i<512; i++)
	{
		flash_data[i]=*(uint16_t*)readAddr;
		readAddr += 2;
	}
	
	flash_data[510] = OLED_STATUS;
	oled_init = OLED_STATUS;
		
	for(i=0; i<128; i++)
	{
		OLED_DrawPoint(i, 0, 1);
		OLED_DrawPoint(i, 63, 1);
	}
	for(i=0; i<63; i++)
	{
		OLED_DrawPoint(0, i, 1);
		OLED_DrawPoint(127, i, 1);
	}
	OLED_Refresh_Gram();
	key_flag = 0;
	delay_ms(5000);
	
	if(key_flag >= 1)//按键按下，表示屏幕出现偏移，向flash_data[1023] 写 1
	{
		flash_data[511] = 0x5678;	//表示红标屏
		oled_flag = 0x5678<<16;		
	}
	else	//表示没有按键按下，是蓝标屏幕，向flash_data[1023] 写 0
	{
		flash_data[511] = 0x1234;	//表示蓝标屏
		oled_flag = 0x1234<<16;
	}
	
	write_data(0x8002c00, (uint16_t*)flash_data, 512);	
}

/* oled测试truetype。ttf 字体*/
void OLED_test2(void)
{
	uint8_t i;

	OLED_Showstring(16, 16, "oled test...",font_1, 16);
	OLED_Refresh_Gram();
	delay_ms(2000);

	for(i=0; i<3; i++)
	{
	  OLED_Clear();
	  OLED_Refresh_Gram();
	  delay_ms(500);

	  OLED_All_Screen();
	  OLED_Refresh_Gram();
	  delay_ms(500);
	}
	OLED_Clear();
	OLED_Showstring(16, 16, "oled test ok!",font_1, 16);
	OLED_Refresh_Gram();
}
