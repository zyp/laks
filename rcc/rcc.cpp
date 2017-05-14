#include "rcc.h"
#include "flash.h"
#include "../syscfg/syscfg.h"

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
	
	#elif defined(STM32L0)
	
	// Enable HSI16.
	RCC.CR |= 1 << 0; // HSI16ON
	while(!(RCC.CR & (1 << 2))); // HSI16RDYF
	
	// Configure PLL.
	RCC.CFGR |= (1 << 22) | (1 << 18) | (0 << 16); // PLLDIV = /2, PLLMUL = 4x, PLLSRC = HSI16
	
	// Enable PLL.
	RCC.CR |= 1 << 24; // PLLON
	while(!(RCC.CR & (1 << 25))); // PLLRDY
	
	// Switch to PLL.
	RCC.CFGR |= 3 << 0; // SW = PLL
	while((RCC.CFGR & (3 << 2)) != (3 << 2)); // SWS = PLL
	
	// Enable VREFINT for HSI48.
	RCC.enable(RCC.SYSCFG);
	SYSCFG.CFGR3 |= (1 << 13) | (1 << 0); // ENREF_HSI48, EN_VREFINT
	while(!(SYSCFG.CFGR3 & (1 << 26))); // REF_HSI48_RDYF
	
	// Enable HSI48.
	RCC.CRRCR |= 1 << 0; // HSI48ON
	while(!(RCC.CRRCR & (1 << 1))); // HSI48RDY
	
	// Select HSI48 for USB.
	RCC.CCIPR |= 1 << 26;
	
	#endif
}

#if defined(STM32F4)
void rcc_init(uint32_t osc_mhz, uint32_t sysclk_mhz) {
	// Initialize flash.
	flash_init();
	
	uint32_t pll_mhz = sysclk_mhz > 192 / 2 ? sysclk_mhz * 2 : sysclk_mhz * 4;
	uint32_t pllp = sysclk_mhz > 192 / 2 ? 0 : 1;
	
	// Enable HSE.
	RCC.CR |= 0x10000;
	while(!(RCC.CR & 0x20000));
	
	// Configure and enable PLL.
	RCC.PLLCFGR = 0x20400000 | ((pll_mhz / 48) << 24) | (pllp << 16) | (pll_mhz << 6) | osc_mhz;
	RCC.CR |= 0x1000000;
	while(!(RCC.CR & 0x2000000));
	
	// Switch to PLL.
	RCC.CFGR |= 0x2;
	while(!(RCC.CFGR & 0x8));
	
	if(sysclk_mhz > 84) {
		// Set APB1 prescaler to /4.
		RCC.CFGR |= 5 << 10; // PPRE1
		
		// Set APB2 prescaler to /2.
		RCC.CFGR |= 4 << 13; // PPRE2
	} else {
		// Set APB1 prescaler to /2.
		RCC.CFGR |= 4 << 10; // PPRE1
		
		// Set APB2 prescaler to /1.
		RCC.CFGR |= 0 << 13; // PPRE2
	}
	
}
#endif

