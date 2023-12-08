//--------------------------------------------------
// BLDC Motor Controller
// attaConnectorPlatform.h
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef UTILS_ATTA_CONNECTOR_PLATFORM_H
#define UTILS_ATTA_CONNECTOR_PLATFORM_H
#include "attaConnectorCmds.h"
#include <cstdint>

namespace AttaConnector {

constexpr uint32_t MAX_PENDING_PACKETS = 1024; // Maximum number of pending packets
constexpr uint32_t RX_SIZE = 1024;             // 1KB RX memory
constexpr uint32_t TX_SIZE = 30 * 1024;        // 30KB TX memory
constexpr uint32_t MAX_CMD_SIZE = 1024;        // Maximum command size in byte

} // namespace AttaConnector

#endif // UTILS_ATTA_CONNECTOR_PLATFORM_H
