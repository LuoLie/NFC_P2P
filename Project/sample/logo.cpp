/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: logo.cpp
  Author: Giayee       Version : V1.1        Date:15/08/11
  Description:       
    This file provides function about how to show logo on the boot.
***********************************************************/

#include "oled.h"
#include "image.h"
#include "tim.h"

/*************************************************
  Function:     OLED_ShowLogo 
  Description:  show logo on the boot.
				user can call the function when 
				system power on.
  Input:        None          
  Output:		None
  Return:		None
*************************************************/
void OLED_ShowLogo(void)
{                        
	OLED_ShowImage(0,0,128,64,image1,1);//show LOGO
	OLED_Refresh_Gram();
	
	/*delay_ms(1000);
	OLED_Clear();                       
	OLED_Refresh_Gram();*/
}
