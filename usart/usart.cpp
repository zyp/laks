#include "usart.h"

template<>
void interrupt<Interrupt::USART1>() {
	USART1.recv();
	//GPIOB.ODR ^= 1 << 1;
}
