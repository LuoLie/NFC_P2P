/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: p2p.cpp
  Author: Giayee       Version : V1.1        Date:15/08/11
  Description:       
    This file provides function about reveice and transmit
  data between Initiator and target.
***********************************************************/

#include "stdio.h"
#include "string.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "bsp.h"
#include "PN532_HSU.h"
#include "snep.h"
#include "oled.h"
#include "tim.h"
PN532_HSU 	pn532_hsu;
PN532		pn532(pn532_hsu);
SNEP		nfc(pn532_hsu);
uint8_t 	message[250]= {'a', 'b'};			//message buffer of receive data or send data
uint8_t 	wmessage[250]= {'a', 'b'};			//message buffer of receive data or send data
extern uint8_t show_surface_flag;
extern uint8_t NFCstate;
const char * tempString;
extern uint8_t startonly;
extern uint8_t watchstate;
extern const char * deviceID;
extern const char * stringtowrite;
extern int showView;
extern int candoPoints;
extern int targetPoints;
extern int candoTime;
extern int awardedPoints;
extern int s,min,h;
extern int Gsec,Gmin;
extern int barvalue;
int failedcout=0;
int targetP;
int CP=0;
const char *ssID="dsfs524-545dsf-65152sdf-65465143";

extern uint8_t oled_mutex;

char * updateSessiondata(){
	char Tsd [100];
	return strncpy(Tsd,ssID,sizeof(Tsd));
}
void updatewritedata()
{	
	CP=(candoPoints);
	if(CP>10){
		if((CP%10)==0)
			targetP=(int)(CP/10)*10;
		else
			targetP=(int)((CP/10)+1)*10;
	}else{
		targetP=10;
	}
	if(barvalue!=10)
		barvalue= 10-(targetP-CP);
	else
		barvalue= (targetP-CP);
	if(strlen(ssID)>0){
		startonly=0;
	}
	h=(candoTime)/60;
	min=(candoTime)%60;
	OLED_Clear();
	NFCstate=0;
}
/*************************************************
  Function:     p2p_target_receive 
  Description:  Target receive data from Initiator 
  Input:        None          
  Output:		None
  Return:		None
*************************************************/
int p2p_target_receive()
{	
	int8_t length=0;
//	const char* stringdata="";
	led_on(LED_B);
	led_off(LED_R);
//	close_tim2_IT();
	DMSG("---------------------\r\n");
	DMSG("Read data from tablet\r\n");
	static uint32_t last_time, time;
	last_time = TimerDrvGetTicks();
	length = nfc.read(wmessage, sizeof(wmessage),600);
//	open_tim2_IT();
//	OLED_Clear();
	if(length > 0)
	{
		time = TimerDrvGetTicks() - last_time;
		led_on(LED_R);
		pn532.PrintHexChar(wmessage, length);
		xprintf("MCU Receive data from tablet ok: %d.%d\r\n", (time/1000), (time%1000));
		NFCstate=0;
	}
	else
	{
		xprintf("MCU Receive data from tablet! res = %d\r\n", length);
		NFCstate=1;
		failedcout=failedcout+1;
//		if(failedcout>3)//手表写数据完成后，收数据不成功次数大于3次，则开始写数据操作
//		{
//			led_off(LED_G);
//			NFCstate=0;
//			failedcout=0;
//			DMSG("\r\nRead failed!!!\r\n");
//		}
	}
	led_off(LED_B);
	return length;
	#if 0
	if(length > 0)
	{
		motor_on(300);
		char tempstr [500];
		led_on(LED_R);
//		stringdata="";
	
		for(uint8_t i=0; i<length; i++)
		{
			char c = (char)wmessage[i];
			if (c <= 0x1f || c > 0x7f) 
			{
				DMSG(".");
				tempstr[i]= '.';
			} 
			else 
			{
				DMSG("%c", c);
				tempstr[i]= (c);
			}
		}
		DMSG("\r\nRead success...!!!\r\n");
		for(int t=0;t<(strlen(tempstr)-1);t++)
		{
			if(tempstr[t]=='.')
				memmove(tempstr+t, tempstr+t+1, strlen(tempstr) - t);
		}
//		stringdata=tempstr;
		if(oled_mutex == 0)
		{
			oled_mutex = 1;
			OLED_Clear();
			OLED_ShowString(0,0,stringtowrite,12);
			OLED_Refresh_Gram();
			oled_mutex = 0;
		}
		char *pch;
		char * strings[10]={};
		uint8_t loop=0;
		//Get data from nfc tag
		pch = strtok (tempstr, " &,#:");
		while (pch != NULL)
		{
			strings[loop]=pch;
			pch = strtok (NULL, " &,#:");
			loop=loop+1;
		}
		if(strings[4][strlen(strings[4])-1]=='.')
			strings[4][strlen(strings[4])-1]='\0';
		if (strncmp (deviceID,strings[1],strlen(strings[1])) != 0)
			return;
		if (strlen(ssID) == 0)
		{
			tempString=strings[2];
			if (strlen(tempString) == 0){
				return;
			}
			if(strlen(strings[3])>0)
				candoPoints=atoi(strings[3]);
			if(strlen(strings[4])>0)
				candoTime=atoi(strings[4]);
			if(strlen(strings[2])>0)
				ssID=strings[2];
			// updatesession(ssID);
		}
		else
		{
			if (strncmp (deviceID,strings[1],strlen(strings[1])) == 0){
				tempString=strings[0];
				if (strncmp (tempString,"0",sizeof("0")) != 0){
					if(strlen(strings[0])>0)
						awardedPoints=atoi(strings[0]);
					if(strlen(strings[4])>0)
						targetPoints=atoi(strings[4]);
					// updatesession(ssID);
					//startgame
					Gmin=1;
					Gsec=59;
					/* showView=5;
					OLED_Clear();
					
					for(int t=2;t<5;t++)
					OLED_ShowString(0,(t-2)*16,strings[t],16);
					OLED_Refresh_Gram();*/
					watchstate=2;
					showView=1;
				}else if (atoi(tempString)==0){
					candoPoints=atoi(strings[3]);
					candoTime=atoi(strings[4]);
					tempString=strings[2];
					if (strlen(tempString) == 0){
						ssID="";
					}else{
						ssID=strings[2];
					}
					//updatesession(ssID);
				}
			}
		}
		//updatewritedata
		updatewritedata();
		if (strlen(ssID) == 0)
		{
			show_surface_flag=0;
		}
		else
		{
			OLED_Clear();
			show_surface_flag=1;
		}
		NFCstate=0;
	}	
	else
	{
		NFCstate=1;
		failedcout=failedcout+1;
//		if(failedcout>3)//手表写数据完成后，收数据不成功次数大于3次，则开始写数据操作
//		{
//			led_off(LED_G);
//			NFCstate=0;
//			failedcout=0;
//			DMSG("\r\nRead failed!!!\r\n");
//		}
	}
	#endif
}
/*************************************************
  Function:     p2p_target_send 
  Description:  Target send data to Initiator 
  Input:        None          
  Output:		None
  Return:		None
*************************************************/
int p2p_target_send()
{	
	int8_t res;
	const char * message;
	led_on(LED_G);
	led_off(LED_R);
	message	= stringtowrite;
//	close_tim2_IT();
	DMSG("---------------------\r\n");
	DMSG("Write data to tablet\r\n");
//	pn532.RFConfig(0x00);
	static uint32_t last_time, time;
	last_time = TimerDrvGetTicks();
	res = nfc.write((uint8_t*)message, strlen(message), 600);
//	open_tim2_IT();
	 /* OLED_Clear();
	  OLED_ShowString(0,0,"sending",12);
		OLED_Refresh_Gram();*/
	if(res == 1)
	{
		time = TimerDrvGetTicks() - last_time;
		led_on(LED_R);
		#if 0
		if(oled_mutex == 0)
		{
			oled_mutex = 1;
			OLED_Clear();
			OLED_ShowString(0,16,stringtowrite,12);
			OLED_Refresh_Gram();
			oled_mutex = 0;
		}
		#endif
		
		xprintf("MCU Send data to tablet ok: %d.%d\r\n", (time/1000), (time%1000));
		DMSG("\r\nWrite success!!!\r\n");
		NFCstate=1;
	}
	else
	{
		/*OLED_Clear();
		OLED_ShowString(0,0,"failed",12);
		OLED_Refresh_Gram();*/
		NFCstate=0;
		DMSG("Write failed!!!\r\n\r\n");
		xprintf("MCU Send data to tablet failed! res = %d\r\n", res);
		
	}
	led_off(LED_G);
	return res;
}
