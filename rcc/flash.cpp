#include "flash.h"

void flash_init() {
	#if defined(STM32F1) || defined(STM32F3)
	
	// Set flash latency.
	FLASH.ACR = 0x12;
	
	#elif defined(STM32F4)
	
	// Set flash latency.
	FLASH.ACR = 0x105;
	
	while(FLASH.ACR != 0x105);
	
	#elif defined(STM32L0)
	
	// SET flash latency.
	FLASH.ACR = 1 << 0;
	
	#endif
}
