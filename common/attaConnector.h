//--------------------------------------------------
// Atta Connector
// attaConnector.cpp
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CONNECTOR_H
#define ATTA_CONNECTOR_H
#include "attaConnectorPlatform.h"
#include <cstdint>

namespace AttaConnector {

bool init();
void update();

template <typename T>
bool transmit(const T& cmd);
template <typename T>
bool receive(T* cmd);

bool transmit(uint8_t cmdId, uint8_t* data, uint32_t size);
uint32_t receiveNextSize(uint8_t cmdId);
bool receive(uint8_t cmdId, uint8_t* data, uint32_t* size);

} // namespace AttaConnector

#include "attaConnector.inl"
#endif // ATTA_CONNECTOR_H
