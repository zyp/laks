#pragma once

#include <mmio/mmio.h>

struct STM32_PWR_PULL_PORT_t {
	volatile uint32_t PUCR;
	volatile uint32_t PDCR;
};

struct STM32_PWR_reg_g4_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t CR4;
	volatile uint32_t SR1;
	volatile uint32_t SR2;
	volatile uint32_t SCR;
	uint32_t _reserved1;
	volatile struct STM32_PWR_PULL_PORT_t PUPD[7];
	uint32_t _reserved2[10];
	volatile uint32_t CR5;
};


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

		// WARNING - 3 bits on g0/g4/l4/l5/wb, 2 bits on f0/f1/f3/f4/f7/l0/l1
		// (and only _1_ bit on h7!)
		void set_lpms(uint32_t level) const {
			ptr()->CR1 &= ~(0x7);
			ptr()->CR1 |= (level);
		}

		/// Sets the LPMS state for CPU2.  (only available on WB)
		/// \param level
		void set_lpms_c2(uint32_t level) const {
			ptr()->C2CR1 &= ~(0x7);
			ptr()->C2CR1 |= (level);
		}
};

// TODO - methods for pullup/pulldowns?

