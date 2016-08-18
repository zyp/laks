#ifndef CRS_H
#define CRS_H

#include <stdint.h>

struct CRS_reg_t {
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
};

class CRS_t {
	public:
		CRS_reg_t& reg;
		
		CRS_t(uint32_t reg_addr) : reg(*(CRS_reg_t*)reg_addr) {}
		
		void enable() {
			reg.CR |= (1 << 6) | (1 << 5); // AUTOTRIMEN, CEN
		}
};

#if defined(STM32L0)
static CRS_t CRS(0x40006c00);
#endif

void flash_init();

#endif
