//--------------------------------------------------
// BLDC Motor Controller
// encoder.cpp
// Date: 2023-09-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/encoder/encoder.h>
#include <drivers/hardware.h>
#include <utils/log.h>

bool Encoder::init(Spi::Peripheral peripheral, Gpio::Gpio chipSelect) {
    _peripheral = peripheral;
    _chipSelect = chipSelect;
    Gpio::write(_chipSelect, true);

    if (readAngle() == -1.0f) {
        Log::error("Encoder", "Failed to initialize encoder, communication not possible");
        return false;
    }

    Log::success("Encoder", "Initialized");
    return true;
}

static uint8_t calculateCRC(uint32_t data) {
    // Thanks to https://github.com/scottbez1/smartknob/blob/master/firmware/src/mt6701_sensor.cpp
    static uint8_t tableCRC6[64] = {0x00, 0x03, 0x06, 0x05, 0x0C, 0x0F, 0x0A, 0x09, 0x18, 0x1B, 0x1E, 0x1D, 0x14, 0x17, 0x12, 0x11,
                                    0x30, 0x33, 0x36, 0x35, 0x3C, 0x3F, 0x3A, 0x39, 0x28, 0x2B, 0x2E, 0x2D, 0x24, 0x27, 0x22, 0x21,
                                    0x23, 0x20, 0x25, 0x26, 0x2F, 0x2C, 0x29, 0x2A, 0x3B, 0x38, 0x3D, 0x3E, 0x37, 0x34, 0x31, 0x32,
                                    0x13, 0x10, 0x15, 0x16, 0x1F, 0x1C, 0x19, 0x1A, 0x0B, 0x08, 0x0D, 0x0E, 0x07, 0x04, 0x01, 0x02};
    uint8_t idx = 0;
    uint8_t crc = 0;

    idx = (uint8_t)(((uint32_t)data >> 12u) & 0x0000003Fu);

    crc = (uint8_t)(((uint32_t)data >> 6u) & 0x0000003Fu);
    idx = crc ^ tableCRC6[idx];

    crc = (uint8_t)((uint32_t)data & 0x0000003Fu);
    idx = crc ^ tableCRC6[idx];

    crc = tableCRC6[idx];

    return crc;
}

float Encoder::readAngle() {
    uint16_t data = 0;
    uint8_t status = 0;
    uint8_t crc = 0;

    // Read angle
    uint8_t bits[3];
    Gpio::write(_chipSelect, false);
    Spi::receive(_peripheral, bits, 3 * sizeof(uint8_t));
    Gpio::write(_chipSelect, true);
    data = (bits[0] << 6) | (bits[1] >> 2);
    status = ((bits[1] & 0b11) << 2) | (bits[2] >> 6);
    crc = bits[2] & 0b111111;

    // Calculate angle
    float angle = data / 16384.0f * 3.141592f * 2.0f; // Calculate angle in radians

    // Check status
    // if (status & STATUS_MAG_FIELD_TOO_STRONG)
    //    Log::debug("Encoder", "Status: MAG_FIELD_TOO_STRONG");
    // if (status & STATUS_MAG_FIELD_TOO_WEAK)
    //    Log::debug("Encoder", "Status: MAG_FIELD_TOO_WEAK");
    // if (status & STATUS_PUSH_BUTTON_DETECTED)
    //    Log::debug("Encoder", "Status: PUSH_BUTTON_DETECTED");
    // if (status & STATUS_LOSS_OF_TRACK)
    //    Log::debug("Encoder", "Status: LOSS_OF_TRACK");

    // Check CRC
    uint8_t crcReceived = calculateCRC(uint32_t(data) << 4 | status);
    if (crcReceived == crc)
        return angle;

    // CRC error
    Log::warning("Encoder", "CRC mismatch: " + std::to_string(crcReceived) + " instead of " + std::to_string(crc));
    return -1.0f;
}
