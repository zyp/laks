#ifndef GENERIC_H
#define GENERIC_H

#include <stdint.h>

struct desc_t {
	uint32_t size;
	void* data;
};

enum SetupStatus {Unhandled, Ok, Stall};

class USB_class_driver {
	friend class USB_generic;
	
	protected:
		virtual SetupStatus handle_setup(uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
			return SetupStatus::Unhandled;
		}
};

class USB_generic {
	private:
		desc_t dev_desc;
		desc_t conf_desc;
		
		USB_class_driver* control_handlers[4];
	
	public:
		USB_generic(desc_t dev, desc_t conf) : dev_desc(dev), conf_desc(conf) {}
		
		virtual bool ep_ready(uint32_t ep) = 0;
		virtual void write(uint32_t ep, uint32_t* bufp, uint32_t len) = 0;
		virtual void hw_set_address(uint8_t addr) = 0;
		virtual void hw_conf_ep(uint8_t ep, uint32_t conf) = 0;
		virtual void hw_set_stall(uint8_t ep) = 0;
		
		bool register_control_handler(USB_class_driver* control_handler) {
			for(USB_class_driver*& handler : control_handlers) {
				if(!handler || handler == control_handler) {
					handler = control_handler;
					return true;
				}
			}
			
			// Handler table is full.
			return false;
		}
	
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
			
			SetupStatus res = SetupStatus::Unhandled;
			
			for(USB_class_driver*& handler : control_handlers) {
				if(handler) {
					res = handler->handle_setup(bmRequestType, bRequest, wValue, wIndex, wLength);
					
					if(res != SetupStatus::Unhandled) {
						break;
					}
				}
			}
			
			if(res == SetupStatus::Ok) {
				return;
			}
			
			hw_set_stall(0);
		}
		
};

#endif
