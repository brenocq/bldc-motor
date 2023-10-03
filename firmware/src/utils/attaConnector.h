//--------------------------------------------------
// BLDC Motor Controller
// attaConnector.cpp
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CONNECTOR_H
#define ATTA_CONNECTOR_H
#include "attaConnectorPlatform.h"
#include <cstdint>

// Macro to define command id
#define ATTA_CONNECTOR_CMD(x) static constexpr uint8_t CMD_ID = x

namespace AttaConnector {

bool init();
void update();

template <typename T>
bool transmit(const T& cmd);
template <typename T>
bool receive(T* cmd);

bool transmit(uint8_t cmdId, uint8_t* data, uint32_t len);
bool receive(uint8_t cmdId, uint8_t* data, uint32_t* len);

} // namespace AttaConnector

#include "attaConnector.inl"
#endif // ATTA_CONNECTOR_H
