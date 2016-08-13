/**************************************************************************/
/*!
    @file     PN532.h
    @author   Adafruit Industries & Seeed Studio
    @license  BSD
*/
/**************************************************************************/

#ifndef __PN532_H__
#define __PN532_H__

#include <stdint.h>
#include "PN532Interface.h"

// PN532 Commands
#define PN532_COMMAND_DIAGNOSE              (0x00)
#define PN532_COMMAND_GETFIRMWAREVERSION    (0x02)
#define PN532_COMMAND_GETGENERALSTATUS      (0x04)
#define PN532_COMMAND_READREGISTER          (0x06)
#define PN532_COMMAND_WRITEREGISTER         (0x08)
#define PN532_COMMAND_READGPIO              (0x0C)
#define PN532_COMMAND_WRITEGPIO             (0x0E)
#define PN532_COMMAND_SETSERIALBAUDRATE     (0x10)
#define PN532_COMMAND_SETPARAMETERS         (0x12)
#define PN532_COMMAND_SAMCONFIGURATION      (0x14)
#define PN532_COMMAND_POWERDOWN             (0x16)
#define PN532_COMMAND_RFCONFIGURATION       (0x32)
#define PN532_COMMAND_RFREGULATIONTEST      (0x58)
#define PN532_COMMAND_INJUMPFORDEP          (0x56)
#define PN532_COMMAND_INJUMPFORPSL          (0x46)
#define PN532_COMMAND_INLISTPASSIVETARGET   (0x4A)
#define PN532_COMMAND_INATR                 (0x50)
#define PN532_COMMAND_INPSL                 (0x4E)
#define PN532_COMMAND_INDATAEXCHANGE        (0x40)
#define PN532_COMMAND_INCOMMUNICATETHRU     (0x42)
#define PN532_COMMAND_INDESELECT            (0x44)
#define PN532_COMMAND_INRELEASE             (0x52)
#define PN532_COMMAND_INSELECT              (0x54)
#define PN532_COMMAND_INAUTOPOLL            (0x60)
#define PN532_COMMAND_TGINITASTARGET        (0x8C)
#define PN532_COMMAND_TGSETGENERALBYTES     (0x92)
#define PN532_COMMAND_TGGETDATA             (0x86)
#define PN532_COMMAND_TGSETDATA             (0x8E)
#define PN532_COMMAND_TGSETMETADATA         (0x94)
#define PN532_COMMAND_TGGETINITIATORCOMMAND (0x88)
#define PN532_COMMAND_TGRESPONSETOINITIATOR (0x90)
#define PN532_COMMAND_TGGETTARGETSTATUS     (0x8A)

#define PN532_RESPONSE_INDATAEXCHANGE       (0x41)
#define PN532_RESPONSE_INLISTPASSIVETARGET  (0x4B)


#define PN532_MIFARE_ISO14443A              (0x00)

// Mifare Commands
#define MIFARE_CMD_AUTH_A                   (0x60)
#define MIFARE_CMD_AUTH_B                   (0x61)
#define MIFARE_CMD_READ                     (0x30)
#define MIFARE_CMD_WRITE                    (0xA0)
#define MIFARE_CMD_WRITE_ULTRALIGHT         (0xA2)
#define MIFARE_CMD_TRANSFER                 (0xB0)
#define MIFARE_CMD_DECREMENT                (0xC0)
#define MIFARE_CMD_INCREMENT                (0xC1)
#define MIFARE_CMD_STORE                    (0xC2)

// Prefixes for NDEF Records (to identify record type)
#define NDEF_URIPREFIX_NONE                 (0x00)
#define NDEF_URIPREFIX_HTTP_WWWDOT          (0x01)
#define NDEF_URIPREFIX_HTTPS_WWWDOT         (0x02)
#define NDEF_URIPREFIX_HTTP                 (0x03)
#define NDEF_URIPREFIX_HTTPS                (0x04)
#define NDEF_URIPREFIX_TEL                  (0x05)
#define NDEF_URIPREFIX_MAILTO               (0x06)
#define NDEF_URIPREFIX_FTP_ANONAT           (0x07)
#define NDEF_URIPREFIX_FTP_FTPDOT           (0x08)
#define NDEF_URIPREFIX_FTPS                 (0x09)
#define NDEF_URIPREFIX_SFTP                 (0x0A)
#define NDEF_URIPREFIX_SMB                  (0x0B)
#define NDEF_URIPREFIX_NFS                  (0x0C)
#define NDEF_URIPREFIX_FTP                  (0x0D)
#define NDEF_URIPREFIX_DAV                  (0x0E)
#define NDEF_URIPREFIX_NEWS                 (0x0F)
#define NDEF_URIPREFIX_TELNET               (0x10)
#define NDEF_URIPREFIX_IMAP                 (0x11)
#define NDEF_URIPREFIX_RTSP                 (0x12)
#define NDEF_URIPREFIX_URN                  (0x13)
#define NDEF_URIPREFIX_POP                  (0x14)
#define NDEF_URIPREFIX_SIP                  (0x15)
#define NDEF_URIPREFIX_SIPS                 (0x16)
#define NDEF_URIPREFIX_TFTP                 (0x17)
#define NDEF_URIPREFIX_BTSPP                (0x18)
#define NDEF_URIPREFIX_BTL2CAP              (0x19)
#define NDEF_URIPREFIX_BTGOEP               (0x1A)
#define NDEF_URIPREFIX_TCPOBEX              (0x1B)
#define NDEF_URIPREFIX_IRDAOBEX             (0x1C)
#define NDEF_URIPREFIX_FILE                 (0x1D)
#define NDEF_URIPREFIX_URN_EPC_ID           (0x1E)
#define NDEF_URIPREFIX_URN_EPC_TAG          (0x1F)
#define NDEF_URIPREFIX_URN_EPC_PAT          (0x20)
#define NDEF_URIPREFIX_URN_EPC_RAW          (0x21)
#define NDEF_URIPREFIX_URN_EPC              (0x22)
#define NDEF_URIPREFIX_URN_NFC              (0x23)

#define PN532_GPIO_VALIDATIONBIT            (0x80)
#define PN532_GPIO_P30                      (0)
#define PN532_GPIO_P31                      (1)
#define PN532_GPIO_P32                      (2)
#define PN532_GPIO_P33                      (3)
#define PN532_GPIO_P34                      (4)
#define PN532_GPIO_P35                      (5)

class PN532
{
public:
    PN532(PN532Interface &interface);

	/**************************************************************************/
	/*!
	@brief  Setups the HW
	*/
	/**************************************************************************/
    uint8_t begin(void);

    // Generic PN532 functions	
	/**************************************************************************/
	/*!
	@brief  Configures the SAM (Secure Access Module)
	*/
	/**************************************************************************/
    bool SAMConfig(void);
	
	/**************************************************************************/
	/*!
	@brief  Checks the firmware version of the PN5xx chip
	
	@returns  The chip's firmware version and ID
	*/
	/**************************************************************************/
    uint32_t getFirmwareVersion(void);
	
	/**************************************************************************/
	/*!
	Writes an 8-bit value that sets the state of the PN532's GPIO pins
	
	@warning This function is provided exclusively for board testing and
	is dangerous since it will throw an error if any pin other
	than the ones marked "Can be used as GPIO" are modified!  All
	pins that can not be used as GPIO should ALWAYS be left high
	(value = 1) or the system will become unstable and a HW reset
	will be required to recover the PN532.
	
	pinState[0]  = P30     Can be used as GPIO
	pinState[1]  = P31     Can be used as GPIO
	pinState[2]  = P32     *** RESERVED (Must be 1!) ***
	pinState[3]  = P33     Can be used as GPIO
	pinState[4]  = P34     *** RESERVED (Must be 1!) ***
	pinState[5]  = P35     Can be used as GPIO
	
	@returns 1 if everything executed properly, 0 for an error
	*/
	/**************************************************************************/
    bool writeGPIO(uint8_t pinstate);
	
	/**************************************************************************/
	/*!
	Reads the state of the PN532's GPIO pins
	
	@returns An 8-bit value containing the pin state where:
	
	pinState[0]  = P30
	pinState[1]  = P31
	pinState[2]  = P32
	pinState[3]  = P33
	pinState[4]  = P34
	pinState[5]  = P35
	*/
	/**************************************************************************/
    uint8_t readGPIO(void);
	
	/**************************************************************************/
	/*!
	Sets the MxRtyPassiveActivation uint8_t of the RFConfiguration register
	
	@param  maxRetries    0xFF to wait forever, 0x00..0xFE to timeout
	after mxRetries
	
	@returns 1 if everything executed properly, 0 for an error
	*/
	/**************************************************************************/
    bool setPassiveActivationRetries(uint8_t maxRetries);
	
	uint8_t RFConfig(uint8_t data);

    /**
    * @brief    Init PN532 as a target
    * @param    timeout max time to wait, 0 means no timeout
    * @return   > 0     success
    *           = 0     timeout
    *           < 0     failed
    */
    int8_t tgInitAsTarget(uint16_t timeout = 0);
	
	/**
    * @brief    called by tgInitAsTarget(timeout)
    */
    int8_t tgInitAsTarget(const uint8_t* command, const uint8_t len, const uint16_t timeout = 0);

	/**
    * @brief    Target get data from Initiator
	* @param    *buf: buffer of received data
	* @param	len: length of received data
    * @return   > 0     length of received data
    *           < 0     failed
    */
    int16_t tgGetData(uint8_t *buf, uint8_t len);
	
	/**
    * @brief    Target send data to Initiator
	* @param    *header: header of command
	* @param	hlen: length of header
	* @param	*body: body of data
				blen: length of body part
    * @return   TRUE     send data success
    *           FALSE    send data failed
    */
    bool tgSetData(const uint8_t *header, uint8_t hlen, const uint8_t *body = 0, uint8_t blen = 0);

    int16_t inRelease(const uint8_t relevantTarget = 0);

    // ISO14443A functions
	/**************************************************************************/
	/*!
	@brief  'InLists' a passive target. PN532 acting as reader/initiator,
	peer acting as card/responder.
	*/
	/**************************************************************************/
    bool inListPassiveTarget();
	
	/**************************************************************************/
	/*!
	Waits for an ISO14443A target to enter the field
	
	@param  cardBaudRate  Baud rate of the card
	@param  uid           Pointer to the array that will be populated
	with the card's UID (up to 7 bytes)
	@param  uidLength     Pointer to the variable that will hold the
	length of the card's UID.
	
	@returns 1 if everything executed properly, 0 for an error
	*/
	/**************************************************************************/
    bool readPassiveTargetID(uint8_t cardbaudrate, uint8_t *uid, uint8_t *uidLength, uint16_t timeout = 1000);
    
	/**************************************************************************/
	/*!
	@brief  Exchanges an APDU with the currently inlisted peer
	
	@param  send            Pointer to data to send
	@param  sendLength      Length of the data to send
	@param  response        Pointer to response data
	@param  responseLength  Pointer to the response data length
	*/
	/**************************************************************************/
	bool inDataExchange(uint8_t *send, uint8_t sendLength, uint8_t *response, uint8_t *responseLength);

    // Mifare Classic functions
	/**************************************************************************/
	/*!
	Indicates whether the specified block number is the first block
	in the sector (block 0 relative to the current sector)
	*/
	/**************************************************************************/
    bool mifareclassic_IsFirstBlock (uint32_t uiBlock);
	
	/**************************************************************************/
	/*!
	Indicates whether the specified block number is the sector trailer
	*/
	/**************************************************************************/
    bool mifareclassic_IsTrailerBlock (uint32_t uiBlock);
	
	/**************************************************************************/
	/*!
	Tries to authenticate a block of memory on a MIFARE card using the
	INDATAEXCHANGE command.  See section 7.3.8 of the PN532 User Manual
	for more information on sending MIFARE and other commands.
	
	@param  uid           Pointer to a byte array containing the card UID
	@param  uidLen        The length (in bytes) of the card's UID (Should
	be 4 for MIFARE Classic)
	@param  blockNumber   The block number to authenticate.  (0..63 for
	1KB cards, and 0..255 for 4KB cards).
	@param  keyNumber     Which key type to use during authentication
	(0 = MIFARE_CMD_AUTH_A, 1 = MIFARE_CMD_AUTH_B)
	@param  keyData       Pointer to a byte array containing the 6 bytes
	key value
	
	@returns 1 if everything executed properly, 0 for an error
	*/
	/**************************************************************************/
    uint8_t mifareclassic_AuthenticateBlock (uint8_t *uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t *keyData);
    
	/**************************************************************************/
	/*!
	Tries to read an entire 16-bytes data block at the specified block
	address.
	
	@param  blockNumber   The block number to authenticate.  (0..63 for
	1KB cards, and 0..255 for 4KB cards).
	@param  data          Pointer to the byte array that will hold the
	retrieved data (if any)
	
	@returns 1 if everything executed properly, 0 for an error
	*/
	/**************************************************************************/
	uint8_t mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t *data);
	
	/**************************************************************************/
	/*!
	Tries to write an entire 16-bytes data block at the specified block
	address.
	
	@param  blockNumber   The block number to authenticate.  (0..63 for
	1KB cards, and 0..255 for 4KB cards).
	@param  data          The byte array that contains the data to write.
	
	@returns 1 if everything executed properly, 0 for an error
	*/
	/**************************************************************************/
    uint8_t mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t *data);
	
	/**************************************************************************/
	/*!
	Formats a Mifare Classic card to store NDEF Records
	
	@returns 1 if everything executed properly, 0 for an error
	*/
	/**************************************************************************/
    uint8_t mifareclassic_FormatNDEF (void);
	
	/**************************************************************************/
	/*!
	Writes an NDEF URI Record to the specified sector (1..15)
	
	Note that this function assumes that the Mifare Classic card is
	already formatted to work as an "NFC Forum Tag" and uses a MAD1
	file system.  You can use the NXP TagWriter app on Android to
	properly format cards for this.
	
	@param  sectorNumber  The sector that the URI record should be written
	to (can be 1..15 for a 1K card)
	@param  uriIdentifier The uri identifier code (0 = none, 0x01 =
	"http://www.", etc.)
	@param  url           The uri text to write (max 38 characters).
	
	@returns 1 if everything executed properly, 0 for an error
	*/
	/**************************************************************************/
    uint8_t mifareclassic_WriteNDEFURI (uint8_t sectorNumber, uint8_t uriIdentifier, const char *url);

    // Mifare Ultralight functions
	/**************************************************************************/
	/*!
	Tries to read an entire 4-bytes page at the specified address.
	
	@param  page        The page number (0..63 in most cases)
	@param  buffer      Pointer to the byte array that will hold the
	retrieved data (if any)
	*/
	/**************************************************************************/
    uint8_t mifareultralight_ReadPage (uint8_t page, uint8_t *buffer);
	
	/**************************************************************************/
	/*!
	Tries to write an entire 4-bytes data buffer at the specified page
	address.
	
	@param  page     The page number to write into.  (0..63).
	@param  buffer   The byte array that contains the data to write.
	
	@returns 1 if everything executed properly, 0 for an error
	*/
	/**************************************************************************/
    uint8_t mifareultralight_WritePage (uint8_t page, uint8_t *buffer);

    // Help functions to display formatted text
	/**************************************************************************/
	/*!
	@brief  Prints a hexadecimal value in plain characters
	
	@param  data      Pointer to the uint8_t data
	@param  numBytes  Data length in bytes
	*/
	/**************************************************************************/
    static void PrintHex(const uint8_t *data, const uint32_t numBytes);
	
	/**************************************************************************/
	/*!
	@brief  Prints a hexadecimal value in plain characters, along with
	the char equivalents in the following format
	
	00 00 00 00 00 00  ......
	
	@param  data      Pointer to the data
	@param  numBytes  Data length in bytes
	*/
	/**************************************************************************/
    static void PrintHexChar(const uint8_t *pbtData, const uint32_t numBytes);

    uint8_t *getBuffer(uint8_t *len) {
        *len = sizeof(pn532_packetbuffer) - 4;
        return pn532_packetbuffer;
    };
	
	/*
	@param  pollNr      specifies the number of polling
	@param  period  indicate the polling period in uints of 150ms
	@param  type  indicate the target type to be polled
	@param  buf  pointer to receive buffer
	@param  len  reveive buffer length
	@return  length of receive
	*/
	int InAutoPoll(uint8_t pollNr, uint8_t period, uint8_t type, uint8_t *buf, uint8_t len);
	int8_t InJumpForDEP(uint8_t *buf, uint8_t len, uint16_t timeout);

private:
    uint8_t _uid[7];  // ISO14443A uid
    uint8_t _uidLen;  // uid len
    uint8_t _key[6];  // Mifare Classic key
    uint8_t inListedTag; // Tg number of inlisted tag.

    uint8_t pn532_packetbuffer[64];

    PN532Interface *_interface;
};

#endif
