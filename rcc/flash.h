#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

struct FLASH_t {
	#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3)
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
	#elif defined(STM32WB)
	volatile uint32_t ACR;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t ECCR;
	volatile uint32_t OPTR;
	volatile uint32_t PCROP1ASR;
	volatile uint32_t PCROP1AER;
	volatile uint32_t WRP1AR;
	volatile uint32_t WRP1BR;
	volatile uint32_t PCROP1BSR;
	volatile uint32_t PCROP1BER;
	volatile uint32_t IPCCBR;
	volatile uint32_t _reserved1[8];
	volatile uint32_t C2ACR;
	volatile uint32_t C2SR;
	volatile uint32_t C2CR; // 0x64
	volatile uint32_t _reserved2[7];
	volatile uint32_t SFR; // 0x80
	volatile uint32_t SRRVR;
	#endif
};

#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3)
static FLASH_t& FLASH = *(FLASH_t*)0x40022000;
#elif defined(STM32F4)
static FLASH_t& FLASH = *(FLASH_t*)0x40023c00;
#elif defined(STM32L0)
static FLASH_t& FLASH = *(FLASH_t*)0x40022000;
#elif defined(STM32WB)
static FLASH_t& FLASH = *(FLASH_t*)0x58004000;
#endif

void flash_init();

#endif
