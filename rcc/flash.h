#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

struct FLASH_t {
	volatile uint32_t ACR;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	#if defined(STM32F1) || defined(STM32F3)
	volatile uint32_t AR;
	volatile uint32_t RESERVED;
	volatile uint32_t OBR;
	volatile uint32_t WRPR;
	#elif defined(STM32F4)
	volatile uint32_t OPTCR;
	#endif
};

#if defined(STM32F1) || defined(STM32F3)
static FLASH_t& FLASH = *(FLASH_t*)0x40022000;
#elif defined(STM32F4)
static FLASH_t& FLASH = *(FLASH_t*)0x40023c00;
#endif

void flash_init();

#endif
