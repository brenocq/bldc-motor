//--------------------------------------------------
// BLDC Motor Controller
// phase.h
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_PHASE_PHASE_H
#define BLDC_DRIVERS_PHASE_PHASE_H
#include <drivers/i2c/i2c.h>

class Phase {
  public:
    enum PhaseId { U = 0, V, W };
    bool init(PhaseId phase, I2c::Peripheral peripheral, I2c::Address address);

    float readCurrent();
    float readVoltage();

  private:
    /// Registers
    enum Reg : uint8_t {
        REG_CONFIG = 0x00,
        REG_SHUNT_VOLTAGE = 0x01,
        REG_BUS_VOLTAGE = 0x02,
        REG_POWER = 0x03,
        REG_CURRENT = 0x04,
        REG_CALIBRATION = 0x05,
    };

    uint16_t readReg(Reg reg);
    void writeReg(Reg reg, uint16_t data);

    /// Register config
    struct RegConfig {
        uint8_t mode : 3;     ///< Operating mode
        uint8_t sadc : 4;     ///< Shunt ADC resolution/averaging
        uint8_t badc : 4;     ///< Bus ADC resolution/averaging
        uint8_t pg : 2;       ///< PGA gain and range
        uint8_t brng : 1;     ///< Bus voltage range (0->16V 1->32V)
        uint8_t reserved : 1; ///< Reserved bit
        uint8_t rst : 1;      ///< Reset bit (set to 1 to reset)

        RegConfig() = default;
        RegConfig(uint16_t d) { *((uint16_t*)this) = d; }
        operator uint16_t() const { return *((uint16_t*)this); }

        enum Brng : uint8_t {
            BRNG_16V = 0b0,
            BRNG_32V = 0b1,
        };

        enum Pg : uint8_t {
            PG_40MV = 0b00,
            PG_80MV = 0b01,
            PG_160MV = 0b10,
            PG_320MV = 0b11,
        };

        /// Values for the badc and sadc bits
        enum Adc : uint8_t {
            ADC_9_BIT_84_US = 0b0000,       // 9-bit | 0.084ms
            ADC_10_BIT_148_US = 0b0001,     // 10-bit | 0.148ms
            ADC_11_BIT_276_US = 0b0010,     // 11-bit | 0.276ms
            ADC_12_BIT_532_US = 0b0011,     // 12-bit | 0.532ms
            ADC_2_SAMPLES_1_MS = 0b1001,    // 12-bit | 2 samples | 1.06ms
            ADC_4_SAMPLES_2_MS = 0b1010,    // 12-bit | 4 samples | 2.13ms
            ADC_8_SAMPLES_4_MS = 0b1011,    // 12-bit | 8 samples | 4.26ms
            ADC_16_SAMPLES_8_MS = 0b1100,   // 12-bit | 16 samples | 8.51ms
            ADC_32_SAMPLES_17_MS = 0b1101,  // 12-bit | 32 samples | 17.02ms
            ADC_64_SAMPLES_34_MS = 0b1110,  // 12-bit | 64 samples | 34.05ms
            ADC_128_SAMPLES_68_MS = 0b1111, // 12-bit | 128 samples | 68.10ms
        };

        enum Mode : uint8_t {
            MODE_POWER_DOWN = 0b000,
            MODE_SHUNT_TRIG = 0b001,
            MODE_BUS_TRIG = 0b010,
            MODE_SHUNT_BUS_TRIG = 0b011,
            MODE_ADC_OFF = 0b100,
            MODE_SHUNT_CONT = 0b101,
            MODE_BUS_CONT = 0b110,
            MODE_SHUNT_BUS_CONT = 0b111,
        };
    };

    PhaseId _phase;              ///< Motor phase
    I2c::Peripheral _peripheral; ///< I2C peripehral
    I2c::Address _address;       ///< I2C address

    RegConfig _regConfigValue; ///< Register config value
};

inline Phase phaseU;
inline Phase phaseV;
inline Phase phaseW;

#endif // BLDC_DRIVERS_PHASE_PHASE_H
