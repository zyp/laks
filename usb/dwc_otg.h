#ifndef DWC_OTG_H
#define DWC_OTG_H

#include "generic.h"
#include "dwc_otg_def.h"

class USB_otg : public USB_generic {
	private:
		DWC_OTG_t& otg;
		
		uint32_t rxfifo_size;
		
		uint32_t rxfifo_bytes;
		uint8_t rxfifo_ep;
		
		uint32_t setup_buf[16];
		
		uint32_t buf_end;
		
		void handle_rxfifo() {
			uint32_t status = otg.reg.GRXSTSP;
			
			usb_rblog.log("RXFIFO status: %08x", status);
			
			uint8_t ep = status & 0xf;
			uint32_t len = (status & 0x7ff0) >> 4;
			uint32_t type = status & (0xf << 17);
			
			rxfifo_ep = ep;
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
			}
			
			if(type == (0x4 << 17)) {
				handle_setup(setup_buf);
				otg.dev_oep_reg[0].DOEPCTL |= (1 << 31) | (1 << 26); // CNAK
			}
			
			// Discard remaining bytes from FIFO.
			for(uint32_t i = 0; i < rxfifo_bytes; i += 4) {
				(void)otg.fifo[0].reg;
			}
			
			// FIXME: Temporary workaround.
			if(type == (0x2 << 17) && ep != 0) {
				// Odd/even filtering on isochronous endpoints.
				if((otg.dev_oep_reg[ep].DOEPCTL & (3 << 18)) == (1 << 18)) {
					if(otg.dev_oep_reg[ep].DOEPCTL & (1 << 16)) {
						otg.dev_oep_reg[ep].DOEPCTL |= (1 << 28);
					} else {
						otg.dev_oep_reg[ep].DOEPCTL |= (1 << 29);
					}
				}
				
				// Clear NAK.
				otg.dev_oep_reg[ep].DOEPCTL |= (1 << 26);
			}
			
			rxfifo_bytes = 0;
		}
	
	protected:
		virtual void hw_set_address(uint8_t addr) {
			usb_rblog.log("SetAddress: %d", addr);
			
			otg.dev_reg.DCFG |= addr << 4;
		}
		
		virtual void hw_conf_ep(uint8_t ep, EPType type, uint32_t size) {
			usb_rblog.log("Configuring EP%02x: size=%d", ep, size);
			
			uint8_t in = ep & 0x80;
			ep &= 0x7f;
			
			uint32_t epctl = ((type == EPType::Control ? 0 : type == EPType::Isochronous ? 1 : type == EPType::Bulk ? 2 : 3) << 18); 
			epctl |= (1 << 28) | (1 << 15) | (ep == 0 ? 64 : size); // USBAEP, SD0PID
			
			if(ep == 0) {
				otg.reg.GRXFSIZ = rxfifo_size >> 2;
				buf_end = rxfifo_size >> 2;
				
				otg.reg.DIEPTXF0 = ((64 >> 2) << 16) | buf_end;
				buf_end += (64 >> 2);
				
				otg.dev_iep_reg[ep].DIEPTSIZ = size;
				otg.dev_oep_reg[ep].DOEPTSIZ = (1 << 29) | (1 << 19) | size;
				
				otg.dev_iep_reg[ep].DIEPCTL = epctl | (1 << 27); // SNAK
				otg.dev_oep_reg[ep].DOEPCTL = epctl | (1 << 31) | (1 << 26); // EPENA, CNAK
				
				return;
			}
			
			if(in) {
				otg.reg.DIEPTXF[ep - 1] = ((size >> 2) << 16) | buf_end;
				buf_end += size >> 2;
				
				otg.dev_iep_reg[ep].DIEPTSIZ = size;
				otg.dev_iep_reg[ep].DIEPCTL = epctl | (1 << 27) | (ep << 22); // SNAK
			} else {
				otg.dev_oep_reg[ep].DOEPTSIZ = (1 << 19) | size;
				otg.dev_oep_reg[ep].DOEPCTL = epctl | (1 << 31) | (1 << 26); // EPENA, CNAK
			}
		}
		
		virtual void hw_set_stall(uint8_t ep) {
			otg.dev_iep_reg[ep].DIEPCTL |= (1 << 21);
		}
	
	public:
		USB_otg(DWC_OTG_t& otg_periph, desc_t dev, desc_t conf) : USB_generic(dev, conf), otg(otg_periph), rxfifo_size(256) {}
		
		void set_rxfifo_size(uint32_t size) {
			rxfifo_size = size;
		}
		
		void set_vbus_sense(bool enabled) {
			if(enabled) {
				otg.reg.GCCFG &= ~(1 << 21); // NOVBUSSENS
			} else {
				otg.reg.GCCFG |= (1 << 21); // NOVBUSSENS
			}
		}
		
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
			uint32_t gintsts = otg.reg.GINTSTS;
			
			// USB reset.
			if(gintsts & (1 << 12)) {
				usb_rblog.log("USB Reset");
				
				otg.dev_reg.DCFG = (1 << 2) | 3;
				otg.dev_oep_reg[0].DOEPCTL = (1 << 27);
				otg.dev_oep_reg[1].DOEPCTL = (1 << 27);
				otg.dev_oep_reg[2].DOEPCTL = (1 << 27);
				otg.dev_oep_reg[3].DOEPCTL = (1 << 27);
				otg.dev_reg.DAINTMSK = (1 << 16) | 1;
				otg.dev_reg.DOEPMSK = (1 << 3) | 1;
				otg.dev_reg.DIEPEMPMSK = (1 << 3) | 1;
				
				buf_end = 0;
				
				handle_reset();
				
				otg.reg.GINTSTS = 1 << 12;
			}
			
			// Enumeration done.
			if(gintsts & (1 << 13)) {
				usb_rblog.log("Enumeration done");
				
				otg.reg.GINTSTS = 1 << 13;
				otg.dev_iep_reg[0].DIEPCTL = 0; // MPSIZ = 64 bytes.
			}
			
			// OTG interrupt.
			if(gintsts & (1 << 2)) {
				otg.reg.GOTGINT = (1 << 2); // SEDET
			}
			
			// RxFIFO non-empty.
			if(gintsts & (1 << 4)) {
				handle_rxfifo();
			}
		}
	
		virtual bool ep_ready(uint32_t ep) {
			return (otg.dev_iep_reg[ep].DIEPCTL & 0x80008000) == 0x8000;
		}
		
		virtual void write(uint32_t ep, uint32_t* bufp, uint32_t len) {
			usb_rblog.log("Writing, ep=%d, len=%d", ep, len);
			
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
			usb_rblog.log("Reading, ep=%d, len=%d", ep, len);
			
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
			
			usb_rblog.log("Read %d bytes.", len);
			
			return len;
		}
};

#endif
