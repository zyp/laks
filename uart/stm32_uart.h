#pragma once

#include <mmio/mmio.h>

#include <type_traits>

struct STM32_UART_reg_v1_t {
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
};

struct STM32_UART_reg_v2_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t BRR;
	volatile uint32_t GTPR;
	volatile uint32_t RTOR;
	volatile uint32_t RQR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t RDR;
	volatile uint32_t TDR;
	volatile uint32_t PRESC; // Gx, L4+, Wx etc
};

struct STM32_UART_reg_lpv1_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t BRR;
	uint32_t _1[2];
	volatile uint32_t RQR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t RDR;
	volatile uint32_t TDR;
};

template <typename T>
class STM32_UART_t : public mmio_ptr<T> {
	static constexpr auto is_v1 = std::is_same_v<T, STM32_UART_reg_v1_t>;
	static constexpr auto is_v2 = std::is_same_v<T, STM32_UART_reg_v2_t>;
	static constexpr auto is_lpv1 = std::is_same_v<T, STM32_UART_reg_lpv1_t>;

    public:
        using mmio_ptr<T>::ptr;

	void write_blocking(uint8_t data) const requires is_v1 {
		// wait for TXE/TXFNF
		while (!(ptr()->SR & (1<<7)));
		ptr()->DR = data;
	}

	void write_blocking(uint8_t data) const requires is_v2 || is_lpv1 {
		// wait for TXE/TXFNF
		while (!(ptr()->ISR & (1<<7)));
		ptr()->TDR = data;
	}
};
