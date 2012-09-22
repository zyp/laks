#ifndef DMA_H
#define DMA_H

#include <stdint.h>

struct DMA_stream_reg_t {
	volatile uint32_t CR;
	volatile uint32_t NDTR;
	volatile uint32_t PAR;
	volatile uint32_t M0AR;
	volatile uint32_t M1AR;
	volatile uint32_t FCR;
};

struct DMA_reg_t {
	volatile uint32_t LISR;
	volatile uint32_t HISR;
	volatile uint32_t LIFCR;
	volatile uint32_t HIFCR;
	DMA_stream_reg_t S[8];
};

class DMA_t {
	public:
		DMA_reg_t& reg;
		
		DMA_t(uint32_t reg_addr) : reg(*(DMA_reg_t*)reg_addr) {}
};

#if defined(STM32F1)

#elif defined(STM32F4)
static DMA_t DMA1(0x40026000);
static DMA_t DMA2(0x40026400);
#endif

#endif
