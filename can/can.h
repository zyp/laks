#ifndef CAN_H
#define CAN_H

#include <stdint.h>

struct CAN_reg_t {
	volatile uint32_t MCR;
	volatile uint32_t MSR;
	volatile uint32_t TSR;
	volatile uint32_t RFR[2];
	volatile uint32_t IER;
	volatile uint32_t ESR;
	volatile uint32_t BTR;
};

struct CAN_mb_reg_t {
	struct mb_t {
		volatile uint32_t IR;
		volatile uint32_t DTR;
		volatile uint32_t DLR;
		volatile uint32_t DHR;
	};
	mb_t T[3];
	mb_t R[2];
};

struct CAN_f_reg_t {
	volatile uint32_t FMR;
	volatile uint32_t FM1R[2];
	volatile uint32_t FS1R[2];
	volatile uint32_t FFA1R[2];
	volatile uint32_t FA1R[2 + 7];
	volatile uint32_t FR[28][2];
};

class CAN_t {
	public:
		CAN_reg_t& reg;
		CAN_mb_reg_t& mb_reg;
		CAN_f_reg_t& f_reg;
		const uint32_t clk;
		
		CAN_t(uint32_t reg_addr, uint32_t bus_clk) : reg(*(CAN_reg_t*)reg_addr), mb_reg(*(CAN_mb_reg_t*)(reg_addr + 0x180)), f_reg(*(CAN_f_reg_t*)(reg_addr + 0x200)), clk(bus_clk) {}
		
		void enable() {
			// Perform a master reset.
			reg.MCR = 0x8000;
			while(reg.MCR & 0x8000);
			
			// Enable initialization mode.
			reg.MCR = 0x1;
			
			// Wait for initialization mode acknowledge.
			while(!(reg.MSR & 0x1));
			
			// Set bit timing.
			reg.BTR = 0x01120000 | (clk / 6 / 1000000 - 1);
			
			// Disable initialization mode.
			reg.MCR = 0x0;
		}
};

#if defined(STM32F1)
static CAN_t CAN1(0x40006400, 36000000);
#elif defined(STM32F4)
static CAN_t CAN1(0x40006400, 42000000);
static CAN_t CAN2(0x40006800, 42000000);
#elif defined(STM32F0)
static CAN_t CAN1(0x40006400, 48000000);
#endif

#endif
