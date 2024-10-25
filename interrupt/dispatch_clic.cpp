#include <cstdint>

import laks.periph.interrupt;
import laks.periph.interrupt.clic;
import laks.periph.interrupt.enums;

extern interrupt::vector_t vectors_exception[]; 
extern interrupt::vector_t vectors_clic[]; 

[[gnu::interrupt, gnu::aligned(64)]]
void riscv_interrupt_handler() {
    uint32_t cause;
    asm("csrr %0, mcause" : "=r"(cause));

    uint32_t type = cause & 0x80000000;
    uint32_t code = cause & 0x00000fff;

    if(type) {
        vectors_clic[code]();
    } else {
        vectors_exception[code]();
    }
}

[[gnu::constructor(200)]]
void riscv_interrupt_init() {
    asm volatile("csrw mtvec, %0" :: "r"(intptr_t(riscv_interrupt_handler) | 3));
    asm volatile("csrrsi x0, mstatus, (1 << 3)");
}
