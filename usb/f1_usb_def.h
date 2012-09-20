#ifndef F1_USB_DEF_H
#define F1_USB_DEF_H

#include <stdint.h>

class F1_USB_t {
	private:
		struct F1_USB_reg_t {
			volatile uint32_t EPR[16];
			volatile uint32_t CNTR;
			volatile uint32_t ISTR;
			volatile uint32_t FNR;
			volatile uint32_t DADDR;
			volatile uint32_t BTABLE;
		};
		
		struct F1_USB_bufd_t {
			volatile uint32_t ADDR_TX;
			volatile uint32_t COUNT_TX;
			volatile uint32_t ADDR_RX;
			volatile uint32_t COUNT_RX;
		};
		
	public:
		F1_USB_reg_t& reg;
		F1_USB_bufd_t* bufd;
		volatile uint32_t* buf;
		
		F1_USB_t(uint32_t reg_addr, uint32_t buf_addr) :
			reg(*(F1_USB_reg_t*)reg_addr),
			bufd((F1_USB_bufd_t*)buf_addr),
			buf((volatile uint32_t*)buf_addr) {}
};

#endif
