//--------------------------------------------------
// Atta Connector
// attaConnector.inl
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <array>

template <typename T>
bool AttaConnector::transmit(const T& cmd) {
    if (transmit(T::CMD_ID, (uint8_t*)&cmd, sizeof(cmd)))
        return true;
    return false;
}

template <typename T>
bool AttaConnector::receive(T* cmd) {
    uint32_t len = 0;
    if (receive(T::CMD_ID, (uint8_t*)cmd, &len) && sizeof(T) == len)
        return true;
    return false;
}
