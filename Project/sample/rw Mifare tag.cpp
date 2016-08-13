/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: rw Mifare tag.cpp
  Author: Giayee       Version : V1.1        Date:15/08/11
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

extern uint8_t key_flag;
int currentPoints=0;
extern int ExtraPoints;
char * EP;
char * myDB[]={'\0','\0','\0','\0','\0'};

void my_utoa(int dataIn, char* bffr, int radix){
int temp_dataIn;
temp_dataIn = dataIn;
int stringLen=1;

while ((int)temp_dataIn/radix != 0){
    temp_dataIn = (int)temp_dataIn/radix;
    stringLen++;
}
//printf("stringLen = %d\n", stringLen);
temp_dataIn = dataIn;
do{
    *(bffr+stringLen-1) = (temp_dataIn%radix)+'0';
    temp_dataIn = (int) temp_dataIn / radix;
}while(stringLen--);}


/*************************************************
  Function:     readMifareTag 
  Description:  Read data from Mirafe Tag 
  Input:        None          
  Output:		None
  Return:		None
*************************************************/
int8_t readMifareTag()
{
	PN532_HSU 	pn532_hsu;
	PN532		pn532(pn532_hsu);
	
	int8_t 	success;
	uint8_t 	uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
	uint8_t 	uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
	uint8_t 	res = 0;
	
	res = pn532.begin();
	if(res == 0)							//wake-up failed
	{
		led_flicker(LED_PORT, LED_G, 200, 5);	
		return -1;
	}
	
	led_on(LED_G);
	led_off(LED_R);
  
	success = pn532.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);
	
	if (success) 
	{
		// Display some basic information about the card
		DMSG("Found an ISO14443A card\r\n");
			
		
		if (uidLength == 4)
		{
			/*if(strlen(myDB[4])!=0){
			for (int i = 0; i < 5; i++) 
				strncpy(myDB[i],"",sizeof(myDB[i]));
			}
			
			char* temp;
			for (int j = 0; j < uidLength; j++) {
				char * tempno;
				my_utoa(uid[j],tempno,10);
				strcat(temp,tempno);
			}
			for (int i = 0; i < 5; i++) {
				if (strncmp (myDB[i],temp,sizeof(const char *)) == 0){
				return 0;
				}
			}
			for (int i = 0; i < 5; i++) {
				if (strncmp (myDB[i],"",sizeof(const char *)) == 0){
					myDB[i]=temp;
					i=100;
				}
			}
			OLED_ShowString(0, 50, temp, 16);
			OLED_Refresh_Gram();
			*/
			
			//show_uid(0, 48, uid, 4);
			/*The block number to read.  (0..63 for 1KB cards, and 0..255 for 4KB cards)*/
			uint8_t		blockNumber = 2;			
			uint8_t		key[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
			// We probably have a Mifare Classic card ... 
			
			DMSG("Seems to be a Mifare Classic card (4 byte UID)\r\n");
			
			// Now we need to try to authenticate it for read/write access
			DMSG("Trying to authenticate block %d with default KEYA value\r\n", blockNumber);
			
			success = pn532.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, MIFARE_CMD_AUTH_A, key);
			
			if (success)
			{				
				DMSG("Sector %d has been authenticated\r\n", blockNumber/4);
				uint8_t data[16];
				
				success = pn532.mifareclassic_ReadDataBlock(blockNumber, data);
				
				if (success)
				{
					// Data seems to have been read ... spit it out
					DMSG("\r\nReading Block %d:", blockNumber);
					//pn532.PrintHexChar(data, 16);
					DMSG("\r\n");
					led_on(LED_R);	
					int rno=0;
					int tno=0;
					for (int Tmp = 0; Tmp < 16; Tmp++) 
					{
						if(data[Tmp]!=0)
						{
							rno+=data[Tmp];//.toInt();
							for (int t1 = 0;(t1<(16-Tmp));t1++) 
							{
								if(Tmp!=15)
									rno*=10;
							}
							tno=tno+1;
						}
						xprintf("%x ", data[Tmp]);
					}
					xprintf("\r\n");
					OLED_Clear();
					currentPoints=(currentPoints)+rno;
					ExtraPoints=currentPoints;
					OLED_ShowNum(0, 50, currentPoints,3, 16);
					OLED_Refresh_Gram();
					//my_utoa(currentPoints,EP,3);
					//ExtraPoints=EP;
					return 0;
				}
				else
				{
					DMSG("Ooops ... unable to read the requested block.  Try another key?\r\n");
				}
			}
			else
			{
				DMSG("Ooops ... authentication failed: Try another key?\r\n");
			}
		}
		
		if (uidLength == 7)
		{
			//show_uid(0, 48, uid, 7);
			uint8_t pageNumber = 4;			//The page number (0..63 in most cases)
			// We probably have a Mifare Ultralight card ...
			DMSG("Seems to be a Mifare Ultralight tag (7 byte UID)\r\n");
			
			// Try to read the first general-purpose user page (#4)
			DMSG("Reading page %d\r\n", pageNumber);
			uint8_t data[16];
			success = pn532.mifareultralight_ReadPage (pageNumber, data);
			if (success)
			{
				// Data seems to have been read ... spit it out
				pn532.PrintHexChar(data,16);
				DMSG("\r\n");
			}
			else
			{
				DMSG("Ooops ... unable to read the requested page!?\r\n");
			}
		}
	}	
	return 0;
}

/*************************************************
  Function:     writeMifareTag 
  Description:  Write data to Mirafe Tag 
  Input:        None          
  Output:		None
  Return:		None
*************************************************/
int8_t writeMifareTag()
{
	PN532_HSU 	pn532_hsu;
	PN532		pn532(pn532_hsu);
	
	int8_t 	success;
	uint8_t 	uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
	uint8_t 	uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
	uint8_t 	res = 0;
	
	res = pn532.begin();
	if(res == 0)							//wake-up failed
	{
		OLED_ShowString(0, 16, "Wakeup failed.  ", 16);
		OLED_Refresh_Gram();
		led_flicker(LED_PORT, LED_G, 500, 10);	
		return -1;
	}
	
	led_on(LED_G);
	led_off(LED_R);
	OLED_ShowString(0, 16, "Wakeup ok.      ", 16);
	OLED_ShowString(0, 32, "Writing card    ", 16);
	
	static int8_t i = -1;
	i++;
	if(i == 0)
	{
		OLED_ShowString(12*8, 32, "    ", 16);
	}
	else if(i == 1)
	{
		OLED_ShowString(12*8, 32, ".   ", 16);
	}
	else if(i == 2)
	{
		OLED_ShowString(12*8, 32, "..  ", 16);
	}
	else
	{
		OLED_ShowString(12*8, 32, "... ", 16);
		i = -1;
	}
	
	OLED_ShowString(0, 48, "                ", 16);
	OLED_Refresh_Gram();
	
	success = pn532.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100);
	
	if (success) 
	{
		// Display some basic information about the card
		DMSG("Found an ISO14443A card\r\n");
		led_on(LED_R);
		
		if (uidLength == 4)
		{
			show_uid(0, 48, uid, 4);
			
			/*The block number to write.  (1..63 for 1KB cards, and 1..255 for 4KB cards)*/
			uint8_t		blockNumber = 4;
			uint8_t		key[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
			// We probably have a Mifare Classic card ... 
			DMSG("Seems to be a Mifare Classic card (4 byte UID)\r\n");
			
			// Now we need to try to authenticate it for read/write access
			// Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
			DMSG("Trying to authenticate block %d with default KEYA value\r\n", blockNumber);			
			
			success = pn532.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, MIFARE_CMD_AUTH_A, key);
			
			if (success)
			{
				DMSG("Sector %d has been authenticated\r\n", blockNumber/4);
				uint8_t data[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3};

				success = pn532.mifareclassic_WriteDataBlock (blockNumber, data);
				
				if (success)
				{
					OLED_ShowString(0, 48, "write ok!!!", 16);
				}
				else
				{
					DMSG("Ooops ... unable to write the requested block.  Try another key?\r\n");
				}
			}
			else
			{
				DMSG("Ooops ... authentication failed: Try another key?\r\n");
			}
		}
		
		if (uidLength == 7)
		{
			show_uid(0, 48, uid, 7);
			uint8_t pageNumber = 4;			//The page number (0..63 in most cases)
			// We probably have a Mifare Ultralight card ...
			DMSG("Seems to be a Mifare Ultralight tag (7 byte UID)\r\n");
			
			//printf("Writing page %d\r\n", pageNumber);
			uint8_t data[4] = { 'a', 'd', 'a', 'f'};
			success = pn532.mifareultralight_WritePage (pageNumber, data);
			if (success)
			{
				DMSG("write page %d ok\r\n", pageNumber);
				
			}
			else
			{
				DMSG("Ooops ... unable to read the requested page!?\r\n");
			}
		}
	}	
	return 0;
}
