/* Linker script for qemu-system-arm -machine lm3s6965evb */

MEMORY {
	flash (rx) : org = 0x00000000, len = 256k
	ram (rwx)  : org = 0x20000000, len = 64k
}

INCLUDE "arm_flash_ram.ld"
