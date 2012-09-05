#ifndef ADC_H
#define ADC_H

#include <stdint.h>

struct ADC_t {
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

#if defined(STM32F1)
static ADC_t& ADC1 = *(ADC_t*)0x40012400;
static ADC_t& ADC2 = *(ADC_t*)0x40012800;
static ADC_t& ADC3 = *(ADC_t*)0x40013c00;
#elif defined(STM32F4)

#endif

#endif 
