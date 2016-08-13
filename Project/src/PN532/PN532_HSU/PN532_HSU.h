
#ifndef __PN532_HSU_H__
#define __PN532_HSU_H__

#include "PN532Interface.h"
#include "PN532_debug.h"

#define PN532_HSU_DEBUG

#define PN532_HSU_READ_TIMEOUT						(1000)

class PN532_HSU : public PN532Interface {
public:
    PN532_HSU();
	
    /**************************************************************************/
	/*!
	@brief  USART INIT
	*/
	/**************************************************************************/
    virtual void begin();
	
	/**************************************************************************/
	/*!
	@brief  wake up PN532 from power down mode
	*/
	/**************************************************************************/
    virtual uint8_t wakeup();
	
	/**
    * @brief    Write command to PN532
	* @param    *header: header of command
	* @param	hlen: length of header
	* @param	*body: body of data
	* @param	blen: length of body part
    * @return   TRUE     Write command success
    *           FALSE    Write command  failed
    */
    virtual int8_t writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body = 0, uint8_t blen = 0);
    
	/**
    * @brief    Read response from PN532
	* @param    buf[]: buffer of response
	* @param	len: length of response
	* @param	timeout: timeout max time to wait, 0 means no timeout
    * @return   >0    	length of response data
    *           <0    	false
    */
	virtual int16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout);
    
private:
    
    uint8_t command;
    
    int8_t readAckFrame();
    
    int8_t receive(uint8_t *buf, int len, uint16_t timeout=PN532_HSU_READ_TIMEOUT);
};
uint8_t nfc_WakeUp(void);
uint8_t pn532_pwr_down(void);

#endif
