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

// Macro to define command code
#define ATTA_CONNECTOR_CMD(x) static constexpr uint8_t CMD_CODE = x

namespace AttaConnector {

bool init();
void update();

template <typename T>
bool transmit(const T& cmd);
template <typename T>
bool receive(T* cmd);

uint8_t crc(uint8_t* message, uint32_t size);

} // namespace AttaConnector

#include "attaConnector.inl"
#endif // ATTA_CONNECTOR_H
