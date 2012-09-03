#include "interrupt.h"

namespace Interrupt {
	MFP mf_vectors[16 + NUM_IRQs];
};

void entry();

void member_function_interrupt_gate() {
	uint32_t interrupt_num;
	asm ("mrs %0, ipsr" : "=r" (interrupt_num));
	
	Interrupt::mf_vectors[interrupt_num].func_p(Interrupt::mf_vectors[interrupt_num].instance_p);
}

extern "C" void unused_interrupt() {
	member_function_interrupt_gate();
	//while(1);
}

template<> void interrupt<Interrupt::NMI>()             __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::HardFault>()       __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::MemManage>()       __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::BusFault>()        __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::UsageFault>()      __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::SVCall>()          __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::PendSV>()          __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::SysTick>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::WWDG>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::PVD>()             __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TAMPER>()          __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::RTC>()             __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::FLASH>()           __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::RCC>()             __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::EXTI0>()           __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::EXTI1>()           __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::EXTI2>()           __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::EXTI3>()           __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::EXTI4>()           __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA1_Channel1>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA1_Channel2>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA1_Channel3>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA1_Channel4>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA1_Channel5>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA1_Channel6>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA1_Channel7>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::ADC1_2>()          __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::USB_HP_CAN_TX>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::USB_LP_CAN_RX0>()  __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::CAN_RX1>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::CAN_SCE>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::EXTI9_5>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM1_BRK>()        __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM1_UP>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM1_TRG_COM>()    __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM1_CC>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM2>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM3>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM4>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::I2C1_EV>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::I2C1_ER>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::I2C2_EV>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::I2C2_ER>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::SPI1>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::SPI2>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::USART1>()          __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::USART2>()          __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::USART3>()          __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::EXTI15_10>()       __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::RTCAlarm>()        __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::USBWakeup>()       __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM8_BRK>()        __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM8_UP>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM8_TRG_COM>()    __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM8_CC>()         __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::ADC3>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::FSMC>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::SDIO>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM5>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::SPI3>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::UART4>()           __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::UART5>()           __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM6>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::TIM7>()            __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA2_Channel1>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA2_Channel2>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA2_Channel3>()   __attribute__ ((weak, alias ("unused_interrupt")));
template<> void interrupt<Interrupt::DMA2_Channel4_5>() __attribute__ ((weak, alias ("unused_interrupt")));

typedef void (*vector_t)();

vector_t vectors[] __attribute__((section(".vectors"))) = {
	(vector_t)0x20004ffc,
	entry,
	interrupt<Interrupt::NMI>,
	interrupt<Interrupt::HardFault>,
	interrupt<Interrupt::MemManage>,
	interrupt<Interrupt::BusFault>,
	interrupt<Interrupt::UsageFault>,
	0,
	0,
	0,
	0,
	interrupt<Interrupt::SVCall>,
	0,
	0,
	interrupt<Interrupt::PendSV>,
	interrupt<Interrupt::SysTick>,
	interrupt<Interrupt::WWDG>,
	interrupt<Interrupt::PVD>,
	interrupt<Interrupt::TAMPER>,
	interrupt<Interrupt::RTC>,
	interrupt<Interrupt::FLASH>,
	interrupt<Interrupt::RCC>,
	interrupt<Interrupt::EXTI0>,
	interrupt<Interrupt::EXTI1>,
	interrupt<Interrupt::EXTI2>,
	interrupt<Interrupt::EXTI3>,
	interrupt<Interrupt::EXTI4>,
	interrupt<Interrupt::DMA1_Channel1>,
	interrupt<Interrupt::DMA1_Channel2>,
	interrupt<Interrupt::DMA1_Channel3>,
	interrupt<Interrupt::DMA1_Channel4>,
	interrupt<Interrupt::DMA1_Channel5>,
	interrupt<Interrupt::DMA1_Channel6>,
	interrupt<Interrupt::DMA1_Channel7>,
	interrupt<Interrupt::ADC1_2>,
	interrupt<Interrupt::USB_HP_CAN_TX>,
	interrupt<Interrupt::USB_LP_CAN_RX0>,
	interrupt<Interrupt::CAN_RX1>,
	interrupt<Interrupt::CAN_SCE>,
	interrupt<Interrupt::EXTI9_5>,
	interrupt<Interrupt::TIM1_BRK>,
	interrupt<Interrupt::TIM1_UP>,
	interrupt<Interrupt::TIM1_TRG_COM>,
	interrupt<Interrupt::TIM1_CC>,
	interrupt<Interrupt::TIM2>,
	interrupt<Interrupt::TIM3>,
	interrupt<Interrupt::TIM4>,
	interrupt<Interrupt::I2C1_EV>,
	interrupt<Interrupt::I2C1_ER>,
	interrupt<Interrupt::I2C2_EV>,
	interrupt<Interrupt::I2C2_ER>,
	interrupt<Interrupt::SPI1>,
	interrupt<Interrupt::SPI2>,
	interrupt<Interrupt::USART1>,
	interrupt<Interrupt::USART2>,
	interrupt<Interrupt::USART3>,
	interrupt<Interrupt::EXTI15_10>,
	interrupt<Interrupt::RTCAlarm>,
	interrupt<Interrupt::USBWakeup>,
	interrupt<Interrupt::TIM8_BRK>,
	interrupt<Interrupt::TIM8_UP>,
	interrupt<Interrupt::TIM8_TRG_COM>,
	interrupt<Interrupt::TIM8_CC>,
	interrupt<Interrupt::ADC3>,
	interrupt<Interrupt::FSMC>,
	interrupt<Interrupt::SDIO>,
	interrupt<Interrupt::TIM5>,
	interrupt<Interrupt::SPI3>,
	interrupt<Interrupt::UART4>,
	interrupt<Interrupt::UART5>,
	interrupt<Interrupt::TIM6>,
	interrupt<Interrupt::TIM7>,
	interrupt<Interrupt::DMA2_Channel1>,
	interrupt<Interrupt::DMA2_Channel2>,
	interrupt<Interrupt::DMA2_Channel3>,
	interrupt<Interrupt::DMA2_Channel4_5>,
};
