#ifndef DFU_H
#define DFU_H

struct DFU_Functional_desc {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bmAttributes;
	uint16_t wDetachTimeOut;
	uint16_t wTransferSize;
	uint16_t bcdDFUVersion;
} __attribute__((packed));

constexpr DFU_Functional_desc dfu_functional_desc(
		uint8_t bmAttributes,
		uint16_t wDetachTimeOut,
		uint16_t wTransferSize,
		uint16_t bcdDFUVersion
	) {
	
	return {
		sizeof(DFU_Functional_desc),
		0x21,
		bmAttributes,
		wDetachTimeOut,
		wTransferSize,
		bcdDFUVersion
	};
}

#endif
