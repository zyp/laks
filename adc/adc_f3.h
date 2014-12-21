#ifndef ADC_F3_H
#define ADC_F3_H

#include <stdint.h>

struct ADC_t {
	volatile uint32_t ISR;
	volatile uint32_t IER;
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	uint32_t _reserved;
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

#if defined(STM32F3)
static ADC_t& ADC1 = *(ADC_t*)0x50000000;
static ADC_t& ADC2 = *(ADC_t*)0x50000100;
static ADC_t& ADC3 = *(ADC_t*)0x50000400;
static ADC_t& ADC4 = *(ADC_t*)0x50000500;
#endif

#endif 
