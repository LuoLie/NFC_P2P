/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: tim.c
  Author: Giayee       Version : V1.0        Date:15/07/15
  Description:       
    This file provides all functions about Timeout judgment.
***********************************************************/

#include "tim.h"

volatile uint32_t gTimerDrvTicks = 0;			//tick value of TIM3

uint32_t TimerDrvGetTicks()
{
	uint32_t ticks;
	ticks = gTimerDrvTicks;
	return ticks;
}

uint8_t TimerDrvGetDly(uint32_t TicksHome, uint32_t TicksDly)
{
	uint32_t ticks;
	ticks = TimerDrvGetTicks();
	if(ticks >= TicksHome)
	{
		if((ticks - TicksHome) >= TicksDly)
		{
		  return 1;
		}
		return 0;
	}
	if((0xffffffff - TicksHome + ticks + 1) >= TicksDly)
	{
		return 1;
	}
	return 0;
}

void delay_ms(uint32_t dly)
{
	uint32_t ticks1 = TimerDrvGetTicks();
    while(!TimerDrvGetDly(ticks1, dly));
}
