#ifndef SYSCFG_H
#define SYSCFG_H

#include <stdint.h>

struct SYSCFG_t {
	volatile uint32_t MEMRM;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	volatile uint32_t CMPCR;
};

#if defined(STM32F4)
static SYSCFG_t& SYSCFG = *(SYSCFG_t*)0x40013800;
#endif

void rcc_init();

#endif
