#ifndef SYSCFG_H
#define SYSCFG_H

#include <stdint.h>

#if defined(STM32L0)

struct SYSCFG_t {
	volatile uint32_t CFGR1;
	volatile uint32_t CFGR2;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
	volatile uint32_t COMP1_CTRL;
	volatile uint32_t COMP2_CTRL;
	volatile uint32_t CFGR3;
};

static SYSCFG_t& SYSCFG = *(SYSCFG_t*)0x40010000;

#endif

#endif
