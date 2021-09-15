#pragma once

#include <mmio/mmio.h>

#include "rcc_enums.h"

struct RCC_reg_f0_t {
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t AHBRSTR;
	volatile uint32_t CFGR2;
	volatile uint32_t CFGR3;
	volatile uint32_t CR2;
};

struct RCC_reg_f1_t {
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
};

struct RCC_reg_f3_t {
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t AHBRSTR;
	volatile uint32_t CFGR2;
	volatile uint32_t CFGR3;
};

struct RCC_reg_f4_t {
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	volatile uint32_t AHB4RSTR; // Reserved
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB3RSTR; // Reserved
	volatile uint32_t APB4RSTR; // Reserved
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t AHB4ENR; // Reserved
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB3ENR; // Reserved
	volatile uint32_t APB4ENR; // Reserved
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	volatile uint32_t AHB4LPENR; // Reserved
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t APB3LPENR; // Reserved
	volatile uint32_t APB4LPENR; // Reserved
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t _1;
	volatile uint32_t _2;
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
};

struct RCC_reg_l0_t {
	volatile uint32_t CR;
	volatile uint32_t ICSCR;
	volatile uint32_t CRRCR;
	volatile uint32_t CFGR;
	volatile uint32_t CIER;
	volatile uint32_t CIFR;
	volatile uint32_t CICR;
	volatile uint32_t IOPRSTR;
	volatile uint32_t AHBRSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t IOPENR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t IOPSMENR;
	volatile uint32_t AHBSMENR;
	volatile uint32_t APB2SMENR;
	volatile uint32_t APB1SMENR;
	volatile uint32_t CCIPR;
	volatile uint32_t CSR;
};

template <typename T>
class RCC_t : public mmio_ptr<T> {
    public:
        using mmio_ptr<T>::ptr;

        template <typename Bus>
        void enable(Bus dev) const {
            rcc::enable(*this, dev);
        }
};