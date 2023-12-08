//--------------------------------------------------
// BLDC Simulation
// attaConnectorPlatform.h
// Date: 2023-10-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_ATTA_CONNECTOR_PLATFORM_H
#define BLDC_ATTA_CONNECTOR_PLATFORM_H
#include "attaConnectorCmds.h"
#include <cstdint>

namespace AttaConnector {

constexpr uint32_t MAX_PENDING_PACKETS = 1024; // Maximum number of pending packets
constexpr uint32_t RX_SIZE = 10 * 1024;        // 10KB RX memory
constexpr uint32_t TX_SIZE = 10 * 1024;        // 10KB TX memory
constexpr uint32_t MAX_CMD_SIZE = 1024;        // Maximum command size in byte

} // namespace AttaConnector

#endif // BLDC_ATTA_CONNECTOR_PLATFORM_H
