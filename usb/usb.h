#ifndef USB_H
#define USB_H

#if defined(STM32F1) || defined(STM32F3)
#include "f1_usb.h"

static F1_USB_t USB(0x40005c00, 0x40006000);

#elif defined(STM32F4)
#include "dwc_otg.h"

static DWC_OTG_t OTG_FS(0x50000000);
static DWC_OTG_t OTG_HS(0x40040000);

#endif

#endif
