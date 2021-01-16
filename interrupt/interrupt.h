#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#include "interrupt_enums.h"

struct NVIC_t {
	volatile uint32_t ISER[32];
	volatile uint32_t ICER[32];
	volatile uint32_t ISPR[32];
	volatile uint32_t ICPR[32];
	volatile uint32_t IABR[64];
	volatile uint8_t IPR[2816];
	volatile uint32_t STIR;
};

static NVIC_t& NVIC = *(NVIC_t*)0xe000e100;

struct SCB_t {
	volatile uint32_t CPUID;
	volatile uint32_t ICSR;
	volatile uint32_t VTOR;
	volatile uint32_t AIRCR;
	volatile uint32_t SCR;
	volatile uint32_t CCR;
	volatile uint8_t SHPR[12];
	volatile uint32_t SHCSR;
	volatile uint32_t CFSR;
	volatile uint32_t HFSR;
	volatile uint32_t DFSR;
	volatile uint32_t MMAR;
	volatile uint32_t BFAR;
};

static SCB_t& SCB = *(SCB_t*)0xe000ed00;

namespace Interrupt {
	inline void enable(IRQ n) {
		NVIC.ISER[int(n) >> 5] = 1 << (int(n) & 0x1f);
	}
	
	inline void set_priority(Exception n, uint8_t priority) {
		SCB.SHPR[int(n) - 4] = priority;
	}
	
	inline void set_priority(IRQ n, uint8_t priority) {
		NVIC.IPR[int(n)] = priority;
	}
};

template<Exception>
void interrupt();

template<IRQ>
void interrupt();

#endif
