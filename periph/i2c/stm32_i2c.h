#pragma once

#include <mmio/mmio.h>

struct STM32_I2C_reg_v1_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t DR;
	volatile uint32_t SR1;
	volatile uint32_t SR2;
	volatile uint32_t CCR;
	volatile uint32_t TRISE;
};

struct STM32_I2C_reg_v2_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t TIMINGR;
	volatile uint32_t TIMEOUTR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t PECR;
	volatile uint32_t RXDR;
	volatile uint32_t TXDR;
};

template <typename T>
class STM32_I2C_t : public mmio_ptr<T> {
    public:
        using mmio_ptr<T>::ptr;
};
