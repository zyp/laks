#pragma once

#include <stdint.h>
#include <mmio/mmio.h>

// Also f0 and f3
struct STM32_FLASH_reg_f1_t {
	volatile uint32_t ACR;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t AR;
	volatile uint32_t RESERVED;
	volatile uint32_t OBR;
	volatile uint32_t WRPR;
};

struct STM32_FLASH_reg_f4_t {
	volatile uint32_t ACR;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t OPTCR;
};

struct STM32_FLASH_reg_l0_t {
	volatile uint32_t ACR;
	volatile uint32_t PECR;
	volatile uint32_t PDKEYR;
	volatile uint32_t PEKEYR;
	volatile uint32_t PRGKEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t OPTR;
	volatile uint32_t WRPROT;
};

struct STM32_FLASH_reg_l4_t {
	volatile uint32_t ACR;
	volatile uint32_t PDKEYR;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t ECCR;
	uint32_t _reserved1;
	volatile uint32_t OPTR;
	volatile uint32_t PCROP1SR;
	volatile uint32_t PCROP1ER;
	volatile uint32_t WRP1AR;
	volatile uint32_t WRP1BR;
	uint32_t _reserved2[4];
	volatile uint32_t PCROP2SR;
	volatile uint32_t PCROP2ER;
	volatile uint32_t WRP2AR;
	volatile uint32_t WRP2BR;
};

struct STM32_FLASH_reg_g4_t {
	volatile uint32_t ACR;
	volatile uint32_t PDKEYR;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t ECCR;
	uint32_t _reserved1;
	volatile uint32_t OPTR;
	volatile uint32_t PCROP1SR;
	volatile uint32_t PCROP1ER;
	volatile uint32_t WRP1AR;
	volatile uint32_t WRP1BR;
	uint32_t _reserved2[4];
	volatile uint32_t PCROP2SR;
	volatile uint32_t PCROP2ER;
	volatile uint32_t WRP2AR;
	volatile uint32_t WRP2BR;
	uint32_t _reserved3[7];
	volatile uint32_t SEC1R;
	volatile uint32_t SEC2R;
};

struct STM32_FLASH_reg_wb_t {
	volatile uint32_t ACR;
	uint32_t _reserved1;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t ECCR;
	uint32_t _reserved2;
	volatile uint32_t OPTR;
	volatile uint32_t PCROP1ASR;
	volatile uint32_t PCROP1AER;
	volatile uint32_t WRP1AR;
	volatile uint32_t WRP1BR;
	volatile uint32_t PCROP1BSR;
	volatile uint32_t PCROP1BER;
	volatile uint32_t IPCCBR;
	uint32_t _reserved3[8];
	volatile uint32_t C2ACR;
	volatile uint32_t C2SR;
	volatile uint32_t C2CR; // 0x64
	uint32_t _reserved4[7];
	volatile uint32_t SFR; // 0x80
	volatile uint32_t SRRVR;
};

template <typename T>
class STM32_FLASH_t : public mmio_ptr<T> {
    public:
        using mmio_ptr<T>::ptr;
};
