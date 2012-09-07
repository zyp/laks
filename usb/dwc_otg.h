#ifndef DWC_OTG_H
#define DWC_OTG_H

#include "generic.h"
#include "dwc_otg_def.h"

class USB_otg : public USB_generic {
	private:
		DWC_OTG_t& otg;
		
		uint32_t rxfifo_bytes;
		uint8_t rxfifo_ep;
		
		uint32_t setup_buf[16];
		
		void handle_rxfifo() {
			uint32_t status = otg.reg.GRXSTSP;
			
			uint8_t ep = status & 0x4;
			uint32_t len = (status & 0x7ff0) >> 4;
			uint32_t type = status & (0xf << 17);
			
			rxfifo_bytes = len;
			
			// OUT packet.
			if(type == (0x2 << 17)) {
				handle_out(ep, len);
			}
			
			// SETUP packet.
			if(type == (0x6 << 17)) {
				for(uint32_t i = 0; i < len; i += 4) {
					setup_buf[i >> 2] = otg.fifo[0].reg;
				}
				
				rxfifo_bytes = 0;
				
				handle_setup(setup_buf);
				otg.dev_oep_reg[0].DOEPCTL |= (1 << 26); // CNAK
			}
			
			// Discard remaining bytes from FIFO.
			for(uint32_t i = 0; i < rxfifo_bytes; i += 4) {
				(void)otg.fifo[0].reg;
			}
			
			rxfifo_bytes = 0;
		}
	
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
		USB_otg(DWC_OTG_t& otg_periph, desc_t dev, desc_t conf) : USB_generic(dev, conf), otg(otg_periph) {}
		
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
				handle_reset();
				
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
		
		virtual uint32_t read(uint32_t ep, uint32_t* bufp, uint32_t len) {
			if(ep != rxfifo_ep) {
				return 0;
			}
			
			if(len > rxfifo_bytes) {
				len = rxfifo_bytes;
			}
			
			// TODO: Handle non-mod4 length properly.
			
			for(uint32_t i = 0; i < len; i += 4) {
				bufp[i >> 2] = otg.fifo[0].reg;
			}
			
			rxfifo_bytes -= len;
			
			return len;
		}
};

#endif
