#pragma once

#include <mmio/mmio.h>

struct STM32_TIMER_reg_v1_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t RCR;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t BDTR;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	// Many of the following are only on "new" parts like G/U/W
	volatile uint32_t OR1;
	volatile uint32_t _reserved1[4];
	volatile uint32_t AF1;
	volatile uint32_t _reserved2;
	volatile uint32_t TISEL;
};

template <typename T>
class STM32_TIMER_t : public mmio_ptr<T> {
    public:
        using mmio_ptr<T>::ptr;
};

