#pragma once

#include <stdint.h>
#include <mmio/mmio.h>

struct STM32_DMAMUX_reg_v1_t {
	/**
	 * Not all parts offer all channels!
	 * WB: 14, G4: 16...
	 */
	volatile uint32_t CCR[20];
	volatile uint32_t _reserved1[12];
	volatile uint32_t CSR; // 0x80
	volatile uint32_t CFR; // 0x84
	volatile uint32_t _reserved2[30];
	/**
	 * Not all parts offer all requests!
	 * WB/G4: 4, H7: 8
	 */
	volatile uint32_t RGCR[8]; // 0x100
	volatile uint32_t _reserved3[8];
	volatile uint32_t RGSR; // 0x140
	volatile uint32_t RGCFR;
};

template <typename T>
class STM32_DMAMUX_t : public mmio_ptr<T> {
	public:
		using mmio_ptr<T>::ptr;
};