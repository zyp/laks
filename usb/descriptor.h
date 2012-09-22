#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <stdint.h>

template <typename A, typename B>
struct X {
	A a;
	B b;
} __attribute__((packed));

template <typename A>
constexpr A pack(A a) {
	return a;
}

template <typename A, typename... R>
constexpr auto pack(A a, R... r) -> X<A, decltype(pack(r...))> {
	return {a, pack(r...)};
}

struct Device_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdUSB;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
} __attribute__((packed));

constexpr Device_desc device_desc(
		uint16_t bcdUSB,
		uint8_t bDeviceClass,
		uint8_t bDeviceSubClass,
		uint8_t bDeviceProtocol,
		uint8_t bMaxPacketSize0,
		uint16_t idVendor,
		uint16_t idProduct,
		uint16_t bcdDevice,
		uint8_t iManufacturer,
		uint8_t iProduct,
		uint8_t iSerialNumber,
		uint8_t bNumConfigurations
	) {
	
	return {
		sizeof(Device_desc),
		1,
		bcdUSB,
		bDeviceClass,
		bDeviceSubClass,
		bDeviceProtocol,
		bMaxPacketSize0,
		idVendor,
		idProduct,
		bcdDevice,
		iManufacturer,
		iProduct,
		iSerialNumber,
		bNumConfigurations
	};
}

struct Configuration_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationvalue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;
} __attribute__((packed));

template <typename... R>
constexpr auto configuration_desc(
		uint8_t bNumInterfaces,
		uint8_t bConfigurationvalue,
		uint8_t iConfiguration,
		uint8_t bmAttributes,
		uint8_t bMaxPower,
		R... r
	) -> decltype(pack(Configuration_desc(), r...)) {
	
	return pack(Configuration_desc {
		sizeof(Configuration_desc),
		2,
		sizeof(pack(Configuration_desc(), r...)),
		bNumInterfaces,
		bConfigurationvalue,
		iConfiguration,
		bmAttributes,
		bMaxPower
	}, r...);
}

struct Interface_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} __attribute__((packed));

template <typename... R>
constexpr auto interface_desc(
		uint8_t bInterfaceNumber,
		uint8_t bAlternateSetting,
		uint8_t bNumEndpoints,
		uint8_t bInterfaceClass,
		uint8_t bInterfaceSubClass,
		uint8_t bInterfaceProtocol,
		uint8_t iInterface,
		R... r
	) -> decltype(pack(Interface_desc(), r...)) {
	
	return pack(Interface_desc {
		sizeof(Interface_desc),
		4,
		bInterfaceNumber,
		bAlternateSetting,
		bNumEndpoints,
		bInterfaceClass,
		bInterfaceSubClass,
		bInterfaceProtocol,
		iInterface
	}, r...);
}

struct Endpoint_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
	uint8_t bRefresh;
	uint8_t bSynchAddress;
} __attribute__((packed));

constexpr Endpoint_desc endpoint_desc(
		uint8_t bEndpointAddress,
		uint8_t bmAttributes,
		uint16_t wMaxPacketSize,
		uint8_t bInterval,
		uint8_t bRefresh = 0,
		uint8_t bSynchAddress = 0
	) {
	
	return {
		sizeof(Endpoint_desc),
		5,
		bEndpointAddress,
		bmAttributes,
		wMaxPacketSize,
		bInterval,
		bRefresh,
		bSynchAddress
	};
}

struct CDC_Header_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint16_t bcdCDC;
} __attribute__((packed));

constexpr CDC_Header_desc cdc_header_desc(
		uint16_t bcdCDC
	) {
	
	return {
		sizeof(CDC_Header_desc),
		0x24,
		0x00,
		bcdCDC
	};
}

struct CDC_Call_Management_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmCapabilities;
	uint8_t bDataInterface;
} __attribute__((packed));

constexpr CDC_Call_Management_desc cdc_call_management_desc(
		uint8_t bmCapabilities,
		uint8_t bDataInterface
	) {
	
	return {
		sizeof(CDC_Call_Management_desc),
		0x24,
		0x01,
		bmCapabilities,
		bDataInterface
	};
}

struct CDC_ACM_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmCapabilities;
} __attribute__((packed));

constexpr CDC_ACM_desc cdc_acm_desc(
		uint8_t bmCapabilities
	) {
	
	return {
		sizeof(CDC_ACM_desc),
		0x24,
		0x02,
		bmCapabilities
	};
}

struct CDC_Union_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bControlInterface;
	uint8_t bSubordinateInterface0;
} __attribute__((packed));

constexpr CDC_Union_desc cdc_union_desc(
		uint8_t bControlInterface,
		uint8_t bSubordinateInterface0
	) {
	
	return {
		sizeof(CDC_Union_desc),
		0x24,
		0x06,
		bControlInterface,
		bSubordinateInterface0
	};
}

#endif
