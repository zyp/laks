#pragma once
/**
 * Calibration helpers, offsets for ROM calibration variables and similar
 */

namespace STM32 {
	namespace Calibration {

#if defined(STM32WB)
		static auto const TS_CAL1_TEMP = 30;
		static uint16_t& TS_CAL1 = *(uint16_t*) 0x1FFF75A8;
		static auto const TS_CAL2_TEMP = 130;
		static uint16_t& TS_CAL2 = *(uint16_t*) 0x1FFF75cA;
		static auto const TS_CAL_VOLTAGE = 3.0f;
		static auto const TS_MIN_SAMPLING_USECS = 5;

		static uint16_t& VREFINT_CAL = *(uint16_t*) 0x1FFF75AA;
		static auto const VREFINT_CAL_TEMP = 30;
		static auto const VREFINT_CAL_VOLTAGE = 3.6f;
		static auto const VREFINT_MIN_SAMPLING_USECS = 4;

		/* A 32bit unique sequence for each device */
		static uint32_t& UID64_UDN = *(uint32_t*) 0x1fff7580;
		/* 24 bit vendor id, and an 8 bit part id, */
		static uint32_t& UID64_PART = *(uint32_t*) 0x1fff7584;
		static auto const UID64_VENDOR_ST = 0x0080E1;
		static auto const UID64_PART_STM32WB_REV_YA = 0x26;
		static auto const UID64_PART_STM32WB_REV_XB = 0x27;		
#endif

	}
}