#include "flash.h"

/**
 * Configure the flash for maximum speed access.
 */
void flash_init() {
	#if defined(STM32F1) || defined(STM32F3)
	
	// Set flash latency.
	FLASH->ACR = 0x12;
	
	#elif defined(STM32F4) || defined(STM32F7)
	
	// Set flash latency.
	FLASH->ACR = 0x107;
	
	while(FLASH->ACR != 0x107);
	
	#elif defined(STM32F0) || defined(STM32L0)
	
	// SET flash latency.
	FLASH->ACR = 1 << 0;
	
	#elif defined(STM32WB)
	// Prefetch and both caches, plus 3WS for 64MHz
	FLASH->ACR = 0x700 | 3;
	
	#endif
}
