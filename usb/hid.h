#ifndef HID_H
#define HID_H

struct HID_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdHID;
	uint8_t bCountryCode;
	uint8_t bNumDescriptors;
	uint8_t bDescriptorType0;
	uint16_t wDescriptorLength0;
} __attribute__((packed));

constexpr HID_desc hid_desc(
		uint16_t bcdHID,
		uint8_t bCountryCode,
		uint8_t bNumDescriptors,
		uint8_t bDescriptorType,
		uint16_t wDescriptorLength
	) {
	
	return {
		sizeof(HID_desc),
		0x21,
		bcdHID,
		bCountryCode,
		bNumDescriptors,
		bDescriptorType,
		wDescriptorLength
	};
}

template <typename T>
struct HID_Item {
	uint8_t tag;
	T data;
} __attribute__((packed));

template <>
struct HID_Item<void> {
	uint8_t tag;
} __attribute__((packed));

constexpr HID_Item<void> hid_item(uint8_t tag) {
	return {tag};
}

template <typename T>
constexpr uint8_t hid_item_data_size() {
	static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4, "item has invalid size");
	
	return sizeof(T) == 1 ? 1 : sizeof(T) == 2 ? 2 : 3;
}

template <typename T>
constexpr HID_Item<T> hid_item(uint8_t tag, T data) {
	return HID_Item<T>{uint8_t(tag | hid_item_data_size<T>()), data};
}

namespace UsagePage {
	enum _8 : uint8_t {
		Undefined,
		Desktop,
		Simulation,
		VR,
		Sport,
		Game,
		Generic,
		Keyboard,
		LED,
		Button,
		Ordinal,
	};
}

constexpr HID_Item<UsagePage::_8> usage_page(UsagePage::_8 page) {
	return hid_item(0x04, page);
}

constexpr HID_Item<uint16_t> usage_page(uint16_t page) {
	return hid_item(0x04, page);
}

namespace DesktopUsage {
	enum _8 : uint8_t {
		Undefined,
		Gamepad = 0x05,
		X = 0x30,
		Y,
		Z,
		Rx,
		Ry,
		Rz,
	};
}

enum class Collection : uint8_t {
	Physical,
	Application,
	Logical,
};

constexpr HID_Item<DesktopUsage::_8> usage(DesktopUsage::_8 usage) {
	return hid_item(0x08, usage);
}

constexpr HID_Item<uint16_t> usage(uint16_t usage) {
	return hid_item(0x08, usage);
}

template <typename... R>
constexpr auto collection(Collection type, R... r) -> decltype(pack(HID_Item<Collection>(), r..., HID_Item<void>())) {
	return pack(hid_item(0xa0, type), r..., hid_item(0xc0));
}

constexpr HID_Item<int16_t> usage_minimum(int16_t x) {
	return hid_item(0x18, x);
}

constexpr HID_Item<int16_t> usage_maximum(int16_t x) {
	return hid_item(0x28, x);
}

constexpr HID_Item<int32_t> logical_minimum(int32_t x) {
	return hid_item(0x14, x);
}

constexpr HID_Item<int32_t> logical_maximum(int32_t x) {
	return hid_item(0x24, x);
}

constexpr HID_Item<uint8_t> report_count(uint8_t x) {
	return hid_item(0x94, x);
}

constexpr HID_Item<uint8_t> report_size(uint8_t x) {
	return hid_item(0x74, x);
}

constexpr HID_Item<uint8_t> input(uint8_t x) {
	return hid_item(0x80, x);
}

constexpr HID_Item<uint8_t> output(uint8_t x) {
	return hid_item(0x90, x);
}

constexpr HID_Item<uint8_t> feature(uint8_t x) {
	return hid_item(0xb0, x);
}

constexpr HID_Item<uint8_t> report_id(uint8_t id) {
	return hid_item(0x84, id);
}

template <typename... R>
constexpr auto gamepad(R... r) -> decltype(
	pack(
		usage_page(UsagePage::Desktop),
		usage(DesktopUsage::Gamepad),
		collection(Collection::Application,
			collection(Collection::Physical,
				r...
			)
		)
	)) {
	return pack(
		usage_page(UsagePage::Desktop),
		usage(DesktopUsage::Gamepad),
		collection(Collection::Application,
			collection(Collection::Physical,
				r...
			)
		)
	);
}

constexpr auto buttons(uint8_t num) -> decltype(
	pack(
		usage_page(UsagePage::Button),
		usage_minimum(1),
		usage_maximum(num),
		logical_minimum(0),
		logical_maximum(1),
		report_count(num),
		report_size(1),
		input(0x02)
	)) {
	return pack(
		usage_page(UsagePage::Button),
		usage_minimum(1),
		usage_maximum(num),
		logical_minimum(0),
		logical_maximum(1),
		report_count(num),
		report_size(1),
		input(0x02)
	);
}

constexpr auto padding_in(uint8_t size) -> decltype(
	pack(
		report_count(1),
		report_size(size),
		input(0x01)
	)) {
	return pack(
		report_count(1),
		report_size(size),
		input(0x01)
	);
}

constexpr auto padding_out(uint8_t size) -> decltype(
	pack(
		report_count(1),
		report_size(size),
		output(0x01)
	)) {
	return pack(
		report_count(1),
		report_size(size),
		output(0x01)
	);
}

class USB_HID : public USB_class_driver {
	protected:
		USB_generic& usb;
		
	private:
		desc_t report_desc_p;
		
		enum ControlState {
			None,
			SetOutputReport,
			SetFeatureReport,
		};
		
		const uint8_t interface_num;
		const uint8_t in_ep;
		const uint8_t in_ep_mps;
		
		ControlState controlstate;
		
		uint32_t buf[16];
	
	public:
		USB_HID(USB_generic& usbd, desc_t rdesc, uint8_t interface, uint8_t endpoint, uint8_t mps) : usb(usbd), report_desc_p(rdesc), interface_num(interface), in_ep(endpoint), in_ep_mps(mps) {
			usb.register_driver(this);
		}
	
	protected:
		virtual SetupStatus handle_setup(uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
			controlstate = None;
			
			// Get report descriptor.
			if(bmRequestType == 0x81 && wIndex == interface_num && bRequest == 0x06 && wValue == 0x2200) {
				if(wLength > report_desc_p.size) {
					wLength = report_desc_p.size;
				}
				
				uint32_t* p = (uint32_t*)report_desc_p.data;
				
				while(wLength >= 64) {
					usb.write(0, p, 64);
					p += 64/4;
					wLength -= 64;
					
					while(!usb.ep_ready(0));
				}
				
				usb.write(0, p, wLength);
				return SetupStatus::Ok;
			}
			
			// Set output report.
			if(bmRequestType == 0x21 && wIndex == interface_num && bRequest == 0x09 && (wValue & 0xff00) == 0x0200) {
				controlstate = SetOutputReport;
				return SetupStatus::Ok;
			}
			
			// Set feature report.
			if(bmRequestType == 0x21 && wIndex == interface_num && bRequest == 0x09 && (wValue & 0xff00) == 0x0300) {
				controlstate = SetFeatureReport;
				return SetupStatus::Ok;
			}
			
			// Get feature report.
			if(bmRequestType == 0xa1 && wIndex == interface_num && bRequest == 0x01 && (wValue & 0xff00) == 0x0300) {
				return get_feature_report(wValue & 0xff) ? SetupStatus::Ok : SetupStatus::Stall;
			}
			
			return SetupStatus::Unhandled;
		}
		
		virtual void handle_set_configuration(uint8_t configuration) {
			if(configuration) {
				usb.hw_conf_ep(0x80 | in_ep, EPType::Interrupt, in_ep_mps);
			}
		}
		
		virtual void handle_out(uint8_t ep, uint32_t len) {
			if(ep != 0) {
				return;
			}
			
			bool res = false;
			
			switch(controlstate) {
				case SetOutputReport:
					usb.read(ep, buf, len);
					res = set_output_report(buf, len);
					break;
				
				case SetFeatureReport:
					usb.read(ep, buf, len);
					res = set_feature_report(buf, len);
					break;
				
				default:
					break;
			}
			if(res) {
				usb.write(0, nullptr, 0);
			} else {
				usb.hw_set_stall(0);
			}
		}
		
		virtual bool set_output_report(uint32_t* buf, uint32_t len) {
			return false;
		}
		
		virtual bool set_feature_report(uint32_t* buf, uint32_t len) {
			return false;
		}
		
		virtual bool get_feature_report(uint8_t report_id) {
			return false;
		}
};

#endif
