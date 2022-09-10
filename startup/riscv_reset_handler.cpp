void entry();
extern int _ram_end;

[[gnu::naked]]
[[gnu::section(".vectors")]]
void _reset_handler() {
    // Initialize stack pointer.
    asm volatile("lui sp, %%hi(%0); add sp, sp, %%lo(%0)" :: "i"(&_ram_end));
    // Absolute jump to entry function.
    asm volatile("jr %0" :: "m"(entry));
}
