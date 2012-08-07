#ifndef USB_H
#define USB_H

#include <stdint.h>

struct USB_reg_t {
	volatile uint32_t GOTGCTL;
	volatile uint32_t GOTGINT;
	volatile uint32_t GAHBCFG;
	volatile uint32_t GUSBCFG;
	volatile uint32_t GRSTCTL;
	volatile uint32_t GINTSTS;
	volatile uint32_t GINTMSK;
	volatile uint32_t GRXSTSR;
	volatile uint32_t GRXSTSP;
	volatile uint32_t GRXFSIZ;
	volatile uint32_t DIEPTXF0;
	volatile uint32_t HNPTXSTS;
	uint32_t _reserved[2];
	volatile uint32_t GCCFG;
	volatile uint32_t CID;
	uint32_t _reserved1[48];
	volatile uint32_t HPTXFSIZ;
	volatile uint32_t DIEPTXF1;
	volatile uint32_t DIEPTXF2;
	volatile uint32_t DIEPTXF3;
};

struct USB_dev_reg_t {
	volatile uint32_t DCFG;
	volatile uint32_t DCTL;
	volatile uint32_t DSTS;
	uint32_t _reserved;
	volatile uint32_t DIEPMSK;
	volatile uint32_t DOEPMSK;
	volatile uint32_t DAINT;
	volatile uint32_t DAINTMSK;
	uint32_t _reserved1[2];
	volatile uint32_t DVBUSDIS;
	volatile uint32_t DVBUSPULSE;
	uint32_t _reserved2;
	volatile uint32_t DIEPEMPMSK;
};

struct USB_dev_iep_reg_t {
	volatile uint32_t DIEPCTL;
	uint32_t _reserved;
	volatile uint32_t DIEPINT;
	uint32_t _reserved1;
	volatile uint32_t DIEPTSIZ;
	uint32_t _reserved2;
	volatile uint32_t DTXFSTS;
	uint32_t _reserved3;
};

struct USB_dev_oep_reg_t {
	volatile uint32_t DOEPCTL;
	uint32_t _reserved;
	volatile uint32_t DOEPINT;
	uint32_t _reserved1;
	volatile uint32_t DOEPTSIZ;
	uint32_t _reserved2[3];
};

union USB_fifo_reg_t {
	volatile uint32_t reg;
	volatile uint32_t buf[1024];
};

class USB_t {
	public:
		USB_reg_t& reg;
		USB_dev_reg_t& dev_reg;
		USB_dev_iep_reg_t* const dev_iep_reg;
		USB_dev_oep_reg_t* const dev_oep_reg;
		USB_fifo_reg_t* const fifo;
		
		USB_t(uint32_t reg_addr) :
			reg(*(USB_reg_t*)reg_addr),
			dev_reg(*(USB_dev_reg_t*)(reg_addr + 0x800)),
			dev_iep_reg((USB_dev_iep_reg_t*)(reg_addr + 0x900)),
			dev_oep_reg((USB_dev_oep_reg_t*)(reg_addr + 0xb00)),
			fifo((USB_fifo_reg_t*)(reg_addr + 0x1000)) {}
};

#if defined(STM32F1)

#elif defined(STM32F4)
static USB_t OTG_FS(0x50000000);
static USB_t OTG_HS(0x40040000);
#endif

#endif
