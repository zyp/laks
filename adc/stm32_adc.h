#pragma once

#include <mmio/mmio.h>

struct STM32_ADC_reg_v1_t {
	volatile uint32_t SR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMPR1;
	volatile uint32_t SMPR2;
	volatile uint32_t JOFR1;
	volatile uint32_t JOFR2;
	volatile uint32_t JOFR3;
	volatile uint32_t JOFR4;
	volatile uint32_t HTR;
	volatile uint32_t LTR;
	volatile uint32_t SQR1;
	volatile uint32_t SQR2;
	volatile uint32_t SQR3;
	volatile uint32_t JSQR;
	volatile uint32_t JDR1;
	volatile uint32_t JDR2;
	volatile uint32_t JDR3;
	volatile uint32_t JDR4;
	volatile uint32_t DR;
};


struct STM32_ADC_reg_v2_t {
	volatile uint32_t ISR;
	volatile uint32_t IER;
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CFGR2;
	volatile uint32_t SMPR1;
	volatile uint32_t SMPR2;
	uint32_t _reserved1;
	volatile uint32_t TR1;
	volatile uint32_t TR2;
	volatile uint32_t TR3;
	uint32_t _reserved2;
	volatile uint32_t SQR1;
	volatile uint32_t SQR2;
	volatile uint32_t SQR3;
	volatile uint32_t SQR4;
	volatile uint32_t DR;
	uint32_t _reserved3[2];
	volatile uint32_t JSQR;
	uint32_t _reserved4[4];
	volatile uint32_t OFR1;
	volatile uint32_t OFR2;
	volatile uint32_t OFR3;
	volatile uint32_t OFR4;
	uint32_t _reserved5[4];
	volatile uint32_t JDR1;
	volatile uint32_t JDR2;
	volatile uint32_t JDR3;
	volatile uint32_t JDR4;
	uint32_t _reserved6[4];
	volatile uint32_t AWD2CR;
	volatile uint32_t AWD3CR;
	uint32_t _reserved7[2];
	volatile uint32_t DIFSEL;
	volatile uint32_t CALFACT;
};

struct STM32_ADC_COMMON_reg_v2_t {
	volatile uint32_t CSR;
	uint32_t _reserved1;
	volatile uint32_t CCR;
	volatile uint32_t CDR;
};

template <typename T>
class STM32_ADC_t : public mmio_ptr<T> {
    public:
        using mmio_ptr<T>::ptr;
};

template <typename T>
class STM32_ADC_COMMON_t : public mmio_ptr<T> {
    public:
        using mmio_ptr<T>::ptr;
};
