/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: surface.cpp
  Author: Giayee       Version : V1.1        Date:15/08/11
  Description:
    This file provides function about display on the OLED
***********************************************************/

#include "oled.h"
#include "battery.h"
#include <stdlib.h>
extern int s,min,h;
extern int Gsec,Gmin;
extern uint8_t watchstate;
extern int candoPoints;
extern uint8_t oled_mutex, NFCstate;
int barvalue=0;
int showView=0;
/*************************************************
  Function:     OLED_ShowSurface
  Description:  display surface on the OLED.
				user can call the function once a minute
  Input:        None
  Output:		None
  Return:		None
*************************************************/
void OLED_ShowSurface(void)
{
	if((oled_mutex == 0) && (NFCstate != 1))
	{
		oled_mutex = 1;
		if(showView==0){
			//bat_value bat;
		//	bat = get_bat_value();                   	//get battery informaiton bat.percentage
		
			//OLED_ShowPower(barvalue);
		//	OLED_ShowNum(16, 50, barvalue, 3, 16);  
			for(int i=0;i<10;i++){
				if(i<barvalue){
					OLED_ShowRect(0,60-(6*i),10,3,1,0);
				}else{
					OLED_ShowRect(0,60-(6*i),10,3,0,0);
				}
			}
			OLED_ShowNum(16, 0,candoPoints,3, 32);      	//
			OLED_ShowString(44, 3, "CAN", 16);      	//
			OLED_ShowNum(16, 32, h, 2, 16);         	//showtime
			OLED_ShowString(40, 32, "HR", 16);      	// HR=hour
			OLED_ShowNum(64, 32, min, 2, 16);
			OLED_ShowString(88, 32, "MIN", 16);     	//MIN=minute
			OLED_Refresh_Gram();
		}
		else if(showView==1){
			OLED_ShowNum(16, 32, Gmin, 2, 16);         	//showtime
			OLED_ShowString(40, 32, "MIN", 16);       //MIN=minute
			OLED_ShowNum(64, 32, Gsec, 2, 16);
			OLED_ShowString(88, 32, "SEC", 16);     	//SEC=second
			if(Gmin<=0&&Gsec<=1){//Timeover
					//Gsmeover
				watchstate=3;
				showView=3;
			}
			OLED_Refresh_Gram();
		}
		oled_mutex = 0;
	}
}
void OLED_showCandopage(){
	OLED_ShowSurface();
}
