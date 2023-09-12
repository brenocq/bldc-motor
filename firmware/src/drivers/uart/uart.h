//--------------------------------------------------
// BLDC Motor Controller
// uart.h
// Date: 2023-09-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_UART_UART_H
#define BLDC_DRIVERS_UART_UART_H
#include <system/hal.h>
#include <string>
#include <vector>

namespace Uart {

constexpr uint32_t baudrate = 115200;
enum class Peripheral : uint8_t { DEFAULT = 0, UART1, UART2, UART3, UART4, UART5, UART6 };

bool init();

void transmit(std::string data, Peripheral peripheral = Peripheral::DEFAULT);

std::vector<uint8_t> receive(uint32_t maxChars, uint32_t timeout, Peripheral peripheral = Peripheral::DEFAULT);

} // namespace Uart

#endif // BLDC_DRIVERS_UART_UART_H
