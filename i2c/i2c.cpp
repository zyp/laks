#include "i2c.h"

#include <rcc/rcc.h>
#include <os/thread.h>

#if defined(STM32F1)
I2C_t I2C1(0x40005400, 36000000, Interrupt::I2C1_EV, Interrupt::I2C1_ER);
I2C_t I2C2(0x40005800, 36000000, Interrupt::I2C2_EV, Interrupt::I2C2_ER);
#elif defined(STM32F4)
I2C_t I2C1(0x40005400, 42000000, Interrupt::I2C1_EV, Interrupt::I2C1_ER);
I2C_t I2C2(0x40005800, 42000000, Interrupt::I2C2_EV, Interrupt::I2C2_ER);
//I2C_t I2C3(0x40005c00, 42000000, Interrupt::I2C3_EV, Interrupt::I2C3_ER);
#endif

#if defined(STM32F1) || defined(STM32F4)
void I2C_t::irq_ev() {
	uint32_t sr1 = reg.SR1;
	reg.SR2;
	
	// EV5, SB = 1: Start condition sent.
	if(sr1 & 0x01) {
		// Send address.
		reg.DR = (addr << 1) | (writing ? 0 : 1);
	}
	
	// EV6, ADDR = 1: Address sent.
	if(sr1 & 0x02) {
		if(writing) {
			reg.DR = *write_p++;
			writing--;
		} else {
			if(reading > 1) {
				reg.CR1 |= 0x400; // Set ACK.
			} else {
				reg.CR1 |= 0x200; // Set STOP.
			}
		}
	}
	
	// EV7, RxNE = 1: Receive buffer not empty.
	if(sr1 & 0x40) {
		*read_p++ = reg.DR;
		reading--;
		
		if(reading == 1) {
			// Unset ACK, set STOP.
			reg.CR1 = (reg.CR1 & ~0x400) | 0x200;
		}
		
		if(reading == 0) {
			busy = 0;
		}
	}
	
	//reg.CR1 &= ~0x400;
	
	// EV8, TxE = 1, BTF = 0: Transmit buffer empty, still writing.
	if(sr1 & 0x80 && !(sr1 & 0x04)) {
		if(writing) {
			// Send data.
			reg.DR = *write_p++;
			writing--;
		} else {
			// All data sent.
			
			if(reading) {
				// Send repeat start.
				reg.CR1 |= 0x100;
			} else {
				// Send stop.
				reg.CR1 |= 0x200;
				busy = 0;
			}
		}
	}
}

void I2C_t::irq_er() {
	handle_error();
}

void I2C_t::handle_error() {
	reg.SR1;
	reg.SR2;
	
	//while(1);
	
	reg.SR1 = 0;
	
	reg.CR1 |= 0x200;
	busy = 0;
}

void I2C_t::enable() {
	reg.CR1 = 0x8000;
	reg.CR1 = 0;
	
	reg.CR2 = 0x700 | (clk / 1000000);
	reg.TRISE = clk / 1000000 + 1;
	reg.CCR = clk / 2 / 100000;
	
	Interrupt::enable(irq_ev_n, &I2C_t::irq_ev, this);
	Interrupt::enable(irq_er_n, &I2C_t::irq_er, this);
	
	reg.CR1 = 1;
}

void I2C_t::write_reg(uint8_t addr_, uint8_t reg_, uint8_t data) {
	addr = addr_;
	writing = 2;
	reading = 0;
	volatile uint8_t buf[] = {reg_, data};
	write_p = buf;
	busy = 1;
	
	reg.CR1 |= 0x100;
	
	while(busy) {
		Thread::yield();
	}
}

void I2C_t::read_reg(uint8_t addr_, uint8_t reg_, uint8_t len, uint8_t* buf) {
	addr = addr_;
	writing = 1;
	reading = len;
	write_p = &reg_;
	read_p = buf;
	busy = 1;
	
	reg.CR1 |= 0x100;
	
	while(busy) {
		Thread::yield();
	}
}
#endif
