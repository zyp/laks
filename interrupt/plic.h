#pragma once

class PLIC_t {
	private:
        struct PLIC_ctx_reg_t {
            volatile uint32_t THRESHOLD;
            volatile uint32_t CLAIM;
            uint32_t _reserved[1022];
        };

		struct PLIC_reg_t {
			volatile uint32_t PRIO[1024];
            volatile uint32_t PEND[32];
            uint32_t _reserved[992];
            volatile uint32_t ENABLE[15872][32];
            uint32_t _reserved2[14336];
            PLIC_ctx_reg_t CTX[15872];
		};
		
	public:
		PLIC_reg_t& reg;
		
		PLIC_t(uint32_t reg_addr) :
			reg(*(PLIC_reg_t*)reg_addr) {}
};
