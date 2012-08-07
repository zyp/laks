#ifndef USART_H
#define USART_H

#include <rcc/rcc.h>
#include <interrupt/interrupt.h>
#include <os/thread.h>

struct USART_reg_t {
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
};

class USART_t {
	public:
		USART_reg_t& reg;
		const uint32_t clk;
		
		USART_t(uint32_t reg_addr, uint32_t bus_clk) : reg(*(USART_reg_t*)reg_addr), clk(bus_clk) {}
		
		inline void set_baudrate(uint32_t baudrate) {
			reg.BRR = clk / baudrate;
		}
		
		inline void enable() {
			reg.CR1 = 0x202c;
		}
		
		inline void send(uint8_t data) {
			while(!(reg.SR & 0x80)) {
				Thread::yield();
			} // Wait for TXE.
			
			reg.DR = data;
		}
		
		inline uint8_t recv() {
			return reg.DR;
		}
};

#if defined(STM32F1)
static USART_t USART1(0x40013800, 72000000);
static USART_t USART2(0x40004400, 36000000);
static USART_t USART3(0x40004800, 36000000);
#elif defined(STM32F4)
static USART_t USART1(0x40011000, 84000000);
static USART_t USART2(0x40004400, 42000000);
static USART_t USART3(0x40004800, 42000000);
#endif

inline void usart_enable() {
	RCC.enable(RCC.USART1);
	USART1.set_baudrate(115200);
	USART1.enable();
	
	//Interrupt::enable(Interrupt::USART1);
}

inline void usart_send(uint8_t data) {
	USART1.send(data);
}

#endif
