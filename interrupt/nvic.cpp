module;

#include <mmio/mmio.h>

import laks.periph.interrupt.enums;

export module laks.periph.interrupt.nvic;

export struct NVIC_reg_t {
	volatile uint32_t ISER[32];
	volatile uint32_t ICER[32];
	volatile uint32_t ISPR[32];
	volatile uint32_t ICPR[32];
	volatile uint32_t IABR[64];
	volatile uint8_t IPR[2816];
	volatile uint32_t STIR;
};

export struct SCB_reg_t {
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

export class NVIC_t : public mmio_ptr<NVIC_reg_t> {
	public:
        mmio_ptr<SCB_reg_t> SCB;

        constexpr NVIC_t(uintptr_t offset) :
            mmio_ptr(offset),
            SCB(offset + 0xc00) {}

        void enable(interrupt::irq n) const {
            ptr()->ISER[int(n) >> 5] = 1 << (int(n) & 0x1f);
        }

        void set_priority(interrupt::exception n, uint8_t priority) const {
            SCB->SHPR[int(n) - 4] = priority;
        }
        
        void set_priority(interrupt::irq n, uint8_t priority) const {
            ptr()->IPR[int(n)] = priority;
        }
};
