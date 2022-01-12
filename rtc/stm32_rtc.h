#pragma once

#include <mmio/mmio.h>

struct STM32_RTC_reg_v2ss_t {
	volatile uint32_t TR;
	volatile uint32_t DR;
	volatile uint32_t CR;
	volatile uint32_t ISR;
	volatile uint32_t PRER;
	volatile uint32_t WUTR;
	uint32_t _reserved1;
	volatile uint32_t ALRMAR;
	volatile uint32_t ALRMBR;
	volatile uint32_t WPR;
	volatile uint32_t SSR;
	volatile uint32_t SHIFTR;
	volatile uint32_t TSTR;
	volatile uint32_t TSDR;
	volatile uint32_t TSSSR;
	volatile uint32_t CALR;
	volatile uint32_t TAMPCR;
	volatile uint32_t ALRMASSR;
	volatile uint32_t ALRMBSSR;
	volatile uint32_t OR;
	volatile uint32_t BKP[32]; /* max known 32, might be less, check DS */
};

template <typename T>
class STM32_RTC_t : public mmio_ptr<T> {
	public:
		using mmio_ptr<T>::ptr;

		void lock(void) const {
			ptr()->WPR = 0x99;  // Doesn't matter...
		}

		void unlock(void) const {
			ptr()->WPR = 0xCA;
			ptr()->WPR = 0x53;
		}

};

