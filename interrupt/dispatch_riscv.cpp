#include "interrupt.h"

extern interrupt::vector_t vectors_exception[]; 
extern interrupt::vector_t vectors_internal[]; 

[[gnu::interrupt]]
void riscv_interrupt_handler() {
    uint32_t cause;
    asm("csrr %0, mcause" : "=r"(cause));

    uint32_t type = cause & 0x80000000;
    uint32_t code = cause & 0x7fffffff;

    if(type) {
        vectors_internal[code]();
    } else {
        vectors_exception[code]();
    }
}

[[gnu::constructor(200)]]
void riscv_interrupt_init() {
    asm volatile("csrw mtvec, %0" :: "r"(riscv_interrupt_handler));
}
