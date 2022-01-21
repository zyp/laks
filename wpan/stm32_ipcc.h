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

		bool tx_pending(uint32_t channel) const {
			bool unmasked = ~(ptr()->C1MR) & ((1<<channel) << 16);
			return unmasked && (!(ptr()->C1TOC2SR & (1<<channel)));
		}

		bool rx_pending(uint32_t channel) const {
			bool unmasked = ~(ptr()->C1MR) & (1<<channel);
			return unmasked && (ptr()->C2TOC1SR & (1<<channel));
		}

		void c1_set_flag(const uint32_t channel) const {
			ptr()->C1SCR = (1<<channel) << 16;
		}
		void c1_clear_flag(const uint32_t channel) const {
			ptr()->C1SCR = (1<<channel);
		}

		void enable_rx(const uint32_t channel) const {
			ptr()->C1MR &= ~(1<<channel);
		}
		void disable_rx(const uint32_t channel) const {
			ptr()->C1MR |= (1<<channel);
		}

		void enable_tx(const uint32_t channel) const {
			ptr()->C1MR &= ~((1<<channel) << 16);
		}
		void disable_tx(const uint32_t channel) const {
			ptr()->C1MR |= (1<<channel) << 16;
		}

};
