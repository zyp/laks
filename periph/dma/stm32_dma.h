#pragma once

#include <stdint.h>
#include <mmio/mmio.h>

struct STM32_DMA_channel_reg_v1_t {
	volatile uint32_t CR;
	volatile uint32_t NDTR;
	volatile uint32_t PAR;
	volatile uint32_t MAR;
	uint32_t _reserved;
};

struct STM32_DMA_reg_v1_t {
	volatile uint32_t ISR;
	volatile uint32_t IFCR;
	STM32_DMA_channel_reg_v1_t C[7];
};


struct STM32_DMA_stream_reg_v2_t {
	volatile uint32_t CR;
	volatile uint32_t NDTR;
	volatile uint32_t PAR;
	volatile uint32_t M0AR;
	volatile uint32_t M1AR;
	volatile uint32_t FCR;
};

struct STM32_DMA_reg_v2_t {
	volatile uint32_t LISR;
	volatile uint32_t HISR;
	volatile uint32_t LIFCR;
	volatile uint32_t HIFCR;
	STM32_DMA_stream_reg_v2_t S[8];
};


template <typename T>
class STM32_DMA_t : public mmio_ptr<T> {
	public:
		using mmio_ptr<T>::ptr;
};
