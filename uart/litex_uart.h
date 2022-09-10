#pragma once

#include <mmio/mmio.h>

struct LiteX_UART_reg_t {
	volatile uint32_t RXTX;
	volatile uint32_t TXFULL;
	volatile uint32_t RXEMPTY;
	volatile uint32_t EV_STATUS;
	volatile uint32_t EV_PENDING;
	volatile uint32_t EV_ENABLE;
	volatile uint32_t TXEMPTY;
	volatile uint32_t RXFULL;
};

class LiteX_UART_t : public mmio_ptr<LiteX_UART_reg_t> {
    public:
        using mmio_ptr<LiteX_UART_reg_t>::ptr;

	bool txe() const {
        return !ptr()->TXFULL;
	}

	bool rxne() const {
        return !ptr()->RXEMPTY;
	}

	uint8_t read() const {
        return ptr()->RXTX;
	}

	void write(uint8_t data) const {
        ptr()->RXTX = data;
	}

	uint8_t read_blocking() const {
		while(!rxne());
		return read();
	}

	void write_blocking(uint8_t data) const {
		while(!txe());
		write(data);
	}
};
