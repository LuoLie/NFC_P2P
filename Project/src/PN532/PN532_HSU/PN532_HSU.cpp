
#include "PN532_HSU.h"
#include "emdev.h"
#include "string.h"
#include "stm32f10x.h"

#include "tim.h"
#include "bsp.h"

extern uart_pcb_t pcb;

PN532_HSU::PN532_HSU()
{
    
}

void PN532_HSU::begin()
{
    uartInit();
}

/**************************************************************************/
/*!
    @brief  power down the PN532
*/
/**************************************************************************/
uint8_t pn532_pwr_down(void)
{
	uint8_t buffer[16];
	uint8_t szRx = 16;
	
    buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0xff;
	
	buffer[3] = 0x03;
	buffer[4] = 0xfd;
	
	buffer[5] = 0xd4;
	buffer[6] = 0x16;
    buffer[7] = 0xf0; // normal mode;
	
	uint8_t sum = buffer[5] + buffer[6] + buffer[7];
	
	buffer[8] = ~sum + 1;
	buffer[9] = 0x00;

    DMSG("POWER DOWN\r\n");

    uartSend(buffer, 10, 0);

    if(uartRxBufferReadArray(buffer, szRx, 100) < 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t nfc_WakeUp(void)
{
	u8 data[24];
	
	data[0]=0x55;
	data[1]=0x55;
	data[2]=0x00;
	data[3]=0x00;
	data[4]=0x00;
	data[5]=0x00;
	data[6]=0x00;
	data[7]=0x00;
	data[8]=0x00;
	data[9]=0x00;
	data[10]=0x00;
	data[11]=0x00;
	data[12]=0x00;
	data[13]=0x00;
	data[14]=0x00;
	data[15]=0x00;
	
	data[16]=0xFF;
	data[17]=0x03; 
	data[18]=0xFD; 
	
	data[19]=0xD4; 
	data[20]=0x14; 
	data[21]=0x01;
	
	data[22]=0x17; 
	data[23]=0x00;
	
	uartSend(data, 24, 0);
	
	uint8_t abtRx[15];
	uint8_t szRx = 15;
	
//	uartRxBufferReadArray(abtRx, szRx, 1000);
	
	if(uartRxBufferReadArray(abtRx, szRx, 1000) < 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t PN532_HSU::wakeup()
{
    return nfc_WakeUp();
}

int8_t PN532_HSU::writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{	
    command = header[0];
    
    uartSendByte(PN532_PREAMBLE);
    uartSendByte(PN532_STARTCODE1);
    uartSendByte(PN532_STARTCODE2);
    
    uint8_t length = hlen + blen + 1;   // length of data field: TFI + DATA
    uartSendByte(length);
    uartSendByte(~length + 1);         // checksum of length
    
    uartSendByte(PN532_HOSTTOPN532);
    uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA

    DMSG("\r\nWrite: ");
    
    uartSend(header, hlen, 0);
    for (uint8_t i = 0; i < hlen; i++) {
        sum += header[i];

        DMSG_HEX(header[i]);
    }

    uartSend(body, blen, 0);
    for (uint8_t i = 0; i < blen; i++) {
        sum += body[i];

        DMSG_HEX(body[i]);
    }
    
    uint8_t checksum = ~sum + 1;            // checksum of TFI + DATA
    uartSendByte(checksum);
    uartSendByte(PN532_POSTAMBLE);

    return readAckFrame();
}

int16_t PN532_HSU::readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint8_t tmp[3];
    
    DMSG("\nRead:  ");
    
    /** Frame Preamble and Start Code */
    if(receive(tmp, 3, timeout)<=0){
        return PN532_TIMEOUT;
    }
    if(0 != tmp[0] || 0!= tmp[1] || 0xFF != tmp[2]){
        DMSG("Preamble error");
        return PN532_INVALID_FRAME;
    }
    
    /** receive length and check */
    uint8_t length[2];
    if(receive(length, 2, timeout) <= 0){
        return PN532_TIMEOUT;
    }
    if( 0 != (uint8_t)(length[0] + length[1]) ){
        DMSG("Length error");
        return PN532_INVALID_FRAME;
    }
    length[0] -= 2;
    if( length[0] > len){
        return PN532_NO_SPACE;
    }
    
    /** receive command byte */
    uint8_t cmd = command + 1;               // response command
    if(receive(tmp, 2, timeout) <= 0){
        return PN532_TIMEOUT;
    }
    if( PN532_PN532TOHOST != tmp[0] || cmd != tmp[1]){
        DMSG("Command error");
        return PN532_INVALID_FRAME;
    }
    
    if(receive(buf, length[0], timeout) != length[0]){
        return PN532_TIMEOUT;
    }
    uint8_t sum = PN532_PN532TOHOST + cmd;
    for(uint8_t i=0; i<length[0]; i++){
        sum += buf[i];
    }
    
    /** checksum and postamble */
    if(receive(tmp, 2, timeout) <= 0){
        return PN532_TIMEOUT;
    }
    if( 0 != (uint8_t)(sum + tmp[0]) || 0 != tmp[1] ){
        DMSG("Checksum error");
		uartRxBufferClearFIFO();
        return PN532_INVALID_FRAME;
    }
//	DMSG_INT(length[0]);
	DMSG("  %d\r\n", length[0]);
    
    return length[0];
}

int8_t PN532_HSU::readAckFrame()
{
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
    uint8_t ackBuf[sizeof(PN532_ACK)];
    
    DMSG("\r\nAck: ");
    
    if( receive(ackBuf, sizeof(PN532_ACK), PN532_ACK_WAIT_TIME) <= 0 ){
        DMSG("Timeout\n");
        return PN532_TIMEOUT;
    }
    
    if( memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK)) ){
        DMSG("Invalid\r\n");
		uartRxBufferClearFIFO();
        return PN532_INVALID_ACK;
    }
    return 0;
}

/**
    @brief receive data .
    @param buf --> return value buffer.
           len --> length expect to receive.
           timeout --> time of reveiving
    @retval number of received bytes, 0 means no data received.
*/
int8_t PN532_HSU::receive(uint8_t *buf, int len, uint16_t timeout)
{
	int read_bytes = 0;
	uint32_t ticks = TimerDrvGetTicks();
	uint8_t *ptr = buf;
	
	while (len) 
	{
		if(pcb.rxfifo.len > 0)
		{
			len--;
			(*ptr++) = uartRxBufferRead();
			read_bytes++;
			
		}
		if(TimerDrvGetDly(ticks, timeout))
		{
			DMSG("receive drv timeout\r\n");
			return -1;
		}		
	}
	return read_bytes;
}
