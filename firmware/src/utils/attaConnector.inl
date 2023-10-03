//--------------------------------------------------
// BLDC Motor Controller
// attaConnector.inl
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <array>

namespace AttaConnector {
// Internal
bool pushPacket(uint8_t cmdId, uint8_t* payload, uint32_t payloadSize);
bool popPacket(uint8_t cmdId, uint8_t* payload, uint32_t payloadSize);
} // namespace AttaConnector

template <typename T>
bool AttaConnector::transmit(const T& cmd) {
    if (pushPacket(T::CMD_ID, (uint8_t*)&cmd, sizeof(cmd)))
        return true;
    return false;
}

template <typename T>
bool AttaConnector::receive(T* cmd) {
    if (popPacket(T::CMD_ID, (uint8_t*)&cmd, sizeof(cmd)))
        return true;
    return false;
}
