
#include "snep.h"
#include "PN532_debug.h"

int8_t SNEP::write(const uint8_t *buf, uint8_t len, uint16_t timeout)
{
	if (0 >= llcp.activate(timeout)) {
		DMSG("failed to activate PN532 as a target\r\n");
		return -1;
	}

	if (0 >= llcp.connect(timeout)) {//76
		DMSG("failed to set up a connection\r\n");
		return -2;
	}

	// response a success SNEP message
	headerBuf[0] = SNEP_DEFAULT_VERSION;
	headerBuf[1] = SNEP_REQUEST_PUT;
	headerBuf[2] = 0;
	headerBuf[3] = 0;
	headerBuf[4] = 0;
	headerBuf[5] = len;
	if (0 >= llcp.write(headerBuf, 6, buf, len)) {//132
		return -3;
	}

	uint8_t rbuf[16];
	if (6 > llcp.read(rbuf, sizeof(rbuf))) {//183
		return -4;
	}

	// check SNEP version
	if (SNEP_DEFAULT_VERSION != rbuf[0]) {
		DMSG("The received SNEP message's major version is different\r\n");
		// To-do: send Unsupported Version response
		return -4;
	}

	// expect a put request
	if (SNEP_RESPONSE_SUCCESS != rbuf[1]) {
		DMSG("Expect a success response\r\n");
		return -4;
	}

	llcp.disconnect(timeout);

	return 1;
}

int16_t SNEP::read(uint8_t *buf, uint8_t len, uint16_t timeout)
{
	if (0 >= llcp.activate(timeout)) {//_1_2
		DMSG("failed to activate PN532 as a target\r\n");
		return -1;
	}

	if (0 >= llcp.waitForConnection(timeout)) {//_3_4
		DMSG("failed to set up a connection\r\n");
		return -2;
	}

	uint16_t status = llcp.read(buf, len);//_5_6  在这一步接收到发送过来的数据
	if (6 > status) {
		return -3;
	}

	// check SNEP version
	if (SNEP_DEFAULT_VERSION != buf[0]) {
		DMSG("The received SNEP message's major version is different\r\n");
		// To-do: send Unsupported Version response
		return -4;
	}

	// expect a put request
	if (SNEP_REQUEST_PUT != buf[1]) {
		DMSG("Expect a put request\r\n");
		return -4;
	}

	// check message's length
	uint32_t length = (buf[2] << 24) + (buf[3] << 16) + (buf[4] << 8) + buf[5];
	// length should not be more than 244 (header + body < 255, header = 6 + 3 + 2)
	if (length > 262) {
		DMSG("The SNEP message is too large: "); 
        DMSG_INT(length);
        DMSG_INT(status - 6);
		DMSG("\r\n");
		return -4;
	}
	for (uint8_t i = 0; i < length; i++) {
		buf[i] = buf[i + 6];
	}

	// response a success SNEP message
	headerBuf[0] = SNEP_DEFAULT_VERSION;
	headerBuf[1] = SNEP_RESPONSE_SUCCESS;
	headerBuf[2] = 0;
	headerBuf[3] = 0;
	headerBuf[4] = 0;
	headerBuf[5] = 0;
	if (!llcp.write(headerBuf, 6))//_7_8_9_10
	{
		DMSG("response a success SNEP message false\r\n");
		return -4;		
	}
	
	return length;
}
