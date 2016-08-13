/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: rw Mifare tag.cpp
  Author: Giayee       Version : V1.1        Date:15/11/12
  Description:       
    This file provides function about read and write Mifare Tag
***********************************************************/

#include "stdio.h"
#include "math.h"
#include "PN532_HSU.h"
#include "string.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "snep.h"
#include "bsp.h"
#include "oled.h"
#include "PN532_debug.h"

#define	GENERIC_PASSIVE_106	0x00
#define	GENERIC_PASSIVE_212	0x01
#define	GENERIC_PASSIVE_424	0x02
#define DEP_PASSIVE_106	0x40
#define DEP_PASSIVE_212	0x41
#define DEP_PASSIVE_424	0x42
#define DEP_ACTIVE_106	0x80
#define DEP_ACTIVE_212	0x81
#define DEP_ACTIVE_424	0x82

void autoPoll(void)
{
	uint8_t buf[30] = {0};
	
	PN532_HSU 	pn532_hsu;
	PN532		pn532(pn532_hsu);
	
	uint8_t 	res = 0;
	int8_t	success = 0;
	
	res = pn532.begin();
	
	if(res == 0)							//wake-up failed
	{
		led_flicker(LED_PORT, LED_G, 200, 5);	
		return;
	}
	
	success = pn532.InAutoPoll(0x0f, 0x01, DEP_ACTIVE_424, buf, 30);
	
	if(success > 0)//found the NFC target
	{
		xprintf("found the NFC target!!!\r\n");
		for(uint8_t i=0; i<success; i++)
			xprintf("%x ", buf[i]);
		xprintf("\r\n");
	}
}

void autoPollTest(void)
{
	while(1)
	{
		autoPoll();//the program will enter the endless Polling
	}
}
