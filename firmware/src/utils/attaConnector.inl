//--------------------------------------------------
// BLDC Motor Controller
// attaConnector.inl
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <utils/log.h>

template <typename T>
bool AttaConnector::transmit(const T& cmd) {
    LOG_DEBUG("AttaConnector", "CRC is $0", (int)crc((uint8_t*)&cmd, sizeof(T)));
    return false;
}

template <typename T>
bool AttaConnector::receive(T* cmd) {
    return false;
}
