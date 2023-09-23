//--------------------------------------------------
// BLDC Motor Controller
// attaConnectorPlatform.cpp
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <queue>
#include <utils/attaConnectorPlatform.h>

static std::queue<uint8_t> buffer;

namespace AttaConnector {

bool transmitBytes(uint8_t* data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++)
        buffer.push(data[i]);
    return true;
}

uint32_t numAvailableBytes() { return buffer.size(); }

bool receiveBytes(uint8_t* data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++)
        data[i] = buffer.front();
    buffer.pop();
    return true;
}

} // namespace AttaConnector
