//--------------------------------------------------
// BLDC Motor Controller
// uart.h
// Date: 2023-09-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_UART_UART_H
#define BLDC_DRIVERS_UART_UART_H
#include <string>
#include <system/hal.h>
#include <vector>

namespace Uart {

constexpr uint32_t baudrate = 115200;
enum class Peripheral : uint8_t { DEFAULT = 0, UART1, UART2, UART3, UART4, UART5, UART6 };

bool init();

bool isInitialized();

void transmit(uint8_t* data, uint32_t size, Peripheral peripheral = Peripheral::DEFAULT);

uint32_t receive(uint8_t* data, uint32_t size, Peripheral peripheral = Peripheral::DEFAULT);

} // namespace Uart

#endif // BLDC_DRIVERS_UART_UART_H
