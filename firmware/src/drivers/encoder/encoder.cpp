//--------------------------------------------------
// BLDC Motor Controller
// encoder.cpp
// Date: 2025-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/encoder/encoder.h>
#include <drivers/hardware.h>
#include <utils/log.h>

bool Encoder::init(Spi::Peripheral peripheral, Gpio::Gpio chipSelect) {
    _peripheral = peripheral;
    _chipSelect = chipSelect;
    Gpio::write(_chipSelect, true);

    // Initial NOP to initialize SPI (first transaction often fails and triggers the encoder's error flag)
    uint16_t nop_cmd = createCommandFrame(REG_NOP, false);
    transmitReceive(nop_cmd);

    // Verify communication by reading a known register
    if (!verifyCommunication()) {
        Log::error("Encoder", "Failed to initialize");
        return false;
    }

    Log::success("Encoder", "Initialized");
    return true;
}

std::optional<float> Encoder::readAngle() {
    auto response = readRegister(REG_ANGLE);
    if (!response)
        return std::nullopt;

    // Convert the raw 14-bit angle (0-16383) to degrees
    return (response->fields.data / 16383.0f) * 360.0f;
}

std::optional<uint16_t> Encoder::readMagnitude() {
    auto response = readRegister(REG_MAGNITUDE);
    if (!response)
        return std::nullopt;
    return uint16_t(response->fields.data);
}

bool Encoder::verifyCommunication() {
    // Reading any register successfully is a good verification.
    // We read the diagnostics register as it should always return a valid value.
    auto response = readRegister(REG_CLEAR_ERROR);
    return response.has_value();
}

std::optional<Encoder::ReadFrame> Encoder::readRegister(Reg address) {
    // 1. Create the command frame for the target register.
    uint16_t read_cmd = createCommandFrame(address, true);
    // 2. Create a NOP command to clock out the response.
    uint16_t nop_cmd = createCommandFrame(REG_NOP, false);

    // 3. Send the read command (response is discarded), then send a NOP to get the actual response.
    transmitReceive(read_cmd);
    auto raw_response = transmitReceive(nop_cmd);

    if (!raw_response) {
        Log::error("Encoder", "SPI communication failed at HAL level.");
        return std::nullopt;
    }

    // 4. Parse the received frame to validate it.
    ReadFrame response = parseReadFrame(*raw_response);

    // 5. Check for sensor-level errors (bad parity or error flag set).
    if (!response.parityOK || response.fields.errorFlag) {
        Log::warning("Encoder", "Received frame with error. Parity OK: $0, Error Flag: $1", response.parityOK, uint16_t(response.fields.errorFlag));
        return std::nullopt;
    }

    return response;
}

std::optional<uint16_t> Encoder::transmitReceive(uint16_t txData) {
    uint16_t rxData = 0;

    Gpio::write(_chipSelect, false); // CS Low
    bool success = Spi::transmitReceive(_peripheral, reinterpret_cast<uint8_t*>(&txData), reinterpret_cast<uint8_t*>(&rxData), 1);
    Gpio::write(_chipSelect, true); // CS High
    Hardware::delayMs(1);           // Short delay to ensure sensor is ready

    if (!success)
        return std::nullopt;

    // Swap the bytes of the received data back to the correct order for the CPU.
    return rxData;
}

Encoder::ReadFrame Encoder::parseReadFrame(uint16_t rawFrame) {
    ReadFrame frame;
    frame.parityOK = validateReceivedParity(rawFrame);
    frame.fields.errorFlag = (rawFrame >> 14) & 0x01;
    frame.fields.data = rawFrame & 0x3FFF;
    return frame;
}

uint16_t Encoder::createCommandFrame(Reg address, bool isRead) {
    uint16_t command = address & 0x3FFF;
    if (isRead) {
        command |= (1 << 14); // Set the read/write bit (RWn)
    }
    if (calculateParityBit(command)) {
        command |= (1 << 15); // Set the parity bit (PAR)
    }
    return command;
}

bool Encoder::validateReceivedParity(uint16_t rawFrame) {
    uint8_t count = 0;
    for (int i = 0; i < 16; i++) {
        if ((rawFrame >> i) & 0x01) {
            count++;
        }
    }
    // For a valid even parity frame, the total number of set bits must be even.
    return (count % 2) == 0;
}

uint8_t Encoder::calculateParityBit(uint16_t value) {
    uint8_t count = 0;
    for (int i = 0; i < 15; i++) {
        if ((value >> i) & 0x01) {
            count++;
        }
    }
    // For even parity, the parity bit is 1 if the number of set bits is odd.
    return (count % 2);
}
