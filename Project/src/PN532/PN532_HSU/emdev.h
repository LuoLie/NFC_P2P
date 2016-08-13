/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: emdev.c
  Author: Giayee       Version : V1.0        Date:15/07/15
  Description:       
    This file provides all functions about USART2's transmit
  and receive.
***********************************************************/

#ifndef __EMDEV_H__
#define __EMDEV_H__

#include "stm32f10x.h"

#define CFG_UART_BUFSIZE	512							//Definition of USART2 for receiving buffer size

/*USART2 buffer information*/
typedef struct _uart_buffer_t
{
	volatile uint16_t len;								//length of new data in the buf
	volatile uint16_t wr_ptr;							//point to read position
	volatile uint16_t rd_ptr;							//point to read position
	uint8_t buf[CFG_UART_BUFSIZE];						//data buffer
}uart_buffer_t;

/*USART information*/
typedef struct _uart_pcb_t
{
	uint8_t status;										
	uint8_t pending_tx_data;
	uart_buffer_t rxfifo;								
}uart_pcb_t;

/*************************************************
  Function:      uartInit 
  Description:   Initialization structure variable 
  Input:         None          
  Output:        None
  Return:		 None
*************************************************/
void uartInit(void);

/*************************************************
  Function:     uartSend 
  Description:  Send data to USART2
  Input:        *buffer: Point to the data to be transmitted
				length: length of the data to be transmitted
				timeout: timeout max time to wait, 0 means no timeout
  Output:       None
  Return:		= 0     success
*************************************************/
int uartSend(const uint8_t *buffer, uint32_t length, int timeout);

/*************************************************
  Function:     uartSendByte 
  Description:  Send a byte of data to USART2
  Input:        ch: data to be transmittd
  Output:       None
  Return:		None
*************************************************/
void uartSendByte(uint8_t ch);

/*************************************************
  Function:     uartRxBufferRead 
  Description:  Read a byte of data from the buffer.
  Input:        None
  Output:       None
  Return:		Data read from buffer
*************************************************/
uint8_t uartRxBufferRead(void);

/*************************************************
  Function:     uartRxBufferReadArray 
  Description:  Read data from the buffer.
  Input:        *tx: read buffer 
		       	len: length of data need to read
				timeout: timeout max time to wait, 0 means no timeout
  Output:		None
  Return:		=0		success
				=-1		timeout
*************************************************/
int uartRxBufferReadArray(uint8_t *rx, uint32_t len, int timeout);

/*************************************************
  Function:     uartRxBufferReadArray 
  Description:  Get whether there is data in the buffer
  Input:        None
  Output:		None
  Return:		=0		no data
				=1		Existing data
*************************************************/
uint8_t uartRxBufferDataPending(void);

/*************************************************
  Function:     uartGetPCB 
  Description:  Get USART PCB pointer
  Input:        None
  Output:		None
  Return:		pointer to uart pcb
*************************************************/
uart_pcb_t *uartGetPCB(void);

/*************************************************
  Function:     uartRxBufferClearFIFO 
  Description:  clear the rx buffer information
  Input:        None
  Output:		None
  Return:		None
*************************************************/
void uartRxBufferClearFIFO(void);

/*************************************************
  Function:     uartRxBufferWrite 
  Description:  Write data to the buffer
				the function is called by USART2 RX interupt function
				when received a byte data
  Input:        data: data to be received
  Output:		None
  Return:		None
*************************************************/
void uartRxBufferWrite(uint8_t data);

#endif
