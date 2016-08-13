/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: emdev.c
  Author: Giayee       Version : V1.0        Date:15/07/15
  Description:       
    This file provides all functions about USART2's transmit
  and receive.
***********************************************************/

#include "stm32f10x.h"
#include "string.h"
#include "tim.h"
#include "emdev.h"
#include "stdio.h"
#include "PN532_debug.h"

uart_pcb_t pcb;

void uartInit(void)
{
	memset(&pcb, 0, sizeof(uart_pcb_t));
	pcb.pending_tx_data = 0;
	pcb.rxfifo.len = 0;
}

int uartSend(const uint8_t *buffer, uint32_t length, int timeout)
{
	const uint8_t *tmp = buffer;
	
	while(length != 0)
	{
		USART_SendData(USART2, *tmp);
		tmp++;
		length--;
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
		{}
	}
	return 0;
}

void uartSendByte(uint8_t ch)
{
	USART_SendData(USART2, (uint8_t) ch);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	{}
		
	return;
}

uint8_t uartRxBufferRead(void)
{
	uint8_t data;
	
	data = pcb.rxfifo.buf[pcb.rxfifo.rd_ptr];
	pcb.rxfifo.rd_ptr = (pcb.rxfifo.rd_ptr + 1) % CFG_UART_BUFSIZE;
	pcb.rxfifo.len--;  
	
	DMSG_HEX(data);
	
	return data;
}

int uartRxBufferReadArray(uint8_t *rx, uint32_t len, int timeout)
{
	uint32_t ticks = TimerDrvGetTicks();
	uint8_t *ptr = rx;
	
	while(len)
	{
		if(pcb.rxfifo.len > 0)
		{
			len--;
			(*ptr++) = uartRxBufferRead();
		}
		if(TimerDrvGetDly(ticks, timeout))
		{
			DMSG("receive drv timeout\r\n");
			return -1;
		}
	}
	return 0;
}

void uartRxBufferWrite(uint8_t data)
{
	pcb.rxfifo.buf[pcb.rxfifo.wr_ptr] = data;
	pcb.rxfifo.wr_ptr = (pcb.rxfifo.wr_ptr + 1) % CFG_UART_BUFSIZE;
	pcb.rxfifo.len++;
}

void uartRxBufferClearFIFO()
{
	pcb.rxfifo.rd_ptr = 0;
	pcb.rxfifo.wr_ptr = 0;
	pcb.rxfifo.len = 0;
}

uint8_t uartRxBufferDataPending(void)
{
	if(pcb.rxfifo.len != 0)
	{
		return 1;
	}
	return 0;
}

uart_pcb_t *uartGetPCB(void)
{
    return &pcb;
}
