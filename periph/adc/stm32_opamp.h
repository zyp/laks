#pragma once

#include <mmio/mmio.h>

struct STM32_OPAMP_reg_t {
	volatile uint32_t CSR1;
	volatile uint32_t CSR2;
	volatile uint32_t CSR3;
	volatile uint32_t CSR4;
	volatile uint32_t CSR5;
	volatile uint32_t CSR6;
	volatile uint32_t TCMR1;
	volatile uint32_t TCMR2;
	volatile uint32_t TCMR3;
	volatile uint32_t TCMR4;
	volatile uint32_t TCMR5;
	volatile uint32_t TCMR6;
};

class STM32_OPAMP_t : public mmio_ptr<STM32_OPAMP_reg_t> {
    public:
        using mmio_ptr<STM32_OPAMP_reg_t>::ptr;
};
