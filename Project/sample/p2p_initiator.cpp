#include "string.h"
#include "stdlib.h"
#include "bsp.h"
#include "PN532_HSU.h"
#include "snep.h"
#include "oled.h"
#include "tim.h"
PN532_HSU 	pn532_hsu_init, pn532_hsu_target;
PN532		pn532_init(pn532_hsu_init), pn532_target(pn532_hsu_target);

extern const char * stringtowrite;

int8_t initiator_data_exchange(void)
{
	uint8_t receiveBuf[128];
	uint8_t receiveLen = 200;
	int8_t res = 0;
	static uint32_t getCount = 0, setCount = 0;
	
	do
	{
//		res = pn532_target.getFirmwareVersion();
		res = pn532_init.InJumpForDEP(receiveBuf, sizeof(receiveBuf), 500);
		DMSG("InJumpForDEP res: %d\r\n", res);
	}while(res <= 0);
	
//	const char *send = "this is a message from initiator";
	char send[100];
	
	for(uint8_t i=0; i<1; i++)
	{
//		res = pn532_init.inDataExchange((uint8_t *)send, strlen(send)+1, receiveBuf, &receiveLen);
		res = pn532_init.inDataExchange((uint8_t *)send, sizeof(send), receiveBuf, &receiveLen);
			
		if(!res)
		{
			DMSG("inDataExchange error: %d\r\n", res);
			return -1;
		}

		getCount++;
		pn532_init.PrintHexChar(receiveBuf, receiveLen);
//		OLED_Clear();
//		OLED_ShowNum(0, 0, getCount, 5, 12);
//		receiveBuf[receiveLen] = '\0';
//		OLED_ShowString(0, 12, (const char *)receiveBuf, 12);
//		OLED_Refresh_Gram();
	}
	return 0;
}

int8_t target_data_exchange(void)
{                                                                                                                 
	uint8_t receiveBuf[128];
	int8_t res = 0;
	static uint32_t getCount = 0, setCount = 0;
	
	do
	{
		res = pn532_target.getFirmwareVersion();
		res = pn532_target.tgInitAsTarget(450);
		DMSG("tgInitAsTarget res: %d\r\n", res);
	}while(res <= 0);
	
//	count = 0;
	
	for(uint8_t i=0; i<1; i++)
	{
		res = pn532_target.tgGetData(receiveBuf, sizeof(receiveBuf));
	
		if(res < 0)
		{
			DMSG("tgGetData error: %d\r\n", res);
			return -1;
		}
		else
		{
			DMSG("tgGetData ok: %d\r\n", res);
			getCount++;
			pn532_target.PrintHexChar(receiveBuf, res);
						
			const char *send = stringtowrite;
			res = pn532_target.tgSetData((uint8_t *)send, strlen(send));
			
			if(!res)
			{
				DMSG("tgSetData error: %d\r\n", res);
				return -1;
			}
			setCount++;
			DMSG("tgSetData ok: %d\r\n", res);
			
			OLED_ShowNum(0, 16, getCount, 5, 16);
			OLED_ShowNum(48, 16, setCount, 5, 16);
			OLED_ShowString(0, 32, (const char *)receiveBuf, 16);
			OLED_Refresh_Gram();
		}
	}
		
	return 0;
}
