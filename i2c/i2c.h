#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <interrupt/interrupt.h>

struct I2C_reg_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t DR;
	volatile uint32_t SR1;
	volatile uint32_t SR2;
	volatile uint32_t CCR;
	volatile uint32_t TRISE;
};

class I2C_t {
	private:
		volatile uint8_t addr;
		volatile uint8_t writing;
		volatile uint8_t reading;
		volatile uint8_t* write_p;
		volatile uint8_t* read_p;
		
		volatile bool busy;
	
	public:
		I2C_reg_t& reg;
		const uint32_t clk;
		Interrupt::IRQ irq_ev_n;
		Interrupt::IRQ irq_er_n;
		
		I2C_t(uint32_t reg_addr, uint32_t bus_clk, Interrupt::IRQ ev_n, Interrupt::IRQ er_n) : reg(*(I2C_reg_t*)reg_addr), clk(bus_clk), irq_ev_n(ev_n), irq_er_n(er_n) {
			reading = writing = 0;
		}
		
		void irq_ev();
		void irq_er();
		
		void handle_error();
		
		void enable();
		
		void write_reg(uint8_t addr_, uint8_t reg_, uint8_t data);
		void read_reg(uint8_t addr_, uint8_t reg_, uint8_t len, uint8_t* buf);
};

#if defined(STM32F1)
extern I2C_t I2C1;
extern I2C_t I2C2;
#elif defined(STM32F4)
extern I2C_t I2C1;
extern I2C_t I2C2;
extern I2C_t I2C3;
#endif

typedef I2C_t I2C;

#endif
