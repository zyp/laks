#include "flash.h"

void flash_init() {
	#if defined(STM32F1)
	
	// Set flash latency.
	FLASH.ACR = 0x12;
	
	#elif defined(STM32F4)
	
	// Set flash latency.
	FLASH.ACR = 0x105;
	
	while(FLASH.ACR != 0x105);
	
	#endif
}
