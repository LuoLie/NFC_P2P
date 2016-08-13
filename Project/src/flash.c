#include "flash.h"
#include "stm32f10x_flash.h"
#include "PN532_debug.h"
#include "oled.h"
#include "tim.h"

#define STM32_FLASH_BASE	0x08000000
#define STM_SECTOR_SIZE 	1024			//byte

/*
writeAddr: address of flash data to be writed(0x08013000 ~ 0x08020000)
pBuffer:pointer to data to be writed
len:	length of data
*/
void write_flash_data(uint32_t writeAddr, uint16_t *pBuffer, uint16_t len)
{
	uint32_t secpos;	//sector address
	uint16_t i;
	uint32_t offaddr;		//offset to 0x08000000
	
	if((writeAddr < 0x08013000) || (writeAddr >= 0x08020000))	return ;
	
	FLASH_Unlock();
	
	offaddr = writeAddr - STM32_FLASH_BASE;
	secpos = offaddr / STM_SECTOR_SIZE;
	
	FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);	//erasa page
	
	uint32_t writePos = writeAddr;
	for(i=0; i<len; i++)
	{
		FLASH_ProgramHalfWord(writePos,pBuffer[i]);
		writePos+=2;
	}
	FLASH_Lock();
}

/*
writeAddr: address of flash data to be read(0x08013000 ~ 0x08020000)
pBuffer:pointer to data buffer
len:	length of data
*/
void read_flash_data(uint32_t readAddr, uint16_t *pBuffer, uint16_t len)
{
	uint16_t i;
	
	for(i=0; i<len; i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(readAddr);
		readAddr += 2;
	}
}

uint16_t STMFLASH_ReadHalfWord(uint32_t addr)
{
	if((addr < 0x08013000) || (addr >= 0x08020000))	return 0;
	
	return *(uint16_t*)addr;
}

void flash_test()
{
	const uint8_t flash_data[] = {"cando001"};
	uint8_t buffer[8];
	
	write_flash_data(FLASH_DATA_ADDR, (uint16_t*)flash_data, sizeof(flash_data));
	read_flash_data(FLASH_DATA_ADDR, (uint16_t*)buffer, 4);
	
	for(uint8_t i=0; i<8; i++)
	{
		DMSG_CHAR(buffer[i]);
	}
	DMSG("\r\n");	
}

void writedata(void)
{
	const uint8_t flash_data[] = {"cando001"};
	
	write_flash_data(FLASH_DATA_ADDR, (uint16_t*)flash_data, sizeof(flash_data));
	
}

void readdata(void)
{
	uint8_t buffer[8], i;
	
	read_flash_data(FLASH_DATA_ADDR, (uint16_t*)buffer, 4);
	
	OLED_Clear();
	OLED_ShowString(0, 0, (const char*)buffer, 32);
	OLED_Refresh_Gram();
	delay_ms(2000);
	
	for(i=0; i<8; i++)
	{
		DMSG_CHAR(buffer[i]);
	}
	DMSG("\r\n");
}
