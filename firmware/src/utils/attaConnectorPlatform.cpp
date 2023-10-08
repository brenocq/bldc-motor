//--------------------------------------------------
// BLDC Motor Controller
// attaConnectorPlatform.cpp
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/uart/uart.h>
#include <queue>
#include <utils/attaConnectorPlatform.h>
#include <utils/log.h>

static std::queue<uint8_t> buffer;

namespace AttaConnector {

bool transmitBytes(uint8_t* data, uint32_t size) {
    if (Uart::isInitialized())
        Uart::transmit(data, size);
    return true;
}

uint32_t receiveBytes(uint8_t* data, uint32_t size) {
    uint32_t numBytes = 0;
    if (Uart::isInitialized())
        numBytes = Uart::receive(data, size);
    return numBytes;
}

} // namespace AttaConnector
