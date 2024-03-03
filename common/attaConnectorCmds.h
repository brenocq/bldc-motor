//--------------------------------------------------
// Atta Connector
// attaConnectorCmds.h
// Date: 2023-10-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_ATTA_CONNECTOR_CMDS_H
#define BLDC_ATTA_CONNECTOR_CMDS_H
#include <array>
#include <cstdint>

// List of command codes
enum CommandCode : uint8_t {
    MY_TEST0_CMD = 0x00,
    MY_TEST1_CMD = 0x01,
    MOTOR_STATE_CMD = 0x02,
    IMU_STATE_CMD = 0x03,
};

struct MyTest0 {
    static constexpr uint8_t CMD_ID = MY_TEST0_CMD;
    uint8_t u0;
    uint8_t u1;
};

struct MyTest1 {
    static constexpr uint8_t CMD_ID = MY_TEST1_CMD;
    float f;
    uint8_t u;
};

struct MotorState {
    static constexpr uint8_t CMD_ID = MOTOR_STATE_CMD;
    float sourceVoltage;
    std::array<float, 3> phaseCurrent;
    std::array<float, 3> phaseVoltage;
    float rotorPosition;
};

struct ImuState {
    static constexpr uint8_t CMD_ID = IMU_STATE_CMD;
    std::array<int16_t, 3> acc;
    std::array<int16_t, 3> gyr;
};

#endif // BLDC_ATTA_CONNECTOR_PLATFORM_H
