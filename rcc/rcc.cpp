#include "rcc.h"
#include "flash.h"

void rcc_init() {
	// Initialize flash.
	flash_init();
	
	#if defined(STM32F1) || defined(STM32F3)
	
	// Enable HSE.
	RCC.CR |= 0x10000;
	while(!(RCC.CR & 0x20000));
	
	// Configure and enable PLL.
	RCC.CFGR = 0x1d0000;
	RCC.CR |= 0x1000000;
	while(!(RCC.CR & 0x2000000));
	
	// Switch to PLL.
	RCC.CFGR |= 0x2;
	while(!(RCC.CFGR & 0x8));
	
	// Set APB1 prescaler to /2.
	RCC.CFGR |= 0x400;
	
	// Set ADCCLK prescaler to /6.
	RCC.CFGR |= 0x8000;
	
	#elif defined(STM32F4)
	
	// Enable HSE.
	RCC.CR |= 0x10000;
	while(!(RCC.CR & 0x20000));
	
	// Configure and enable PLL.
	RCC.PLLCFGR = 0x20400000 | (7 << 24) | (2 * 168 << 6) | 8;
	RCC.CR |= 0x1000000;
	while(!(RCC.CR & 0x2000000));
	
	// Switch to PLL.
	RCC.CFGR |= 0x2;
	while(!(RCC.CFGR & 0x8));
	
	// Set APB1 prescaler to /4.
	RCC.CFGR |= 5 << 10;
	
	// Set APB2 prescaler to /2.
	RCC.CFGR |= 4 << 13;
	
	#endif
}
