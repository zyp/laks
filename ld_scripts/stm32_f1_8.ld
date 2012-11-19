MEMORY {
	flash (rx) : org = 0x08000000, len = 64k
	ram (rwx)  : org = 0x20000000, len = 20k
}

INCLUDE "arm_flash_ram.ld"
