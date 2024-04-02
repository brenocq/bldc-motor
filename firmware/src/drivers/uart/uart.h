//--------------------------------------------------
// BLDC Motor Controller
// uart.h
// Date: 2023-09-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_UART_UART_H
#define BLDC_DRIVERS_UART_UART_H
#include <drivers/dma/dma.h>
#include <string>
#include <system/hal.h>
#include <vector>

/**
 * @brief UART Driver
 *
 * Transmits and receives using the default UART peripheral. The default peripheral is set as the only UART peripheral that was initialized in the
 * GPIO list
 *
 * @warning For now it only works with one UART peripheral
 */
namespace Uart {

constexpr uint32_t baudrate = 115200;
enum class Peripheral : uint8_t { DEFAULT = 0, UART1, UART2, UART3, UART4, UART5, UART6 };
constexpr uint32_t TX_BUFFER_SIZE = 1024;
constexpr uint32_t RX_BUFFER_SIZE = 1024;

using Handle = UART_HandleTypeDef;

bool init();

bool isInitialized();

void update();

Handle* getHandle(Peripheral peripheral);

void transmit(uint8_t* data, uint32_t size);

uint32_t receive(uint8_t* data, uint32_t size);

void linkDmaTx(Peripheral peripheral, Dma::Handle* dmaHandle);
void linkDmaRx(Peripheral peripheral, Dma::Handle* dmaHandle);

} // namespace Uart

#endif // BLDC_DRIVERS_UART_UART_H
