module;

#include <mmio/mmio.h>

export module laks.periph.usb.stm32;

export struct STM32_USB_reg_v1_t {
    volatile uint32_t EPR[16];
    volatile uint32_t CNTR;
    volatile uint32_t ISTR;
    volatile uint32_t FNR;
    volatile uint32_t DADDR;
    volatile uint32_t BTABLE;

    struct bufd_t {
        volatile uint32_t ADDR_TX;
        volatile uint32_t COUNT_TX;
        volatile uint32_t ADDR_RX;
        volatile uint32_t COUNT_RX;
    };

    using buf_t = volatile uint32_t;
};

export struct STM32_USB_reg_v2_t {
    volatile uint32_t EPR[16];
    volatile uint32_t CNTR;
    volatile uint32_t ISTR;
    volatile uint32_t FNR;
    volatile uint32_t DADDR;
    volatile uint32_t BTABLE;
    volatile uint32_t LPMCSR;
    volatile uint32_t BCDR;

    struct bufd_t {
        volatile uint16_t ADDR_TX;
        volatile uint16_t COUNT_TX;
        volatile uint16_t ADDR_RX;
        volatile uint16_t COUNT_RX;
    };

    using buf_t = volatile uint16_t;
};

export struct STM32_USB_reg_v3_t {
    volatile uint32_t EPR[16];
    volatile uint32_t CNTR;
    volatile uint32_t ISTR;
    volatile uint32_t FNR;
    volatile uint32_t DADDR;
    uint32_t _reserved;
    volatile uint32_t LPMCSR;
    volatile uint32_t BCDR;

    struct bufd_t {
        volatile uint32_t TXRXBD;
        volatile uint32_t RXTXBD;
    };

    using buf_t = volatile uint32_t;
};

export template <typename T>
class STM32_USB_t : public mmio_ptr<T> {
	public:
		mmio_ptr<typename T::bufd_t> bufd;
		mmio_ptr<typename T::buf_t> buf;
		
		constexpr STM32_USB_t(uint32_t reg_addr, uint32_t buf_addr) :
			mmio_ptr<T>(reg_addr),
			bufd(buf_addr),
            buf(buf_addr) {}
};
