/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: battery.c
  Author: Giayee       Version : V1.0        Date:15/07/15
  Description:       
    This file provides all battery information functions.
***********************************************************/

#include "stdint.h"
#include "battery.h"

#define CHARGE_VOLTAGE	200

uint16_t adcbuffer[ADC_BUFFER_SIZE];
uint8_t charge_status = 0;

bat_value get_bat_value(void)
{
	uint8_t 	i;
	uint32_t 	ad_average = 0;
	uint16_t	vol;
	uint8_t		per;
	bat_value	bat;
	
	for (i=0; i<ADC_BUFFER_SIZE; i++)
	{
		ad_average += adcbuffer[i];
	}
	
	ad_average /= ADC_BUFFER_SIZE;
	vol = ad_average * (UP + DOWN) * REF / 4096 / DOWN;      	//voltage:mv
	per = (vol - MIN) * 100 / (MAX - MIN);			            //Range: 0~100
	if(1 == get_charge_status())
	{
		per = (vol - CHARGE_VOLTAGE - MIN) * 100 / (MAX - MIN);
	}
	
	bat.ad_value = ad_average;
	bat.mv_value = vol;
	bat.percentage = per;
	
	return bat;
}

uint8_t get_charge_status(void)
{
	return charge_status;
}
