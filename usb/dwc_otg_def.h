#ifndef DWC_OTG_DEF_H
#define DWC_OTG_DEF_H

#include <mmio/mmio.h>

		struct DWC_OTG_reg_t {
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
			volatile uint32_t DIEPTXF[5];
		};
		
		struct DWC_OTG_dev_reg_t {
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
		
		struct DWC_OTG_dev_iep_reg_t {
			volatile uint32_t DIEPCTL;
			uint32_t _reserved;
			volatile uint32_t DIEPINT;
			uint32_t _reserved1;
			volatile uint32_t DIEPTSIZ;
			uint32_t _reserved2;
			volatile uint32_t DTXFSTS;
			uint32_t _reserved3;
		};
		
		struct DWC_OTG_dev_oep_reg_t {
			volatile uint32_t DOEPCTL;
			uint32_t _reserved;
			volatile uint32_t DOEPINT;
			uint32_t _reserved1;
			volatile uint32_t DOEPTSIZ;
			uint32_t _reserved2[3];
		};
		
		union DWC_OTG_fifo_reg_t {
			volatile uint32_t reg;
			volatile uint32_t buf[1024];
		};
		
class DWC_OTG_t : public mmio_ptr<DWC_OTG_reg_t> {
	public:
		mmio_ptr<DWC_OTG_dev_reg_t> dev_reg;
		mmio_ptr<DWC_OTG_dev_iep_reg_t> dev_iep_reg;
		mmio_ptr<DWC_OTG_dev_oep_reg_t> dev_oep_reg;
		mmio_ptr<DWC_OTG_fifo_reg_t> fifo;
		
		constexpr DWC_OTG_t(uint32_t reg_addr) :
			mmio_ptr(reg_addr),
			dev_reg(reg_addr + 0x800),
			dev_iep_reg(reg_addr + 0x900),
			dev_oep_reg(reg_addr + 0xb00),
			fifo(reg_addr + 0x1000) {}
};

#endif
