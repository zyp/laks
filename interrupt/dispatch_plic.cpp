#include "interrupt.h"

extern interrupt::vector_t vectors_irq[]; 

template<>
void interrupt::handler<interrupt::internal::MEI>() {
    while(1) {}
}
