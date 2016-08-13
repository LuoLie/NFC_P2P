#ifndef __FLASH_H__
#define __FLASH_H__
#include "stm32f10x.h"

#define FLASH_DATA_ADDR		0x08013000

void write_flash_data(uint32_t writeAddr, uint16_t *pBuffer, uint16_t len);
void read_flash_data(uint32_t readAddr, uint16_t *pBuffer, uint16_t len);
uint16_t STMFLASH_ReadHalfWord(uint32_t addr);
void flash_test(void);
void writedata(void);
void readdata(void);

#endif
