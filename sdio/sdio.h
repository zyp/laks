#ifndef SDIO_H
#define SDIO_H

#include <stdint.h>

struct SDIO_reg_t {
	volatile uint32_t POWER;
	volatile uint32_t CLKCR;
	volatile uint32_t ARG;
	volatile uint32_t CMD;
	volatile uint32_t RESPCMD;
	volatile uint32_t RESP1;
	volatile uint32_t RESP2;
	volatile uint32_t RESP3;
	volatile uint32_t RESP4;
	volatile uint32_t DTIMER;
	volatile uint32_t DLEN;
	volatile uint32_t DCTRL;
	volatile uint32_t DCOUNT;
	volatile uint32_t STA;
	volatile uint32_t ICR;
	volatile uint32_t MASK;
	uint32_t _reserved[2];
	volatile uint32_t FIFOCNT;
	uint32_t _reserved1[13];
	volatile uint32_t FIFO;
};

class SDIO_t {
	public:
		SDIO_reg_t& reg;
		
		SDIO_t(uint32_t reg_addr) : reg(*(SDIO_reg_t*)reg_addr) {}
};

#if defined(STM32F1)
static SDIO_t SDIO(0x40018000);
#elif defined(STM32F4)
static SDIO_t SDIO(0x40012c00);
#endif

#endif
