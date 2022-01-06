#pragma once

#include <mmio/mmio.h>

struct STM32_PWR_reg_wb_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t CR4;
	volatile uint32_t SR1;
	volatile uint32_t SR2;
	volatile uint32_t SCR;
	volatile uint32_t CR5;
	volatile uint32_t PUCRA;
	volatile uint32_t PDCRA;
	volatile uint32_t PUCRB;
	volatile uint32_t PDCRB;
	volatile uint32_t PUCRC;
	volatile uint32_t PDCRC;
	volatile uint32_t PUCRD;
	volatile uint32_t PDCRD;
	volatile uint32_t PUCRE;
	volatile uint32_t PDCRE;
	volatile uint32_t _reserved1[4];
	volatile uint32_t PUCRH;
	volatile uint32_t PDCRH;
	volatile uint32_t _reserved2[8];
	volatile uint32_t C2CR1;
	volatile uint32_t C2CR3;
	volatile uint32_t EXTSCR;
};


template <typename T>
class STM32_PWR_t : public mmio_ptr<T> {
	public:
		using mmio_ptr<T>::ptr;
};

// TODO - methods for pullup/pulldowns?

