#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

struct GPIO_reg_t {
	#if defined(STM32F1)
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
	#elif defined(STM32F4)
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
	#endif
};

class GPIO_t {
	public:
		GPIO_reg_t& reg;
		
		constexpr GPIO_t(uint32_t reg_addr) : reg(*(GPIO_reg_t*)reg_addr) {}
};

#if defined(STM32F1)
static GPIO_t GPIOA(0x40010800);
static GPIO_t GPIOB(0x40010c00);
static GPIO_t GPIOC(0x40011000);
static GPIO_t GPIOD(0x40011400);
#elif defined(STM32F4)
static GPIO_t GPIOA(0x40020000);
static GPIO_t GPIOB(0x40020400);
static GPIO_t GPIOC(0x40020800);
static GPIO_t GPIOD(0x40020c00);
static GPIO_t GPIOE(0x40021000);
static GPIO_t GPIOF(0x40021400);
static GPIO_t GPIOG(0x40021800);
static GPIO_t GPIOH(0x40021c00);
static GPIO_t GPIOI(0x40022000);
#endif

#endif
