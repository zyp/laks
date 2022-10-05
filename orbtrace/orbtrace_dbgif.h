#pragma once

#include <stdint.h>
#include <mmio/mmio.h>

struct Orbtrace_DBGIF_reg_t {
	volatile uint32_t CMD;
	volatile uint32_t STATUS;
	volatile uint32_t DWRITE;
	volatile uint32_t DREAD;
	volatile uint32_t PINS_OUT;
	volatile uint32_t PINS_IN;
};

class Orbtrace_DBGIF_t : public mmio_ptr<Orbtrace_DBGIF_reg_t> {
	public:
		using mmio_ptr<Orbtrace_DBGIF_reg_t>::ptr;
};
