/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: tim.h
  Author: Giayee       Version : V1.1        Date:15/08/11
  Description:       
    This file provides all functions about Timeout judgment.
***********************************************************/

#ifndef __TIM_H__
#define __TIM_H__
#include "stm32f10x.h"

extern volatile uint32_t gTimerDrvTicks;

/*************************************************
  Function:     TimerDrvGetTicks 
  Description:  get TIM3 tick value 
  Input:        None          
  Output:		None
  Return:		ticks value
*************************************************/
uint32_t 	TimerDrvGetTicks(void);

/*************************************************
  Function:     TimerDrvGetDly 
  Description:  Timeout judgment
  Input:        TicksHome: start value of ticks
				TicksDly: delay time  Uint:ms
  Output:		None
  Return:		=0, No timeout
				=1, timeout
*************************************************/
uint8_t 	TimerDrvGetDly(uint32_t TicksHome, uint32_t TicksDly);

/*************************************************
  Function:     delay_ms 
  Description:  delay time
  Input:        dly: time to delay(Uint: ms)
  Output:		None
  Return:		None
*************************************************/
void delay_ms(uint32_t dly);
#endif
