#ifndef USART_H
#define USART_H

#include <rcc/rcc.h>
#include <interrupt/interrupt.h>
#include <os/thread.h>

#if defined(STM32F1) || defined(STM32F4)
struct USART_reg_t {
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
};
#elif defined(STM32F3) || defined(STM32L0)
struct USART_reg_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t BRR;
	volatile uint32_t GTPR;
	volatile uint32_t RTOR;
	volatile uint32_t RQR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t RDR;
	volatile uint32_t TDR;
};
#endif

class USART_t {
	public:
		USART_reg_t& reg;
		const uint32_t clk;
		
		USART_t(uint32_t reg_addr, uint32_t bus_clk) : reg(*(USART_reg_t*)reg_addr), clk(bus_clk) {}
		
		inline void set_baudrate(uint32_t baudrate) {
			reg.BRR = clk / baudrate;
		}
};

#if defined(STM32F1) || defined(STM32F3)
static USART_t USART1(0x40013800, 72000000);
static USART_t USART2(0x40004400, 36000000);
static USART_t USART3(0x40004800, 36000000);
#elif defined(STM32F4)
static USART_t USART1(0x40011000, 84000000);
static USART_t USART2(0x40004400, 42000000);
static USART_t USART3(0x40004800, 42000000);
#elif defined(STM32L0)
static USART_t USART1(0x40013800, 32000000);
static USART_t USART2(0x40004400, 32000000);
#endif

#endif
