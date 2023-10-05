//--------------------------------------------------
// BLDC Motor Controller
// attaConnectorPlatform.h
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef UTILS_ATTA_CONNECTOR_PLATFORM_H
#define UTILS_ATTA_CONNECTOR_PLATFORM_H
#include <cstdint>

// List of command codes
enum CommandCode : uint8_t {
    MY_TEST0_CMD = 0x00,
    MY_TEST1_CMD = 0x01,
    RESERVED_CMD = 0xFF, // CMD reserved for atta connector internal use
};

namespace AttaConnector {

constexpr uint32_t RX_SIZE = 10 * 1024; // 10KB RX memory
constexpr uint32_t TX_SIZE = 10 * 1024; // 10KB TX memory
constexpr uint32_t MAX_CMD_SIZE = 1024; // Maximum command size in byte

} // namespace AttaConnector

#endif // UTILS_ATTA_CONNECTOR_PLATFORM_H
