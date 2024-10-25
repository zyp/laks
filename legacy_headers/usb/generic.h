#ifndef GENERIC_H
#define GENERIC_H

#include <stdint.h>

#include <util/rblog.h>

RBLog<256, 2> usb_rblog;

struct desc_t {
	uint32_t size;
	void* data;
};

enum SetupStatus {Unhandled, Ok, Stall};
enum class EPType {Control, Bulk, Interrupt, Isochronous};

class USB_class_driver {
	friend class USB_generic;
	
	protected:
		virtual SetupStatus handle_setup(uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
			return SetupStatus::Unhandled;
		}
		
		virtual void handle_set_configuration(uint8_t configuration) {}
		
		virtual void handle_out(uint8_t ep, uint32_t len) {}
};

class USB_generic {
	private:
		desc_t dev_desc;
		desc_t conf_desc;
		
		USB_class_driver* class_drivers[4];
		USB_class_driver* out_handlers[6];
		
		uint8_t current_configuration;
	
	public:
		USB_generic(desc_t dev, desc_t conf) : dev_desc(dev), conf_desc(conf) {}
		
		virtual bool ep_ready(uint32_t ep) = 0;
		virtual void write(uint32_t ep, uint32_t* bufp, uint32_t len) = 0;
		virtual uint32_t read(uint32_t ep, uint32_t* bufp, uint32_t len) = 0;
		virtual void hw_set_address(uint8_t addr) = 0;
		virtual void hw_conf_ep(uint8_t ep, EPType type, uint32_t size) = 0;
		virtual void hw_set_stall(uint8_t ep) = 0;
		
		bool register_driver(USB_class_driver* driver) {
			for(USB_class_driver*& d : class_drivers) {
				if(!d || d == driver) {
					d = driver;
					return true;
				}
			}
			
			// Handler table is full.
			return false;
		}
		
		bool register_out_handler(USB_class_driver* out_handler, uint8_t ep) {
			if(ep >= sizeof(out_handlers) / sizeof(*out_handlers)) {
				return false;
			}
			
			out_handlers[ep] = out_handler;
			
			return true;
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
			// TODO: Handle setting configuration after configuration is set.
			
			if(wValue == current_configuration) {
				// Keeping current configuration.
				
			} else if(current_configuration) {
				// Refusing to set a new configuration.
				return false;
				
			} else if(wValue == 1) {
				// Setting configuration.
				current_configuration = wValue;
				
				for(USB_class_driver*& driver : class_drivers) {
					if(driver) {
						driver->handle_set_configuration(wValue);
					}
				}
				
			} else {
				// Unknown configuration.
				return false;
			}
			
			write(0, 0, 0);
			return true;
		}
		
		void handle_reset() {
			current_configuration = 0;
			
			hw_conf_ep(0, EPType::Control, 64);
		}
		
		void handle_setup(const uint32_t* bufp) {
			uint8_t bmRequestType = bufp[0] & 0xff;
			uint8_t bRequest = (bufp[0] >> 8) & 0xff;
			uint16_t wValue = (bufp[0] >> 16) & 0xffff;
			uint16_t wIndex = bufp[1] & 0xffff;
			uint16_t wLength = (bufp[1] >> 16) & 0xffff;
			
			usb_rblog.log("handle_setup, bmRequestType=%02x, bRequest=%02x", bmRequestType, bRequest);
			
			out_handlers[0] = nullptr;
			
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
			
			// SET_INTERFACE
			if(bmRequestType == 0x01 && bRequest == 0x0b) {
				// TODO: Don't ignore this request.
				write(0, nullptr, 0);
				return;
			}
			
			SetupStatus res = SetupStatus::Unhandled;
			
			for(USB_class_driver*& driver : class_drivers) {
				if(driver) {
					res = driver->handle_setup(bmRequestType, bRequest, wValue, wIndex, wLength);
					
					if(res == SetupStatus::Ok) {
						out_handlers[0] = driver;
					}
					
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
		
		void handle_out(uint8_t ep, uint32_t len) {
			usb_rblog.log("handle_out, ep=%02x, len=%d", ep, len);
			
			if(out_handlers[ep]) {
				out_handlers[ep]->handle_out(ep, len);
			}
		}
};

#endif
