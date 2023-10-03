//--------------------------------------------------
// BLDC Motor Controller
// attaConnectorPlatform.cpp
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <queue>
#include <utils/attaConnectorPlatform.h>
#include <utils/log.h>

static std::queue<uint8_t> buffer;

namespace AttaConnector {

bool transmitBytes(uint8_t* data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        LOG_DEBUG("AttaConnector", "TX $x0", (int)data[i]);
        buffer.push(data[i]);
    }
    return true;
}

uint32_t numAvailableBytes() { return buffer.size(); }

bool receiveBytes(uint8_t* data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        data[i] = buffer.front();
        buffer.pop();
        LOG_DEBUG("AttaConnector", "RX $x0", (int)data[i]);
    }
    return true;
}

} // namespace AttaConnector
