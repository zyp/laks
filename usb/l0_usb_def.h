#ifndef L0_USB_DEF_H
#define L0_USB_DEF_H

#include <stdint.h>

class L0_USB_t {
	private:
		struct L0_USB_reg_t {
			volatile uint32_t EPR[16];
			volatile uint32_t CNTR;
			volatile uint32_t ISTR;
			volatile uint32_t FNR;
			volatile uint32_t DADDR;
			volatile uint32_t BTABLE;
			volatile uint32_t LPMCSR;
			volatile uint32_t BCDR;
		};
		
		struct L0_USB_bufd_t {
			volatile uint16_t ADDR_TX;
			volatile uint16_t COUNT_TX;
			volatile uint16_t ADDR_RX;
			volatile uint16_t COUNT_RX;
		};
		
	public:
		L0_USB_reg_t& reg;
		L0_USB_bufd_t* bufd;
		volatile uint16_t* buf;
		
		L0_USB_t(uint32_t reg_addr, uint32_t buf_addr) :
			reg(*(L0_USB_reg_t*)reg_addr),
			bufd((L0_USB_bufd_t*)buf_addr),
			buf((volatile uint16_t*)buf_addr) {}
};

#endif
