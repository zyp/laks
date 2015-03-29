#ifndef L0_USB_H
#define L0_USB_H

#include "generic.h"
#include "l0_usb_def.h"

class USB_l0 : public USB_generic {
	private:
		L0_USB_t& usb;
		
		uint32_t setupbuf[16];
		
		uint32_t buf_end;
		
		uint8_t pending_addr;
	
	protected:
		virtual void hw_set_address(uint8_t addr) {
			usb_rblog.log("SetAddress: %d", addr);
			
			pending_addr = addr;
		}
		
		virtual void hw_conf_ep(uint8_t ep, EPType type, uint32_t size) {
			usb_rblog.log("Configuring EP%02x: size=%d", ep, size);
			
			uint8_t in = ep & 0x80;
			ep &= 0x7f;
			
			uint32_t old_epr = usb.reg.EPR[ep];
			//uint32_t new_epr = 0x3220;
			uint32_t new_epr = 0x8080 | ((type == EPType::Bulk ? 0 : type == EPType::Control ? 1 : type == EPType::Isochronous ? 2 : 3) << 9) | ep;
			
			if(in || ep == 0) {
				usb.bufd[ep].ADDR_TX = buf_end;
				buf_end += size;
				
				new_epr |= (old_epr & 0x0070) ^ 0x0020;
			}
			
			if(!in) {
				usb.bufd[ep].ADDR_RX = buf_end;
				buf_end += size;
				usb.bufd[ep].COUNT_RX = 0x8000 | (1 << 10);
				
				new_epr |= (old_epr & 0x7000) ^ 0x3000;
			}
			
			usb.reg.EPR[ep] = new_epr;
			
			usb_rblog.log("EPR: old=%04x, new=%04x", old_epr, usb.reg.EPR[ep]);
		}
		
		virtual void hw_set_stall(uint8_t ep) {
			usb_rblog.log("Setting stall on EP %d", ep);
			
			usb.reg.EPR[ep] = (usb.reg.EPR[ep] & 0x878f) | 0x2030;
			
			//otg.dev_iep_reg[ep].DIEPCTL |= (1 << 21);
		}
	
	public:
		USB_l0(L0_USB_t& usb_periph, desc_t dev, desc_t conf) : USB_generic(dev, conf), usb(usb_periph) {}
		
		void init() {
			usb.reg.CNTR = 3;
			Time::sleep(10);
			usb.reg.CNTR = 1;
			Time::sleep(10);
			// Exit power down mode.
			usb.reg.CNTR = 0;
		}
		
		void process() {
			uint32_t istr = usb.reg.ISTR;
			
			if(istr & (1 << 10)) {
				usb_rblog.log("USB Reset");
				
				buf_end = 0x40;
				
				usb.reg.DADDR = 0x80;
				
				handle_reset();
				
				usb.reg.ISTR = ~(1 << 10);
				
				return;
			}
			
			if(istr & (1 << 15)) {
				usb_rblog.log("USB Transfer: %02x", istr & 0x1f);
				
				uint32_t ep = istr & 0xf;
				uint32_t dir = istr & 0x10;
				
				usb_rblog.log("EPR%d: %04x", ep, usb.reg.EPR[ep]);
				
				if(dir) {
					// RX.
					
					usb_rblog.log("RXBUF: ADDR: %04x, COUNT: %04x", usb.bufd[ep].ADDR_RX, usb.bufd[ep].COUNT_RX);
					
					uint32_t len = usb.bufd[ep].COUNT_RX & 0x03ff;
					
					if(usb.reg.EPR[ep] & (1 << 11)) {
						usb_rblog.log("SETUP packet received");
						
						read(0, setupbuf, 8);
						
						handle_setup(setupbuf);
						
					} else {
						usb_rblog.log("OUT packet received");
						
						handle_out(ep, len);
					}
					
					//usb.reg.EPR[ep] = 0x9280;
					//usb.reg.EPR[ep] &= 0x078f;
					
					usb.reg.EPR[ep] = (usb.reg.EPR[ep] & 0x078f) | 0x1000;
				} else {
					// TX.
					
					usb_rblog.log("TXBUF: ADDR: %04x, COUNT: %04x", usb.bufd[ep].ADDR_TX, usb.bufd[ep].COUNT_TX);
					
					if(pending_addr) {
						usb_rblog.log("Actually changing addr to: %d", pending_addr);
						
						usb.reg.DADDR = 0x80 | pending_addr;
						
						pending_addr = 0;
					}
					
					usb.reg.EPR[ep] &= 0x870f;
				}
				usb_rblog.log("Leaving: EPR%d: %04x", ep, usb.reg.EPR[ep]);
			}
		}
		
		virtual bool ep_ready(uint32_t ep) {
			return (usb.reg.EPR[ep] & 0x30) == 0x20;
		}
		
		virtual void write(uint32_t ep, uint32_t* bufp, uint32_t len) {
			usb_rblog.log("Writing, ep=%d, len=%d", ep, len);
			
			uint16_t* p = (uint16_t*)bufp;
			uint32_t base = usb.bufd[ep].ADDR_TX >> 1;
			
			for(uint32_t i = 0; i < len; i += 2) {
				usb.buf[base + (i >> 1)] = *p++;
			}
			
			usb.bufd[ep].COUNT_TX = len;
			usb.reg.EPR[ep] = (usb.reg.EPR[ep] & 0x870f) | 0x0010;
		}
		
		virtual uint32_t read(uint32_t ep, uint32_t* bufp, uint32_t len) {
			usb_rblog.log("Reading, ep=%d, len=%d", ep, len);
			
			uint16_t* p = (uint16_t*)bufp;
			uint32_t base = usb.bufd[ep].ADDR_RX >> 1;
			
			for(uint32_t i = 0; i < len; i += 2) {
				*p++ = uint16_t(usb.buf[base + (i >> 1)]);
			}
			
			return len;
		}
};

#endif
