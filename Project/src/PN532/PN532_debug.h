#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "stdio.h"

#define DEBUG

int xprintf(const char *format, ...);

#ifdef DEBUG
#define DMSG(args...)		xprintf(args)
#define DMSG_CHAR(c)		xprintf("%c", c)
#define DMSG_STR(str)		xprintf("%s\r\n", str)
#define DMSG_HEX(num)		xprintf(" %x", num)
#define DMSG_INT(num)		xprintf(" %d", num)
#else
#define DMSG(args...)
#define DMSG_CHAR(c)
#define DMSG_STR(str)       
#define DMSG_HEX(num)       
#define DMSG_INT(num)     
#endif

#endif
