#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

struct FLASH_t {
	#if defined(STM32F1) || defined(STM32F3)
	volatile uint32_t ACR;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t AR;
	volatile uint32_t RESERVED;
	volatile uint32_t OBR;
	volatile uint32_t WRPR;
	#elif defined(STM32F4)
	volatile uint32_t ACR;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t OPTCR;
	#elif defined(STM32L0)
	volatile uint32_t ACR;
	volatile uint32_t PECR;
	volatile uint32_t PDKEYR;
	volatile uint32_t PEKEYR;
	volatile uint32_t PRGKEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t OPTR;
	volatile uint32_t WRPROT;
	#endif
};

#if defined(STM32F1) || defined(STM32F3)
static FLASH_t& FLASH = *(FLASH_t*)0x40022000;
#elif defined(STM32F4)
static FLASH_t& FLASH = *(FLASH_t*)0x40023c00;
#elif defined(STM32L0)
static FLASH_t& FLASH = *(FLASH_t*)0x40022000;
#endif

void flash_init();

#endif
