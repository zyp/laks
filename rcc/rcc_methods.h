#pragma once

#include <stdint.h>

void rcc_init();
void rcc_init(uint32_t osc_mhz, uint32_t sysclk_mhz);
