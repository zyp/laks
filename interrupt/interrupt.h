#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

struct NVIC_t {
	volatile uint32_t ISER[32];
	volatile uint32_t ICER[32];
	volatile uint32_t ISPR[32];
	volatile uint32_t ICPR[32];
	volatile uint32_t IABR[64];
	volatile uint8_t IPR[2816];
	volatile uint32_t STIR;
};

static NVIC_t& NVIC = *(NVIC_t*)0xe000e100;

struct SCB_t {
	volatile uint32_t CPUID;
	volatile uint32_t ICSR;
	volatile uint32_t VTOR;
	volatile uint32_t AIRCR;
	volatile uint32_t SCR;
	volatile uint32_t CCR;
	volatile uint8_t SHPR[12];
	volatile uint32_t SHCSR;
	volatile uint32_t CFSR;
	volatile uint32_t HFSR;
	volatile uint32_t DFSR;
	volatile uint32_t MMAR;
	volatile uint32_t BFAR;
};

static SCB_t& SCB = *(SCB_t*)0xe000ed00;

namespace Interrupt {
	enum Exception {
		NMI        = 2,
		HardFault  = 3,
		MemManage  = 4,
		BusFault   = 5,
		UsageFault = 6,
		SVCall     = 11,
		PendSV     = 14,
		SysTick    = 15
	};
	
	enum IRQ {
		WWDG,
		PVD,
		TAMPER,
		RTC,
		FLASH,
		RCC,
		EXTI0,
		EXTI1,
		EXTI2,
		EXTI3,
		EXTI4,
		DMA1_Channel1,
		DMA1_Channel2,
		DMA1_Channel3,
		DMA1_Channel4,
		DMA1_Channel5,
		DMA1_Channel6,
		DMA1_Channel7,
		ADC1_2,
		USB_HP_CAN_TX,
		USB_LP_CAN_RX0,
		CAN_RX1,
		CAN_SCE,
		EXTI9_5,
		TIM1_BRK,
		TIM1_UP,
		TIM1_TRG_COM,
		TIM1_CC,
		TIM2,
		TIM3,
		TIM4,
		I2C1_EV,
		I2C1_ER,
		I2C2_EV,
		I2C2_ER,
		SPI1,
		SPI2,
		USART1,
		USART2,
		USART3,
		EXTI15_10,
		RTCAlarm,
		USBWakeup,
		TIM8_BRK,
		TIM8_UP,
		TIM8_TRG_COM,
		TIM8_CC,
		ADC3,
		FSMC,
		SDIO,
		TIM5,
		SPI3,
		UART4,
		UART5,
		TIM6,
		TIM7,
		DMA2_Channel1,
		DMA2_Channel2,
		DMA2_Channel3,
		DMA2_Channel4_5,
		NUM_IRQs
	};
	
	inline void enable(IRQ n) {
		NVIC.ISER[n >> 5] = 1 << (n & 0x1f);
	}
	
	inline void set_priority(Exception n, uint8_t priority) {
		SCB.SHPR[n - 4] = priority;
	}
	
	inline void set_priority(IRQ n, uint8_t priority) {
		NVIC.IPR[n] = priority;
	}
	
	struct MFP {
		void (*func_p)(void*);
		void* instance_p;
	};
	
	extern MFP mf_vectors[];
	
	template<class T>
	inline void set_handler(IRQ n, void (T::*f)(), T* i) {
		MFP& mfp = mf_vectors[16 + n];
		mfp.func_p = reinterpret_cast<void (*)(void*)>(f);
		mfp.instance_p = i;
	}
	
	template<class T>
	inline void enable(IRQ n, void (T::*f)(), T* i) {
		set_handler(n, f, i);
		enable(n);
	}
};

template<Interrupt::Exception>
void interrupt();

template<Interrupt::IRQ>
void interrupt();

#endif
