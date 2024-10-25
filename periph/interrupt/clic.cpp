module;

#include <mmio/mmio.h>

import laks.periph.interrupt.enums;

export module laks.periph.interrupt.clic;

export struct CLIC_int_reg_t {
    volatile uint8_t IP;
    volatile uint8_t IE;
    volatile uint8_t ATTR;
    volatile uint8_t CTL;
};

export struct CLIC_reg_t {
    volatile uint32_t CFG;
    volatile uint32_t INFO;
    uint32_t _reserved[14];
    volatile uint32_t TRIG[32];
    uint32_t _reserved2[976];
    CLIC_int_reg_t INT[4096];
};

export class CLIC_t : public mmio_ptr<CLIC_reg_t> {
	public:
        void enable(interrupt::irq n) const {
            ptr()->INT[int(n)].IE = 1;
        }

        void enable(interrupt::internal n) const {
            ptr()->INT[int(n)].IE = 1;
        }
};
