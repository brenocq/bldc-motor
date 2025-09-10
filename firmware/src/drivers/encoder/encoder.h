//--------------------------------------------------
// BLDC Motor Controller
// encoder.h
// Date: 2025-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_ENCODER_ENCODER_H
#define BLDC_DRIVERS_ENCODER_ENCODER_H
#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>
#include <optional>

/**
 * @brief Driver for the AS5048A magnetic encoder
 *
 * Datasheet: https://nl.mouser.com/datasheet/2/588/asset_pdf_25493108-3418798.pdf
 */
class Encoder {
  public:
    bool init(Spi::Peripheral peripheral, Gpio::Gpio chipSelect);

    std::optional<float> readAngle();
    std::optional<uint16_t> readMagnitude();
    bool verifyCommunication();

  private:
    // clang-format off
    enum Reg : uint16_t {
        REG_NOP         = 0x0000, // No operation
        REG_CLEAR_ERROR = 0x0001, // Clear Error Flag register
        REG_PROG_CTRL   = 0x0003, // Programming Control register
        REG_OTP_ZERO_HI = 0x0016, // OTP Zero Position High bits
        REG_OTP_ZERO_LO = 0x0017, // OTP Zero Position Low bits
        REG_DIAG_AGC    = 0x3FFD, // Diagnostics and AGC register
        REG_MAGNITUDE   = 0x3FFE, // Magnitude register
        REG_ANGLE       = 0x3FFF, // Angle register
    };
    // clang-format on

    /**
     * @brief Structure to hold the parsed data from a sensor read frame.
     * Uses a union to provide access to the raw 16-bit value and
     * its individual fields via bit-fields.
     */
    struct ReadFrame {
        union {
            uint16_t raw;
            struct {
                // NOTE: Bit-field order is compiler-dependent. This assumes little-endian packing.
                uint16_t data : 14;     // 14-bit data payload
                uint16_t errorFlag : 1; // State of the Error Flag (EF) bit
                uint16_t parityBit : 1; // The parity bit from the transmission
            } fields;
        };
        bool parityOK; // Result of validation
    };

    // Private helper functions for the driver logic
    uint16_t createCommandFrame(Reg address, bool isRead);
    ReadFrame parseReadFrame(uint16_t rawFrame);
    std::optional<uint16_t> transmitReceive(uint16_t txData);
    std::optional<ReadFrame> readRegister(Reg address);

    static uint8_t calculateParityBit(uint16_t value);
    static bool validateReceivedParity(uint16_t rawFrame);

    Spi::Peripheral _peripheral; ///< SPI peripheral
    Gpio::Gpio _chipSelect;      ///< SPI chip select
};

inline Encoder encoder;

#endif // BLDC_DRIVERS_ENCODER_ENCODER_H
