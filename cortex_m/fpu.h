#ifndef FPU_H
#define FPU_H

#include <stdint.h>

struct COPROC_t {
	volatile uint32_t CPAC;
};

static COPROC_t& COPROC = *(COPROC_t*)0xe000ed88;

struct FPU_t {
	volatile uint32_t FPCC;
	volatile uint32_t FPCA;
	volatile uint32_t FPDSC;
};

static FPU_t& FPU = *(FPU_t*)0xe000ef34;

#endif
