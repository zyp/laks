#ifndef SEMIHOSTING_H
#define SEMIHOSTING_H

#include <stdint.h>

inline uint32_t semihosting_call(uint32_t num, const uint32_t* args = nullptr) {
	register uint32_t a asm("r0") = num;
	register uint32_t b asm("r1") = (uint32_t)args;
	
	asm volatile("bkpt 0xab" : "=r"(a) : "0"(a), "r"(b) : "memory");
	
	return a;
}

inline void semihosting_print(const void *buf) {
	semihosting_call(0x04, (const uint32_t*)buf);
}

inline void semihosting_exit() {
	semihosting_call(0x18);
}

#endif
