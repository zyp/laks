#pragma once

#include <mmio/mmio.h>

struct STM32_GPIO_reg_v1_t {
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
};

struct STM32_GPIO_reg_v2_t {
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
};

class STM32_GPIO_v1_t : public mmio_ptr<STM32_GPIO_reg_v1_t> {
	public:
		using mmio_ptr<STM32_GPIO_reg_v1_t>::ptr;
		
		class Pin {
			private:
				const STM32_GPIO_v1_t& g;
				int n;
			
			public:
				constexpr Pin(const STM32_GPIO_v1_t& gpio, int pin) : g(gpio), n(pin) {}
				
				enum Mode {
					Input = 0x4,
					InputPull = 0x8,
					Output = 0x3,
					AF = 0xb,
					Analog = 0x0,
				};
				
				void set_mode(Mode m) {
					if(n < 8) {
						g->CRL = (g->CRL & ~(0xf << (n * 4))) | m << (n * 4);
					} else {
						g->CRH = (g->CRH & ~(0xf << (n * 4 - 32))) | m << (n * 4 - 32);
					}
				}
				
				void on() {
					g->BSRR = 1 << n;
				}
				
				void off() {
					g->BSRR = 1 << 16 << n;
				}
				
				void set(bool value) {
					if(value) {
						on();
					} else {
						off();
					}
				}
				
				bool get() {
					return g->IDR & (1 << n);
				}
				
				void toggle() {
					set(!(g->ODR & (1 << n)));
				}
		};
		
		class PinArray {
			private:
				const STM32_GPIO_v1_t& g;
				int f;
				int l;
				
				constexpr uint32_t mask1() {
					return ((2 << l) - 1) ^ ((1 << f) - 1);
				}
				
				constexpr uint32_t mask2() {
					return ((4 << (l * 2)) - 1) ^ ((1 << (f * 2)) - 1);
				}
			
			public:
				constexpr PinArray(const STM32_GPIO_v1_t& gpio, int first, int last) : g(gpio), f(first), l(last) {}
				
				void set(uint16_t value) {
					value <<= f;
					g->BSRR = ((~value & mask1()) << 16) | (value & mask1());
				}
				
				uint16_t get() {
					return (g->IDR & mask1()) >> f;
				}
		};
		
		constexpr Pin operator[](int pin) const {
			return Pin(*this, pin);
		}
		
		constexpr PinArray array(int first, int last) const {
			return PinArray(*this, first, last);
		}
};

class STM32_GPIO_v2_t : public mmio_ptr<STM32_GPIO_reg_v2_t> {
	public:
		using mmio_ptr<STM32_GPIO_reg_v2_t>::ptr;
		
		class Pin {
			private:
				const STM32_GPIO_v2_t& g;
				int n;
			
			public:
				constexpr Pin(const STM32_GPIO_v2_t& gpio, int pin) : g(gpio), n(pin) {}
				
				enum Mode {
					Input,
					Output,
					AF,
					Analog,
				};
				
				enum Type {
					PushPull,
					OpenDrain,
				};
				
				enum Pull {
					PullNone,
					PullUp,
					PullDown,
				};
				
				enum Speed {
					Low,
					Medium,
					Fast,
					High,
				};
				
				void set_mode(Mode m) {
					g->MODER = (g->MODER & ~(3 << (n * 2))) | m << (n * 2);
				}
				
				void set_type(Type t) {
					if(t) {
						g->OTYPER |= 1 << n;
					} else {
						g->OTYPER &= ~(1 << n);
					}
				}
				
				void set_pull(Pull p) {
					g->PUPDR = (g->PUPDR & ~(3 << (n * 2))) | p << (n * 2);
				}
				
				void set_af(int af) {
					if(n < 8) {
						g->AFRL = (g->AFRL & ~(0xf << (n * 4))) | af << (n * 4);
					} else {
						g->AFRH = (g->AFRH & ~(0xf << (n * 4 - 32))) | af << (n * 4 - 32);
					}
				}
				
				void set_speed(Speed s) {
					g->OSPEEDR = (g->OSPEEDR & ~(3 << (n * 2))) | s << (n * 2);
				}
				
				void on() {
					g->BSRR = 1 << n;
				}
				
				void off() {
					g->BSRR = 1 << 16 << n;
				}
				
				void set(bool value) {
					if(value) {
						on();
					} else {
						off();
					}
				}
				
				bool get() {
					return g->IDR & (1 << n);
				}
				
				void toggle() {
					set(!(g->ODR & (1 << n)));
				}
		};
		
		class PinArray {
			private:
				const STM32_GPIO_v2_t& g;
				int f;
				int l;
				
				constexpr uint32_t mask1() {
					return ((2 << l) - 1) ^ ((1 << f) - 1);
				}
				
				constexpr uint32_t mask2() {
					return ((4 << (l * 2)) - 1) ^ ((1 << (f * 2)) - 1);
				}
			
			public:
				constexpr PinArray(const STM32_GPIO_v2_t& gpio, int first, int last) : g(gpio), f(first), l(last) {}
				
				void set_mode(Pin::Mode m) {
					g->MODER = (g->MODER & ~mask2()) | ((0x55555555 * m) & mask2());
				}
				
				void set_type(Pin::Type t) {
					g->OTYPER = (g->OTYPER & ~mask2()) | ((0x55555555 * t) & mask2());
				}
				
				void set_pull(Pin::Pull p) {
					g->PUPDR = (g->PUPDR & ~mask2()) | ((0x55555555 * p) & mask2());
				}
				
				void set(uint16_t value) {
					value <<= f;
					g->BSRR = ((~value & mask1()) << 16) | (value & mask1());
				}
				
				uint16_t get() {
					return (g->IDR & mask1()) >> f;
				}
		};
		
		constexpr Pin operator[](int pin) const {
			return Pin(*this, pin);
		}
		
		constexpr PinArray array(int first, int last) const {
			return PinArray(*this, first, last);
		}
};
