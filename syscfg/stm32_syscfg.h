#pragma once

#include <mmio/mmio.h>

struct STM32_SYSCFG_reg_f4_t {
	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	volatile uint32_t CMPCR;
};


struct STM32_SYSCFG_reg_l0_t {
	volatile uint32_t CFGR1;
	volatile uint32_t CFGR2;
	volatile uint32_t EXTICR[4];
	volatile uint32_t COMP1_CTRL;
	volatile uint32_t COMP2_CTRL;
	volatile uint32_t CFGR3;
};

struct STM32_SYSCFG_reg_wb_t {
	volatile uint32_t MEMRMP;
	volatile uint32_t CFGR1;
	volatile uint32_t EXTICR[4];
	volatile uint32_t SCSR;
	volatile uint32_t CFGR2;
	volatile uint32_t SWPR;
	volatile uint32_t SKR;
	volatile uint32_t SWPR2;
	uint32_t _reserved1[53];  // basicallly, vrefbuf is in this slot!
	volatile uint32_t IMR1;
	volatile uint32_t IMR2;
	volatile uint32_t C2IMR1;
	volatile uint32_t C2IMR2;
	volatile uint32_t SIPCR;
};


template <typename T>
class STM32_SYSCFG_t : public mmio_ptr<T> {
    public:
        using mmio_ptr<T>::ptr;
};
