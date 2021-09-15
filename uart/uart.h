#pragma once

#include "stm32_uart.h"

constexpr STM32_UART_t<STM32_UART_reg_v1_t> USART1{
        0x40011000,
};
constexpr STM32_UART_t<STM32_UART_reg_v1_t> USART2{
        0x40004400,
};


