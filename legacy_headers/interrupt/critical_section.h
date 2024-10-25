#pragma once

#include <cstdint>

#ifdef CORTEX_M
struct critical_section {
    uint32_t primask;

    critical_section() {
        asm volatile("mrs %0, primask" : "=r" (primask));

        asm volatile("cpsid i");

        asm volatile("dmb");
    }

    ~critical_section() {
        asm volatile("dmb");

        asm volatile("msr primask, %0" :: "r" (primask));
    }
};
#endif

#ifdef RISCV
struct critical_section {
    uint32_t mie;

    critical_section() {
        asm volatile("csrrw %0, mie, x0" : "=r" (mie));
    }

    ~critical_section() {
        asm volatile("csrw mie, %0" :: "r" (mie));
    }
};
#endif
