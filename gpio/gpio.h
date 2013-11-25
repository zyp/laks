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
	#elif defined(STM32F3) || defined(STM32F4)
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
		
		class Pin {
			private:
				const GPIO_t& g;
				int n;
			
			public:
				constexpr Pin(const GPIO_t& gpio, int pin) : g(gpio), n(pin) {}
				
				enum Mode {
					#if defined(STM32F1)
					Input = 0x4,
					Output = 0x3,
					AF = 0xb,
					Analog = 0x0,
					#elif defined(STM32F3) || defined(STM32F4)
					Input,
					Output,
					AF,
					Analog,
					#endif
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
					#if defined(STM32F1)
					if(n < 8) {
						g.reg.CRL = (g.reg.CRL & ~(0xf << (n * 4))) | m << (n * 4);
					} else {
						g.reg.CRH = (g.reg.CRH & ~(0xf << (n * 4 - 32))) | m << (n * 4 - 32);
					}
					#elif defined(STM32F3) || defined(STM32F4)
					g.reg.MODER = (g.reg.MODER & ~(3 << (n * 2))) | m << (n * 2);
					#endif
				}
				
				void set_type(Type t) {
					#if defined(STM32F1)
					// TODO: Unified configure() method?
					#elif defined(STM32F3) || defined(STM32F4)
					if(t) {
						g.reg.OTYPER |= 1 << n;
					} else {
						g.reg.OTYPER &= ~(1 << n);
					}
					#endif
				}
				
				void set_pull(Pull p) {
					#if defined(STM32F1)
					// TODO: Unified configure() method?
					#elif defined(STM32F3) || defined(STM32F4)
					g.reg.PUPDR = (g.reg.PUPDR & ~(3 << (n * 2))) | p << (n * 2);
					#endif
				}
				
				void set_af(int af) {
					#if defined(STM32F3) || defined(STM32F4)
					if(n < 8) {
						g.reg.AFRL = (g.reg.AFRL & ~(0xf << (n * 4))) | af << (n * 4);
					} else {
						g.reg.AFRH = (g.reg.AFRH & ~(0xf << (n * 4 - 32))) | af << (n * 4 - 32);
					}
					#endif
				}
				
				void set_speed(Speed s) {
					#if defined(STM32F3) || defined(STM32F4)
					g.reg.OSPEEDR = (g.reg.OSPEEDR & ~(3 << (n * 2))) | s << (n * 2);
					#endif
				}
				
				void on() {
					g.reg.BSRR = 1 << n;
				}
				
				void off() {
					g.reg.BSRR = 1 << 16 << n;
				}
				
				void set(bool value) {
					if(value) {
						on();
					} else {
						off();
					}
				}
				
				bool get() {
					return g.reg.IDR & (1 << n);
				}
				
				void toggle() {
					set(!(g.reg.ODR & (1 << n)));
				}
		};
		
		class PinArray {
			private:
				const GPIO_t& g;
				int f;
				int l;
				
				constexpr uint32_t mask1() {
					return ((2 << l) - 1) ^ ((1 << f) - 1);
				}
				
				constexpr uint32_t mask2() {
					return ((4 << (l * 2)) - 1) ^ ((1 << (f * 2)) - 1);
				}
			
			public:
				constexpr PinArray(const GPIO_t& gpio, int first, int last) : g(gpio), f(first), l(last) {}
				
				#if defined(STM32F3) || defined(STM32F4)
				void set_mode(Pin::Mode m) {
					g.reg.MODER = (g.reg.MODER & ~mask2()) | ((0x55555555 * m) & mask2());
				}
				
				void set_type(Pin::Type t) {
					g.reg.OTYPER = (g.reg.OTYPER & ~mask2()) | ((0x55555555 * t) & mask2());
				}
				
				void set_pull(Pin::Pull p) {
					g.reg.PUPDR = (g.reg.PUPDR & ~mask2()) | ((0x55555555 * p) & mask2());
				}
				#endif
				
				void set(uint16_t value) {
					value <<= f;
					g.reg.BSRR = ((~value & mask1()) << 16) | (value & mask1());
				}
				
				uint16_t get() {
					return g.reg.IDR & mask1();
				}
		};
		
		constexpr GPIO_t(uint32_t reg_addr) : reg(*(GPIO_reg_t*)reg_addr) {}
		
		constexpr Pin operator[](int pin) {
			return Pin(*this, pin);
		}
		
		constexpr PinArray array(int first, int last) {
			return PinArray(*this, first, last);
		}
};

typedef GPIO_t::Pin Pin;
typedef GPIO_t::PinArray PinArray;

#if defined(STM32F1)
static GPIO_t GPIOA(0x40010800);
static GPIO_t GPIOB(0x40010c00);
static GPIO_t GPIOC(0x40011000);
static GPIO_t GPIOD(0x40011400);
#elif defined(STM32F3)
static GPIO_t GPIOA(0x48000000);
static GPIO_t GPIOB(0x48000400);
static GPIO_t GPIOC(0x48000800);
static GPIO_t GPIOD(0x48000c00);
static GPIO_t GPIOE(0x48001000);
static GPIO_t GPIOF(0x48001400);
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
