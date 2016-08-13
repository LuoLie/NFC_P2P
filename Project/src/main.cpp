/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: logo.cpp
  Author: Giayee       Version : V1.1        Date:15/08/11
  Description:       
    main function.
***********************************************************/

#include <stdio.h>
#include "bsp.h"
#include <string>
#include <stdlib.h>
#include <string.h>
#include "tim.h"
#include "oled.h"

#include "battery.h"
#include "PN532_HSU.h"
#include "snep.h"
#include "emdev.h"
#include "flash.h"

extern int Gsec,Gmin;
extern int showView;
extern int p2p_target_receive(void);
extern char * updateSessiondata(void);
extern void updatewritedata(void);
extern int p2p_target_send(void);
extern int8_t readMifareTag(void);
extern int8_t writeMifareTag(void);
extern void OLED_showCandopage(void);
extern int8_t initiator_data_exchange(void);
extern int8_t target_data_exchange(void);
uint8_t key_flag = 0;					//number of press key
uint8_t sleep_flag = 0;					//MCU enter sleep status flag
uint8_t show_surface_flag = 0;			//show surface flag
int candoPoints=0;
int candoTime=0;
int targetPoints=0;
int ExtraPoints=0;
int awardedPoints=0;
const char * deviceID="cando001";
char * SessionID="";
char zero[]="0";
const char * stringtowrite;
uint8_t watchstate=0;
uint8_t NFCstate=0;
unsigned int timer;

uint8_t oled_mutex = 0;			//oled show mutex condition

void mcu_enter_sleep()
{	
	OLED_Clear();
	OLED_Refresh_Gram();
	led_off(CHG_LED | LED_B | LED_R | LED_G);
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	__WFI();
}

void mcu_exit_sleep()
{	
	led_off(CHG_LED);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

uint8_t startonly=0;
int8_t nextempty(char temps[]){
	int8_t length;
	length=100;
	for (uint8_t i = 0; i < length; i++) 
	{
		if(temps[i]=='\0'){
			return i;
		}
	}
	return 100;
}

char tempstr [200];
char *itoa(int value, char *s, int radix)
{
    const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    unsigned long ulvalue = value;
    char *p = s, *q = s;
    char temp;
    if (radix == 10 && value < 0) {
        *p++ = '-';
        q = p;
        ulvalue = -value;
    }
    do {
        *p++ = digits[ulvalue % radix];
        ulvalue /= radix;
    } while (ulvalue > 0);
    *p-- = '\0';
    while (q < p) {
        temp = *q;
        *q++ = *p;
        *p-- = temp;
    }
    return s;
}

int main(void)
{
	BSP_Init();
	
	watchstate=1;

	led_off(CHG_LED);
	
	OLED_Clear();
	OLED_ShowString(0, 0, (const char *)"Initiator:", 16);
	OLED_Refresh_Gram();
	
	PN532_HSU 	pn532_hsu_init;
	PN532		pn532_init(pn532_hsu_init);
	int res = pn532_init.begin();
	if(res == 0)							//wake-up failed
	{
		led_flicker(LED_PORT, LED_G, 200, 5);	
		DMSG("PN532 begin error: %d\r\n", res);
		return -1;
	}
	
	while(1)
	{
		
		while(1)
		{
//			target_data_exchange();
			initiator_data_exchange();
		}
		#if 0
		while(1)
		{
			uint32_t total_second, second;
			total_second = TimerDrvGetTicks();
			
			p2p_target_send();
			
			second = TimerDrvGetTicks() - total_second;
			xprintf("%d.%d\r\n", second/1000, second%1000);
//			led_off(LED_G);
		}
		#endif
		#if 0
		while(1)
		{
			uint32_t total_second, second;
			total_second = TimerDrvGetTicks();
			
			p2p_target_receive();
			
			second = TimerDrvGetTicks() - total_second;
			xprintf("%d.%d\r\n", second/1000, second%1000);
		}
		#endif
	}
	
}

