#ifndef TIMER_H
#define TIMER_H

struct TIM_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t RCR;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t BDTR;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
};

#if defined(STM32F1) || defined(STM32F3)
static TIM_t& TIM1 = *(TIM_t*)0x40012c00;
static TIM_t& TIM2 = *(TIM_t*)0x40000000;
static TIM_t& TIM3 = *(TIM_t*)0x40000400;
static TIM_t& TIM4 = *(TIM_t*)0x40000800;
static TIM_t& TIM5 = *(TIM_t*)0x40000c00;
static TIM_t& TIM6 = *(TIM_t*)0x40001000;
static TIM_t& TIM7 = *(TIM_t*)0x40001400;
static TIM_t& TIM8 = *(TIM_t*)0x40013400;
#elif defined(STM32F4)
static TIM_t& TIM1 = *(TIM_t*)0x40010000;
static TIM_t& TIM2 = *(TIM_t*)0x40000000;
static TIM_t& TIM3 = *(TIM_t*)0x40000400;
static TIM_t& TIM4 = *(TIM_t*)0x40000800;
static TIM_t& TIM5 = *(TIM_t*)0x40000c00;
static TIM_t& TIM6 = *(TIM_t*)0x40001000;
static TIM_t& TIM7 = *(TIM_t*)0x40001400;
static TIM_t& TIM8 = *(TIM_t*)0x40010400;
#endif

#endif
