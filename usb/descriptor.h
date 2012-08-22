#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <stdint.h>

struct Device_desc {
	enum {
		SIZE = 18,
	};
	
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
};


struct Configuration_desc {
	enum {
		SIZE = 9,
	};
	
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationvalue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;
} __attribute__((packed));

struct Interface_desc {
	enum {
		SIZE = 9,
	};
	
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

struct Endpoint_desc {
	enum {
		SIZE = 7,
	};
	
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
} __attribute__((packed));

struct CDC_Header_desc {
	enum {
		SIZE = 5,
	};
	
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint16_t bcdCDC;
} __attribute__((packed));

struct CDC_Call_Management_desc {
	enum {
		SIZE = 5,
	};
	
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmCapabilities;
	uint8_t bDataInterface;
} __attribute__((packed));

struct CDC_ACM_desc {
	enum {
		SIZE = 4,
	};
	
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmCapabilities;
} __attribute__((packed));

struct CDC_Union_desc {
	enum {
		SIZE = 5,
	};
	
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bControlInterface;
	uint8_t bSubordinateInterface0;
} __attribute__((packed));

template <typename A, typename B>
struct X {
	enum {
		SIZE = A::SIZE + B::SIZE,
	};
	
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

#endif
