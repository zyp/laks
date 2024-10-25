#pragma once

#include <mmio/mmio.h>

struct STM32_LTDC_reg_t {
    uint32_t _reserved[2];
	volatile uint32_t SSCR;
	volatile uint32_t BPCR;
	volatile uint32_t AWCR;
	volatile uint32_t TWCR;
	volatile uint32_t GCR;
    uint32_t _reserved1[2];
	volatile uint32_t SRCR;
    uint32_t _reserved2[1];
	volatile uint32_t BCCR;
    uint32_t _reserved3[1];
	volatile uint32_t IER;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t LIPCR;
	volatile uint32_t CPSR;
	volatile uint32_t CDSR;
    uint32_t _reserved4[14];
	volatile uint32_t L1CR;
	volatile uint32_t L1WHPCR;
	volatile uint32_t L1WVPCR;
	volatile uint32_t L1CKCR;
	volatile uint32_t L1PFCR;
	volatile uint32_t L1CACR;
	volatile uint32_t L1DCCR;
	volatile uint32_t L1BFCR;
    uint32_t _reserved5[2];
	volatile uint32_t L1CFBAR;
	volatile uint32_t L1CFBLR;
	volatile uint32_t L1CFBLNR;
    uint32_t _reserved6[3];
	volatile uint32_t L1CLUTWR;
    uint32_t _reserved7[15];
	volatile uint32_t L2CR;
	volatile uint32_t L2WHPCR;
	volatile uint32_t L2WVPCR;
	volatile uint32_t L2CKCR;
	volatile uint32_t L2PFCR;
	volatile uint32_t L2CACR;
	volatile uint32_t L2DCCR;
	volatile uint32_t L2BFCR;
    uint32_t _reserved8[2];
	volatile uint32_t L2CFBAR;
	volatile uint32_t L2CFBLR;
	volatile uint32_t L2CFBLNR;
    uint32_t _reserved9[3];
	volatile uint32_t L2CLUTWR;
};

class STM32_LTDC_t : public mmio_ptr<STM32_LTDC_reg_t> {
    public:
        using mmio_ptr<STM32_LTDC_reg_t>::ptr;
};
