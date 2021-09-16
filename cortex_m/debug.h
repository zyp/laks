#pragma once

#include <stdint.h>

#include <mmio/mmio.h>

struct ITM_reg_t {
	union {
		volatile uint32_t u32;
		volatile uint16_t u16;
		volatile uint8_t u8;
	} STIM[256];
    uint32_t _reserved[640];
    volatile uint32_t TER[8];
    uint32_t _reserved2[8];
    volatile uint32_t TPR;
    uint32_t _reserved3[15];
    volatile uint32_t TCR;
    uint32_t _reserved4[76];
    volatile uint32_t LSR;
};

struct DWT_reg_t {
    volatile uint32_t CTRL;
    volatile uint32_t CYCCNT;
    volatile uint32_t CPICNT;
    volatile uint32_t EXCCNT;
    volatile uint32_t SLEEPCNT;
    volatile uint32_t LSUCNT;
    volatile uint32_t FOLDCNT;
    volatile uint32_t PCSR;
};

struct TPIU_reg_t {
    volatile uint32_t SSPSR;
    volatile uint32_t CSPSR;
    uint32_t _reserved[2];
    volatile uint32_t ACPR;
    uint32_t _reserved2[55];
    volatile uint32_t SPPR;
};

constexpr mmio_ptr<ITM_reg_t> ITM {0xe0000000};
constexpr mmio_ptr<DWT_reg_t> DWT {0xe0001000};
constexpr mmio_ptr<TPIU_reg_t> TPIU {0xe0040000};