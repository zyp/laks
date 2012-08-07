MEMORY {
	flash (rx) : org = 0x08000000, len = 1024k
	ram (rwx)  : org = 0x20000000, len = 128k
	ccm (rwx)  : org = 0x10000000, len = 64k
}

INCLUDE "arm_flash_ram.ld"
