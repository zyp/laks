#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>

struct ETH_MAC_reg_t {
	volatile uint32_t CR;
	volatile uint32_t FFR;
	volatile uint32_t HTHR;
	volatile uint32_t HTLR;
	volatile uint32_t MIIAR;
	volatile uint32_t MIIDR;
	volatile uint32_t FCR;
	volatile uint32_t VLANTR;
	uint32_t _reserved[2];
	volatile uint32_t RWUFFR;
	volatile uint32_t PMTCSR;
	uint32_t _reserved1;
	volatile uint32_t DBGR;
	volatile uint32_t SR;
	volatile uint32_t IMR;
	volatile uint32_t AR[4][2];
};

struct ETH_MMC_reg_t {
	volatile uint32_t CR;
	volatile uint32_t RIR;
	volatile uint32_t TIR;
	volatile uint32_t RIMR;
	volatile uint32_t TIMR;
	uint32_t _reserved[14];
	volatile uint32_t TGFSCCR;
	volatile uint32_t TGFMSCCR;
	uint32_t _reserved1[5];
	volatile uint32_t TGFCR;
	uint32_t _reserved2[10];
	volatile uint32_t RFCECR;
	volatile uint32_t RFAECR;
	uint32_t _reserved3[11];
	volatile uint32_t RGUFCR;
};

struct ETH_PTP_reg_t {
	volatile uint32_t TSCR;
	volatile uint32_t SSIR;
	volatile uint32_t TSHR;
	volatile uint32_t TSLR;
	volatile uint32_t TSHUR;
	volatile uint32_t TSLUR;
	volatile uint32_t TSAR;
	volatile uint32_t TTHR;
	volatile uint32_t TTLR;
	uint32_t _reserved;
	volatile uint32_t TSSR;
};

struct ETH_DMA_reg_t {
	volatile uint32_t BMR;
	volatile uint32_t TPDR;
	volatile uint32_t RPDR;
	volatile uint32_t RDLAR;
	volatile uint32_t TDLAR;
	volatile uint32_t SR;
	volatile uint32_t OMR;
	volatile uint32_t IER;
	volatile uint32_t MFBOCR;
	volatile uint32_t RSWTR;
	uint32_t _reserved[8];
	volatile uint32_t CHTDR;
	volatile uint32_t CHRDR;
	volatile uint32_t CHTBAR;
	volatile uint32_t CHRBAR;
};

class ETH_t {
	public:
		ETH_MAC_reg_t& mac_reg;
		ETH_MMC_reg_t& mmc_reg;
		ETH_PTP_reg_t& ptp_reg;
		ETH_DMA_reg_t& dma_reg;
		
		ETH_t(uint32_t reg_addr) : 
			mac_reg(*(ETH_MAC_reg_t*)reg_addr),
			mmc_reg(*(ETH_MMC_reg_t*)(reg_addr + 0x100)),
			ptp_reg(*(ETH_PTP_reg_t*)(reg_addr + 0x700)),
			dma_reg(*(ETH_DMA_reg_t*)(reg_addr + 0x1000)) {}
};

#if defined(STM32F4)
static ETH_t ETH(0x40028000);
#endif

#endif

