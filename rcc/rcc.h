#ifndef RCC_H
#define RCC_H

#include <stdint.h>

struct RCC_t {
	#if defined(STM32F1)
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	#elif defined(STM32F3)
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t AHBRSTR;
	volatile uint32_t CFGR2;
	volatile uint32_t CFGR3;
	#elif defined(STM32F4)
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	volatile uint32_t AHB4RSTR; // Reserved
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB3RSTR; // Reserved
	volatile uint32_t APB4RSTR; // Reserved
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t AHB4ENR; // Reserved
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB3ENR; // Reserved
	volatile uint32_t APB4ENR; // Reserved
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	volatile uint32_t AHB4LPENR; // Reserved
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t APB3LPENR; // Reserved
	volatile uint32_t APB4LPENR; // Reserved
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t _1;
	volatile uint32_t _2;
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	#elif defined(STM32L0)
	volatile uint32_t CR;
	volatile uint32_t ICSCR;
	volatile uint32_t CRRCR;
	volatile uint32_t CFGR;
	volatile uint32_t CIER;
	volatile uint32_t CIFR;
	volatile uint32_t CICR;
	volatile uint32_t IOPRSTR;
	volatile uint32_t AHBRSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t IOPENR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t IOPSMENR;
	volatile uint32_t AHBSMENR;
	volatile uint32_t APB2SMENR;
	volatile uint32_t APB1SMENR;
	volatile uint32_t CCIPR;
	volatile uint32_t CSR;
	#endif
	
	#if defined(STM32F1)
	enum AHB_dev {
		DMA1  = 1 << 0,
		DMA2  = 1 << 1,
		SRAM  = 1 << 2,
		FLITF = 1 << 4,
		CRC   = 1 << 6,
		FSMC  = 1 << 8,
		SDIO  = 1 << 10
	};
	
	enum APB1_dev {
		TIM2   = 1 << 0,
		TIM3   = 1 << 1,
		TIM4   = 1 << 2,
		TIM5   = 1 << 3,
		TIM6   = 1 << 4,
		TIM7   = 1 << 5,
		TIM12  = 1 << 6,
		TIM13  = 1 << 7,
		TIM14  = 1 << 8,
		WWDG   = 1 << 11,
		SPI2   = 1 << 14,
		SPI3   = 1 << 15,
		USART2 = 1 << 17,
		USART3 = 1 << 18,
		UART4  = 1 << 19,
		UART5  = 1 << 20,
		I2C1   = 1 << 21,
		I2C2   = 1 << 22,
		USB    = 1 << 23,
		CAN    = 1 << 25,
		BKP    = 1 << 27,
		PWR    = 1 << 28,
		DAC    = 1 << 29
	};
	
	enum APB2_dev {
		AFIO   = 1 << 0,
		GPIOA  = 1 << 2,
		GPIOB  = 1 << 3,
		GPIOC  = 1 << 4,
		GPIOD  = 1 << 5,
		GPIOE  = 1 << 6,
		GPIOF  = 1 << 7,
		GPIOG  = 1 << 8,
		ADC1   = 1 << 9,
		ADC2   = 1 << 10,
		TIM1   = 1 << 11,
		SPI1   = 1 << 12,
		TIM8   = 1 << 13,
		USART1 = 1 << 14,
		ADC3   = 1 << 15,
		TIM9   = 1 << 19,
		TIM10  = 1 << 20,
		TIM11  = 1 << 21
	};
	#elif defined(STM32F3)
	enum AHB_dev {
		DMA1  = 1 << 0,
		DMA2  = 1 << 1,
		SRAM  = 1 << 2,
		FLITF = 1 << 4,
		CRC   = 1 << 6,
		GPIOA = 1 << 17,
		GPIOB = 1 << 18,
		GPIOC = 1 << 19,
		GPIOD = 1 << 20,
		GPIOE = 1 << 21,
		GPIOF = 1 << 22,
		TSC   = 1 << 24,
		ADC12 = 1 << 28,
		ADC34 = 1 << 29,
	};
	
	enum APB1_dev {
		TIM2   = 1 << 0,
		TIM3   = 1 << 1,
		TIM4   = 1 << 2,
		TIM6   = 1 << 4,
		TIM7   = 1 << 5,
		WWDG   = 1 << 11,
		SPI2   = 1 << 14,
		SPI3   = 1 << 15,
		USART2 = 1 << 17,
		USART3 = 1 << 18,
		UART4  = 1 << 19,
		UART5  = 1 << 20,
		I2C1   = 1 << 21,
		I2C2   = 1 << 22,
		USB    = 1 << 23,
		CAN    = 1 << 25,
		PWR    = 1 << 28,
		DAC    = 1 << 29,
	};
	
	enum APB2_dev {
		SYSCFG = 1 << 0,
		TIM1   = 1 << 11,
		SPI1   = 1 << 12,
		TIM8   = 1 << 13,
		USART1 = 1 << 14,
		TIM15  = 1 << 16,
		TIM16  = 1 << 17,
		TIM17  = 1 << 18,
	};
	#elif defined(STM32F4)
	enum AHB1_dev {
		GPIOA     = 1 << 0,
		GPIOB     = 1 << 1,
		GPIOC     = 1 << 2,
		GPIOD     = 1 << 3,
		GPIOE     = 1 << 4,
		GPIOF     = 1 << 5,
		GPIOG     = 1 << 6,
		GPIOH     = 1 << 7,
		GPIOI     = 1 << 8,
		CRC       = 1 << 12,
		DMA1      = 1 << 21,
		DMA2      = 1 << 22,
		ETHMAC    = 1 << 25,
		ETHMACTX  = 1 << 26,
		ETHMACRX  = 1 << 27,
		ETHMACPTP = 1 << 28,
		OTGHS     = 1 << 29,
		OTGHSULPI = 1 << 30,
	};
	
	enum AHB2_dev {
		DCMI  = 1 << 0,
		CRYP  = 1 << 4,
		HASH  = 1 << 5,
		RNG   = 1 << 6,
		OTGFS = 1 << 7,
	};
	
	enum AHB3_dev {
		FSMC  = 1 << 0,
	};
	
	enum APB1_dev {
		TIM2   = 1 << 0,
		TIM3   = 1 << 1,
		TIM4   = 1 << 2,
		TIM5   = 1 << 3,
		TIM6   = 1 << 4,
		TIM7   = 1 << 5,
		TIM12  = 1 << 6,
		TIM13  = 1 << 7,
		TIM14  = 1 << 8,
		WWDG   = 1 << 11,
		SPI2   = 1 << 14,
		SPI3   = 1 << 15,
		USART2 = 1 << 17,
		USART3 = 1 << 18,
		UART4  = 1 << 19,
		UART5  = 1 << 20,
		I2C1   = 1 << 21,
		I2C2   = 1 << 22,
		I2C3   = 1 << 23,
		CAN1   = 1 << 25,
		CAN2   = 1 << 26,
		PWR    = 1 << 28,
		DAC    = 1 << 29,
	};
	
	enum APB2_dev {
		TIM1   = 1 << 0,
		TIM8   = 1 << 1,
		USART1 = 1 << 4,
		USART6 = 1 << 5,
		ADC    = 1 << 8,
		SDIO   = 1 << 11,
		SPI1   = 1 << 12,
		SYSCFG = 1 << 14,
		TIM9   = 1 << 16,
		TIM10  = 1 << 17,
		TIM11  = 1 << 18,
	};
	#elif defined(STM32L0)
	enum AHB_dev {
		DMA   = 1 << 0,
		MIF   = 1 << 8,
		CRC   = 1 << 12,
		TOUCH = 1 << 16,
		RNG   = 1 << 20,
		CRYP  = 1 << 24,
	};
	
	enum APB1_dev {
		TIM2    = 1 << 0,
		TIM6    = 1 << 4,
		WWDG    = 1 << 11,
		SPI2    = 1 << 14,
		USART2  = 1 << 17,
		LPUART1 = 1 << 18,
		I2C1    = 1 << 21,
		I2C2    = 1 << 22,
		USB     = 1 << 23,
		CRS     = 1 << 27,
		PWR     = 1 << 28,
		DAC     = 1 << 29,
		LPTIM1  = 1 << 31,
	};
	
	enum APB2_dev {
		SYSCFG = 1 << 0,
		TIM21  = 1 << 2,
		TIM22  = 1 << 5,
		MIFI   = 1 << 7,
		ADC    = 1 << 9,
		SPI1   = 1 << 12,
		USART1 = 1 << 14,
		DBG    = 1 << 22,
	};
	
	enum IOP_dev {
		GPIOA  = 1 << 0,
		GPIOB  = 1 << 1,
		GPIOC  = 1 << 2,
		GPIOD  = 1 << 3,
		GPIOH  = 1 << 7,
	};
	#endif
	
	#if defined(STM32F1) || defined(STM32F3) || defined(STM32L0)
	inline void enable(AHB_dev dev) {
		AHBENR |= dev;
	}
	#elif defined(STM32F4)
	inline void enable(AHB1_dev dev) {
		AHB1ENR |= dev;
	}
	
	inline void enable(AHB2_dev dev) {
		AHB2ENR |= dev;
	}
	
	inline void enable(AHB3_dev dev) {
		AHB3ENR |= dev;
	}
	#endif
	inline void enable(APB1_dev dev) {
		APB1ENR |= dev;
	}
	
	inline void enable(APB2_dev dev) {
		APB2ENR |= dev;
	}
	#if defined(STM32L0)
	inline void enable(IOP_dev dev) {
		IOPENR |= dev;
	}
	#endif
};

#if defined(STM32F1) || defined(STM32F3) || defined(STM32L0)
static RCC_t& RCC = *(RCC_t*)0x40021000;
#elif defined(STM32F4)
static RCC_t& RCC = *(RCC_t*)0x40023800;
#endif

void rcc_init();

#if defined(STM32F4)
void rcc_init(uint32_t osc_mhz, uint32_t sysclk_mhz);
#endif


#endif
