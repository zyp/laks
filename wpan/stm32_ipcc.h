#pragma once

#include <stdint.h>
#include <mmio/mmio.h>


struct STM32_IPCC_reg_v1_t {
	volatile uint32_t C1CR;
	volatile uint32_t C1MR;
	volatile uint32_t C1SCR;
	volatile uint32_t C1TOC2SR;
	volatile uint32_t C2CR;
	volatile uint32_t C2MR;
	volatile uint32_t C2SCR;
	volatile uint32_t C2TOC1SR;
};

template <typename T>
class STM32_IPCC_t : public mmio_ptr<T> {
	public:
		using mmio_ptr<T>::ptr;
};
