#ifndef __CC2520_UTIL_H__
#define __CC2520_UTIL_H__

uint8  CC2520_MEMWR(uint16 addr, uint16 count, uint8  *pData);
uint8  CC2520_MEMWR8(uint16 addr, uint8 value);
uint8  CC2520_MEMWR16(uint16 addr, uint16 value);
uint8  CC2520_MEMWR24(uint16 addr, uint32 value);

#endif
