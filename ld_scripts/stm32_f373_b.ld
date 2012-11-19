MEMORY {
	flash (rx) : org = 0x08000000, len = 128k
	ram (rwx)  : org = 0x20000000, len = 24k
}

INCLUDE "arm_flash_ram.ld"
