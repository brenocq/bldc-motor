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
    LOG_DEBUG("AttaConnector", "TX begin");
    for (uint32_t i = 0; i < len; i++) {
        LOG_DEBUG("AttaConnector", "$x0", (int)data[i]);
        buffer.push(data[i]);
    }
    LOG_DEBUG("AttaConnector", "TX end");
    return true;
}

uint32_t numAvailableBytes() { return buffer.size(); }

bool receiveBytes(uint8_t* data, uint32_t len) {
    LOG_DEBUG("AttaConnector", "RX begin");
    for (uint32_t i = 0; i < len; i++) {
        data[i] = buffer.front();
        LOG_DEBUG("AttaConnector", "$x0", (int)data[i]);
    }
    LOG_DEBUG("AttaConnector", "RX end");
    buffer.pop();
    return true;
}

} // namespace AttaConnector
