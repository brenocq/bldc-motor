//--------------------------------------------------
// Atta Connector
// attaConnectorCmds.h
// Date: 2023-10-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_ATTA_CONNECTOR_CMDS_H
#define BLDC_ATTA_CONNECTOR_CMDS_H
#include <cstdint>

// List of command codes
enum CommandCode : uint8_t {
    MY_TEST0_CMD = 0x00,
    MY_TEST1_CMD = 0x01,
    RESERVED_CMD = 0xFF, // CMD reserved for atta connector internal use
};

// Macro to define command id
#define ATTA_CONNECTOR_CMD(x) static constexpr uint8_t CMD_ID = x

struct MyTest0 {
    ATTA_CONNECTOR_CMD(MY_TEST0_CMD);
    uint8_t u0;
    uint8_t u1;
};

struct MyTest1 {
    ATTA_CONNECTOR_CMD(MY_TEST1_CMD);
    float f;
    uint8_t u;
};

#endif // BLDC_ATTA_CONNECTOR_PLATFORM_H
