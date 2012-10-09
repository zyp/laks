#ifndef PIN_H
#define PIN_H

#include "gpio.h"

class Pin {
	private:
		GPIO_t& g;
		int n;
	
	public:
		Pin(GPIO_t& gpio, int pin) : g(gpio), n(pin) {}
		
		enum Mode {
			#if defined(STM32F1)
			Input = 0x4,
			Output = 0x3,
			AF = 0xb,
			Analog = 0x0,
			#elif defined(STM32F4)
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
				g.CRL = (g.CRL & ~(0xf << (n * 4))) | m << (n * 4);
			} else {
				g.CRH = (g.CRH & ~(0xf << (n * 4 - 32))) | m << (n * 4 - 32);
			}
			#elif defined(STM32F4)
			g.MODER = (g.MODER & ~(3 << (n * 2))) | m << (n * 2);
			#endif
		}
		
		void set_type(Type t) {
			#if defined(STM32F1)
			// TODO: Unified configure() method?
			#elif defined(STM32F4)
			if(t) {
				g.OTYPER |= 1 << n;
			} else {
				g.OTYPER &= ~(1 << n);
			}
			#endif
		}
		
		void set_pull(Pull p) {
			#if defined(STM32F1)
			// TODO: Unified configure() method?
			#elif defined(STM32F4)
			g.PUPDR = (g.PUPDR & ~(3 << (n * 2))) | p << (n * 2);
			#endif
		}
		
		void set_af(int af) {
			#if defined(STM32F4)
			if(n < 8) {
				g.AFRL = (g.AFRL & ~(0xf << (n * 4))) | af << (n * 4);
			} else {
				g.AFRH = (g.AFRH & ~(0xf << (n * 4 - 32))) | af << (n * 4 - 32);
			}
			#endif
		}
		
		void set_speed(Speed s) {
			g.OSPEEDR = (g.OSPEEDR & ~(3 << (n * 2))) | s << (n * 2);
		}
		
		void on() {
			g.BSRR = 1 << n;
		}
		
		void off() {
			g.BSRR = 1 << 16 << n;
		}
		
		void set(bool value) {
			if(value) {
				on();
			} else {
				off();
			}
		}
		
		bool get() {
			return g.IDR & (1 << n);
		}
		
		void toggle() {
			set(!(g.ODR & (1 << n)));
		}
};

static Pin PA0(GPIOA, 0);
static Pin PA1(GPIOA, 1);
static Pin PA2(GPIOA, 2);
static Pin PA3(GPIOA, 3);
static Pin PA4(GPIOA, 4);
static Pin PA5(GPIOA, 5);
static Pin PA6(GPIOA, 6);
static Pin PA7(GPIOA, 7);
static Pin PA8(GPIOA, 8);
static Pin PA9(GPIOA, 9);
static Pin PA10(GPIOA, 10);
static Pin PA11(GPIOA, 11);
static Pin PA12(GPIOA, 12);
static Pin PA13(GPIOA, 13);
static Pin PA14(GPIOA, 14);
static Pin PA15(GPIOA, 15);

static Pin PB0(GPIOB, 0);
static Pin PB1(GPIOB, 1);
static Pin PB2(GPIOB, 2);
static Pin PB3(GPIOB, 3);
static Pin PB4(GPIOB, 4);
static Pin PB5(GPIOB, 5);
static Pin PB6(GPIOB, 6);
static Pin PB7(GPIOB, 7);
static Pin PB8(GPIOB, 8);
static Pin PB9(GPIOB, 9);
static Pin PB10(GPIOB, 10);
static Pin PB11(GPIOB, 11);
static Pin PB12(GPIOB, 12);
static Pin PB13(GPIOB, 13);
static Pin PB14(GPIOB, 14);
static Pin PB15(GPIOB, 15);

static Pin PC0(GPIOC, 0);
static Pin PC1(GPIOC, 1);
static Pin PC2(GPIOC, 2);
static Pin PC3(GPIOC, 3);
static Pin PC4(GPIOC, 4);
static Pin PC5(GPIOC, 5);
static Pin PC6(GPIOC, 6);
static Pin PC7(GPIOC, 7);
static Pin PC8(GPIOC, 8);
static Pin PC9(GPIOC, 9);
static Pin PC10(GPIOC, 10);
static Pin PC11(GPIOC, 11);
static Pin PC12(GPIOC, 12);
static Pin PC13(GPIOC, 13);
static Pin PC14(GPIOC, 14);
static Pin PC15(GPIOC, 15);

static Pin PD0(GPIOD, 0);
static Pin PD1(GPIOD, 1);
static Pin PD2(GPIOD, 2);
static Pin PD3(GPIOD, 3);
static Pin PD4(GPIOD, 4);
static Pin PD5(GPIOD, 5);
static Pin PD6(GPIOD, 6);
static Pin PD7(GPIOD, 7);
static Pin PD8(GPIOD, 8);
static Pin PD9(GPIOD, 9);
static Pin PD10(GPIOD, 10);
static Pin PD11(GPIOD, 11);
static Pin PD12(GPIOD, 12);
static Pin PD13(GPIOD, 13);
static Pin PD14(GPIOD, 14);
static Pin PD15(GPIOD, 15);

#endif
