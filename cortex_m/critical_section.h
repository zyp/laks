#pragma once

#include <cstdint>

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
