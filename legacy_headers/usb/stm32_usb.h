#pragma once

#include "generic.h"

import laks.periph.usb.stm32;

#include <type_traits>

template <typename T>
class STM32_USB_drv : public USB_generic {
	private:
		const STM32_USB_t<T>& usb;
		
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
			
			uint32_t old_epr = usb->EPR[ep];
			//uint32_t new_epr = 0x3220;
			uint32_t new_epr = 0x8080 | ((type == EPType::Bulk ? 0 : type == EPType::Control ? 1 : type == EPType::Isochronous ? 2 : 3) << 9) | ep;
			
			if(in || ep == 0) {
				if constexpr (std::is_same_v<T, STM32_USB_reg_v3_t>) {
					usb.bufd[ep].TXRXBD = buf_end;
				} else {
					usb.bufd[ep].ADDR_TX = buf_end;
				}
				buf_end += size;
				
				new_epr |= (old_epr & 0x0070) ^ 0x0020;
			}
			
			if(!in) {
				if constexpr (std::is_same_v<T, STM32_USB_reg_v3_t>) {
					usb.bufd[ep].RXTXBD = (1 << 31) | (1 << 26) | buf_end;
				} else {
					usb.bufd[ep].ADDR_RX = buf_end;
					usb.bufd[ep].COUNT_RX = (1 << 15) | (1 << 10);
				}
				buf_end += size;
				
				new_epr |= (old_epr & 0x7000) ^ 0x3000;
			}
			
			usb->EPR[ep] = new_epr;
			
			usb_rblog.log("EPR: old=%04x, new=%04x", old_epr, usb->EPR[ep]);
		}
		
		virtual void hw_set_stall(uint8_t ep) {
			usb_rblog.log("Setting stall on EP %d", ep);
			
			usb->EPR[ep] = (usb->EPR[ep] & 0x878f) | 0x2030;
			
			//otg.dev_iep_reg[ep].DIEPCTL |= (1 << 21);
		}
	
	public:
		STM32_USB_drv(const STM32_USB_t<T>& usb_periph, desc_t dev, desc_t conf) : USB_generic(dev, conf), usb(usb_periph) {}
		
		void init() {
			usb->CNTR = 3;
			//Time::sleep(10);
			usb->CNTR = 1;
			//Time::sleep(10);
			// Exit power down mode.
			usb->CNTR = 0;
		}
		
		void process() {
			uint32_t istr = usb->ISTR;
			
			if(istr & (1 << 10)) {
				usb_rblog.log("USB Reset");
				
				buf_end = 0x40;
				
				usb->DADDR = 0x80;
				
				handle_reset();
				
				usb->ISTR = ~(1 << 10);
				
				return;
			}
			
			if(istr & (1 << 15)) {
				usb_rblog.log("USB Transfer: %02x", istr & 0x1f);
				
				uint32_t ep = istr & 0xf;
				uint32_t dir = istr & 0x10;
				
				usb_rblog.log("EPR%d: %04x", ep, usb->EPR[ep]);
				
				if(dir) {
					// RX.
					
					uint32_t len;
					if constexpr (std::is_same_v<T, STM32_USB_reg_v3_t>) {
						usb_rblog.log("RXBUF: RXTXBD: %08x", usb.bufd[ep].RXTXBD);
						len = (usb.bufd[ep].RXTXBD >> 16) & 0x03ff;
					} else {
						usb_rblog.log("RXBUF: ADDR: %04x, COUNT: %04x", usb.bufd[ep].ADDR_RX, usb.bufd[ep].COUNT_RX);
						len = usb.bufd[ep].COUNT_RX & 0x03ff;
					}

					if(usb->EPR[ep] & (1 << 11)) {
						usb_rblog.log("SETUP packet received");
						
						read(0, setupbuf, 8);
						
						handle_setup(setupbuf);
						
					} else {
						usb_rblog.log("OUT packet received");
						
						handle_out(ep, len);
					}
					
					//usb->EPR[ep] = 0x9280;
					//usb->EPR[ep] &= 0x078f;
					
					usb->EPR[ep] = (usb->EPR[ep] & 0x078f) | 0x1000;
				} else {
					// TX.
					
					if constexpr (std::is_same_v<T, STM32_USB_reg_v3_t>) {
						usb_rblog.log("TXBUF: TXRXBD: %08x", usb.bufd[ep].TXRXBD);
					} else {
						usb_rblog.log("TXBUF: ADDR: %04x, COUNT: %04x", usb.bufd[ep].ADDR_TX, usb.bufd[ep].COUNT_TX);
					}
					
					if(pending_addr) {
						usb_rblog.log("Actually changing addr to: %d", pending_addr);
						
						usb->DADDR = 0x80 | pending_addr;
						
						pending_addr = 0;
					}
					
					usb->EPR[ep] &= 0x870f;
				}
				usb_rblog.log("Leaving: EPR%d: %04x", ep, usb->EPR[ep]);
			}
		}
		
		virtual bool ep_ready(uint32_t ep) {
			return (usb->EPR[ep] & 0x30) == 0x20;
		}
		
		virtual void write(uint32_t ep, uint32_t* bufp, uint32_t len) {
			usb_rblog.log("Writing, ep=%d, len=%d", ep, len);
			
			if constexpr (std::is_same_v<T, STM32_USB_reg_v3_t>) {
				uint32_t base = (usb.bufd[ep].TXRXBD & 0xffff) >> 2;
				
				for(uint32_t i = 0; i < len; i += 4) {
					usb.buf[base + (i >> 2)] = *bufp++;
				}
				
				usb.bufd[ep].TXRXBD = (len << 16) | (usb.bufd[ep].TXRXBD & 0xffff);

			} else {
				uint16_t* p = (uint16_t*)bufp;
				uint32_t base = usb.bufd[ep].ADDR_TX >> 1;
				
				for(uint32_t i = 0; i < len; i += 2) {
					usb.buf[base + (i >> 1)] = *p++;
				}
				
				usb.bufd[ep].COUNT_TX = len;
			}

			usb->EPR[ep] = (usb->EPR[ep] & 0x870f) | 0x0010;
		}
		
		virtual uint32_t read(uint32_t ep, uint32_t* bufp, uint32_t len) {
			usb_rblog.log("Reading, ep=%d, len=%d", ep, len);
			
			if constexpr (std::is_same_v<T, STM32_USB_reg_v3_t>) {
				uint32_t base = (usb.bufd[ep].RXTXBD & 0xffff) >> 2;
				
				for(uint32_t i = 0; i < len; i += 4) {
					*bufp++ = usb.buf[base + (i >> 2)];
				}

			} else {
				uint16_t* p = (uint16_t*)bufp;
				uint32_t base = usb.bufd[ep].ADDR_RX >> 1;
				
				for(uint32_t i = 0; i < len; i += 2) {
					*p++ = uint16_t(usb.buf[base + (i >> 1)]);
				}
			}
			
			return len;
		}
};
