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

#endif
