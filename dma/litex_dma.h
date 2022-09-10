#pragma once

#include <stdint.h>
#include <mmio/mmio.h>

struct LiteX_Wishbone_DMA_reg_t {
	volatile uint32_t BASEH;
	volatile uint32_t BASEL;
	volatile uint32_t LENGTH;
	volatile uint32_t ENABLE;
	volatile uint32_t DONE;
	volatile uint32_t LOOP;
	volatile uint32_t OFFSET;
};

class LiteX_Wishbone_DMA_Reader_t : public mmio_ptr<LiteX_Wishbone_DMA_reg_t> {
	public:
		using mmio_ptr<LiteX_Wishbone_DMA_reg_t>::ptr;
};

class LiteX_Wishbone_DMA_Writer_t : public mmio_ptr<LiteX_Wishbone_DMA_reg_t> {
	public:
		using mmio_ptr<LiteX_Wishbone_DMA_reg_t>::ptr;
};
