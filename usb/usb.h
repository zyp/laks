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

static uint32_t buf[16];

struct desc_t {
	uint32_t size;
	void* data;
};

class USB_generic {
	private:
		desc_t dev_desc;
		desc_t conf_desc;
	
	public:
		USB_generic(desc_t dev, desc_t conf) : dev_desc(dev), conf_desc(conf) {}
		
		virtual bool ep_ready(uint32_t ep) = 0;
		virtual void write(uint32_t ep, uint32_t* bufp, uint32_t len) = 0;
		virtual void hw_set_address(uint8_t addr) = 0;
		virtual void hw_conf_ep(uint8_t ep, uint32_t conf) = 0;
		virtual void hw_set_stall(uint8_t ep) = 0;
	
	protected:
		bool get_descriptor(uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
			desc_t* descp;
			
			switch(wValue) {
				case 0x100:
					descp = &dev_desc;
					break;
				case 0x200:
					descp = &conf_desc;
					break;
				default:
					return false;
			}
			
			uint32_t* dp = (uint32_t*)descp->data;
			uint32_t length = wLength > descp->size ? descp->size : wLength;
			
			while(length > 64) {
				write(0, dp, 64);
				dp += 16;
				length -= 64;
				
				while(!ep_ready(0));
			}
			
			write(0, dp, length);
			
			return true;
		}

		bool set_address(uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
			hw_set_address(wValue);
			write(0, 0, 0);
			return true;
		}

		bool set_configuration(uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
			switch(wValue) {
				case 0:
					hw_conf_ep(1, 0);
					break;
		
				case 1:
					hw_conf_ep(1, (1 << 22) | (2 << 18) | (1 << 15) | 64);
					break;
		
				default:
					return false;
			}
	
			write(0, 0, 0);
			return true;
		}
		
		
		void handle_setup(const uint32_t* bufp) {
			uint8_t bmRequestType = bufp[0] & 0xff;
			uint8_t bRequest = (bufp[0] >> 8) & 0xff;
			uint16_t wValue = (bufp[0] >> 16) & 0xffff;
			uint16_t wIndex = bufp[1] & 0xffff;
			uint16_t wLength = (bufp[1] >> 16) & 0xffff;
	
			// GET_DESCRIPTOR
			if(bmRequestType == 0x80 && bRequest == 0x06) {
				if(get_descriptor(wValue, wIndex, wLength)) {
					return;
				}
			}
	
			// SET_ADDRESS
			if(bmRequestType == 0x00 && bRequest == 0x05) {
				if(set_address(wValue, wIndex, wLength)) {
					return;
				}
			}
	
			// SET_CONFIGURATION
			if(bmRequestType == 0x00 && bRequest == 0x09) {
				if(set_configuration(wValue, wIndex, wLength)) {
					return;
				}
			}
	
			// I2C_READ
			//if(bmRequestType == 0xc0 && bRequest == 0xf0) {
			//	if(i2c_read(wValue, wIndex, wLength)) {
			//		return;
			//	}
			//}
	
			// JTAG_SHIFT
			//if(bmRequestType == 0xc0 && bRequest == 0xff) {
			//	if(jtag_shift(wValue, wIndex, wLength)) {
			//		return;
			//	}
			//}
			
			hw_set_stall(0);
		}
		
};

class USB_otg : public USB_generic {
	private:
		USB_t& otg;
	
	protected:
		virtual void hw_set_address(uint8_t addr) {
			otg.dev_reg.DCFG |= addr << 4;
		}
		
		virtual void hw_conf_ep(uint8_t ep, uint32_t conf) {
			otg.dev_iep_reg[ep].DIEPCTL = conf;
		}
		
		virtual void hw_set_stall(uint8_t ep) {
			otg.dev_iep_reg[ep].DIEPCTL |= (1 << 21);
		}
	
	public:
		USB_otg(USB_t& otg_periph, desc_t dev, desc_t conf) : USB_generic(dev, conf), otg(otg_periph) {}
		
		void init() {
			// Set PHYSEL.
			otg.reg.GUSBCFG |= (1 << 6);
			
			Time::sleep(10);
			
			while(!(otg.reg.GRSTCTL & (1 << 31)));
			otg.reg.GRSTCTL |= 1;
			while(otg.reg.GRSTCTL & 1);
			
			otg.reg.GAHBCFG = 0;
			
			// USB configuration
			otg.reg.GUSBCFG = (1 << 30) | (0xf << 10) | (0 << 9) | (0 << 8) | (1 << 6);
			//                 FDMOD       TRDT          HNPCAP     SRPCAP     PHYSEL
			
			// interrupt mask
			otg.reg.GINTMSK = (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 4);
			//                 ENUMDNEM    USBRST      USBSUSPM    ESUSPM      SOFM       OTGINT     MMISM
			
			// device configuration
			otg.dev_reg.DCFG = (1 << 2) | 3;
			//                 NZLSOHSK   DSPD
			
			// core configuration
			otg.reg.GCCFG = (1 << 19) | (1 << 16);
			//               VBUSBSEN    PWRDWN
			
		}
		
		void process() {
			// USB reset.
			if(otg.reg.GINTSTS & (1 << 12)) {
				otg.dev_oep_reg[0].DOEPCTL = (1 << 27);
				otg.dev_reg.DAINTMSK = (1 << 16) | 1;
				otg.dev_reg.DOEPMSK = (1 << 3) | 1;
				otg.dev_reg.DIEPEMPMSK = (1 << 3) | 1;
				otg.reg.GRXFSIZ = 256;
				otg.reg.DIEPTXF0 = (64 << 16) | 256;
				otg.reg.DIEPTXF1 = (64 << 16) | 320;
				otg.dev_oep_reg[0].DOEPTSIZ = (3 << 29);
			}
	
			// OTG interrupt.
			if(otg.reg.GINTSTS & (1 << 2)) {
				otg.reg.GOTGINT = (1 << 2); // SEDET
			}
	
			// RxFIFO non-empty.
			if(otg.reg.GINTSTS & (1 << 4)) {
				handle_rxfifo();
			}
	
			otg.reg.GINTSTS = 0xffffffff;
		}
	
		virtual bool ep_ready(uint32_t ep) {
			return (otg.dev_iep_reg[ep].DIEPCTL & 0x80008000) == 0x8000;
		}
		
		virtual void write(uint32_t ep, uint32_t* bufp, uint32_t len) {
			otg.dev_iep_reg[ep].DIEPTSIZ = (1 << 19) | len;
			//                               PKTCNT
			otg.dev_iep_reg[ep].DIEPCTL |= (1 << 31) | (1 << 26);
			//                               EPENA       CNAK
	
			len = (len + 3) >> 2;
	
			while(len--) {
				otg.fifo[ep].reg = *bufp++;
			}
		}
		
		void handle_rxfifo() {
			uint32_t status = otg.reg.GRXSTSP;
	
			uint32_t len = (status & 0x7ff0) >> 6;
	
			for(uint32_t i = 0; i < len; i++) {
				buf[i] = otg.fifo[0].reg;
			}
	
			//if(status == 0x000c0080) {
			if((status & (0xf << 17)) == (0x4 << 17)) {
				handle_setup(buf);
				otg.dev_oep_reg[0].DOEPCTL |= (1 << 26);
			}
		}
};

#endif
