/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: battery.h
  Author: Giayee       Version : V1.1        Date:15/08/11
  Description:       
    This file provides all battery information functions.
  User can call function of get_bat_value() to get battery
  information.
***********************************************************/

#ifndef __BATTERY_H__
#define __BATTERY_H__

#define ADC_BUFFER_SIZE	10							//buffer size for ADC results	

#define MIN 			3300    					//Voltage value when the battery is exhausted
#define MAX 			4100    					//Voltage value when the battery is full
#define REF 			3300    					//Reference voltage of ADC
#define UP  			1       					//Pull-up resistor
#define DOWN  			2       					//Pull-down resistor

/*battery information description*/
typedef struct
{
	uint16_t	ad_value;							//Average value of ADC conversion
	uint16_t	mv_value;							//Voltage value of battery  Uint:mv
	uint8_t		percentage;							//percentage of battery		Range 0~100
}bat_value;

/*************************************************
  Function:     get_bat_value 
  Description:  get battery information 
  Input:        None          
  Output:		None
  Return:		Variable of bat_value structure
*************************************************/
bat_value get_bat_value(void);

/*************************************************
  Function:     get_charge_status 
  Description:  get battery charge status 
  Input:        None          
  Output:		None
  Return:		1: On charging
				0: Not Charge
*************************************************/
uint8_t get_charge_status(void);

#endif
