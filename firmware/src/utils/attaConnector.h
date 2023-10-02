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

// Internal
uint32_t getTxSize();
uint32_t getRxSize();
void txPush(uint8_t data);
uint8_t rxPop();

uint8_t crc(uint8_t* message, uint32_t size);
void cobsEncode(uint8_t* message, uint32_t size, uint8_t* encoded, uint32_t* encodedSize);

} // namespace AttaConnector

#include "attaConnector.inl"
#endif // ATTA_CONNECTOR_H
