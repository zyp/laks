#pragma once

#include <stdint.h>
#include <mmio/mmio.h>

struct STM32_HSEM_reg_v1_t {
	volatile uint32_t R[32];
	volatile uint32_t RLR[32];
	volatile uint32_t C1IER;
	volatile uint32_t C1ICR;
	volatile uint32_t C1ISR;
	volatile uint32_t C1MISR;
	volatile uint32_t C2IER;
	volatile uint32_t C2ICR;
	volatile uint32_t C2ISR;
	volatile uint32_t C2MISR;
	uint32_t _reserved[8];
	volatile uint32_t CR;
	volatile uint32_t KEYR;
};

template <typename T>
class STM32_HSEM_t : public mmio_ptr<T> {
	public:
		using mmio_ptr<T>::ptr;

		bool get_lock_1step(uint32_t sem) const {
			return ptr()->RLR[sem] == (uint32_t)((1<<31) | (4<<8)); // cpu1 coreid
		}

		void release(uint32_t sem, uint32_t process) const {
			ptr()->R[sem] = (4<<8) | process;  // cpu1 coreid
		}
};