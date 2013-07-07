#if defined(STM32F1) || defined(STM32F3) || defined(STM32F4)

#include "usart.h"

template<>
void interrupt<Interrupt::USART1>() {
	USART1.recv();
	//GPIOB.ODR ^= 1 << 1;
}

#endif
