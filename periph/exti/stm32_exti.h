#pragma once

#include <mmio/mmio.h>

struct STM32_EXTI_reg_l4_t {
	volatile uint32_t IMR1;
	volatile uint32_t EMR1;
	volatile uint32_t RTSR1;
	volatile uint32_t FTSR1;
	volatile uint32_t SWIER1;
	volatile uint32_t PR1;
	uint32_t _reserved1[2];
	volatile uint32_t IMR2;
	volatile uint32_t EMR2;
	volatile uint32_t RTSR2;
	volatile uint32_t FTSR2;
	volatile uint32_t SWIER2;
	volatile uint32_t PR2;
};

struct STM32_EXTI_reg_wb_t {
	volatile uint32_t RTSR1;
	volatile uint32_t FTSR1;
	volatile uint32_t SWIER1;
	volatile uint32_t PR1;
	uint32_t _reserved1[4];
	volatile uint32_t RTSR2;
	volatile uint32_t FTSR2;
	volatile uint32_t SWIER2;
	volatile uint32_t PR2;
	uint32_t _reserved2[20];
	volatile uint32_t IMR1;
	volatile uint32_t EMR1;
	uint32_t _reserved3[2];
	volatile uint32_t IMR2;
	volatile uint32_t EMR2;
	uint32_t _reserved4[10];
	volatile uint32_t C2IMR1;
	volatile uint32_t C2EMR1;
	uint32_t _reserved5[2];
	volatile uint32_t C2IMR2;
	volatile uint32_t C2EMR2;
};

template <typename T>
class STM32_EXTI_t : public mmio_ptr<T> {
    public:
        using mmio_ptr<T>::ptr;
};
