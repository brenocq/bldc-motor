//--------------------------------------------------
// BLDC Motor Controller
// phase.h
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_PHASE_PHASE_H
#define BLDC_DRIVERS_PHASE_PHASE_H
#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>

// clang-format off
// Used to cast register structs to uint16_t
#define REG_CAST_16(T) \
        T() = default; \
        T(uint16_t d) { *((uint16_t*)this) = d; } \
        operator uint16_t() const { return *((uint16_t*)this); }
// clang-format on

/* @brief Phase sensor driver
 *
 * Driver for the INA239-Q1, it is possible to measure the phase voltage, current, and temperature.
 */
class Phase {
  public:
    enum PhaseId { U = 0, V, W };

    bool init(PhaseId phase, Spi::Peripheral peripheral, Gpio::Gpio chipSelect);

    float readCurrent();
    float readVoltage();
    float readTemperature();

  private:
    // clang-format off
    enum Reg : uint8_t {
        REG_CONFIG          = 0x00,
        REG_ADC_CONFIG      = 0x01,
        REG_SHUNT_CAL       = 0x02,
        REG_VSHUNT          = 0x04,
        REG_VBUS            = 0x05,
        REG_DIETEMP         = 0x06,
        REG_CURRENT         = 0x07,
        REG_POWER           = 0x08,
        REG_DIAG_ALRT       = 0x0B,
        REG_SOVL            = 0x0C,
        REG_SUVL            = 0x0D,
        REG_BOVL            = 0x0E,
        REG_BUVL            = 0x0F,
        REG_TEMP_LIMIT      = 0x10,
        REG_PWR_LIMIT       = 0x11,
        REG_MANUFACTURER_ID = 0x3E,
        REG_DEVICE_ID       = 0x3F,
    };
    // clang-format on

    uint16_t readReg(Reg reg);
    void writeReg(Reg reg, uint16_t data);

    /* @brief Register config
     * Conversion delay can be used to have all phase sensors reading at approximately the same time. Keep in mind that there may be clock drift.
     */
    struct Config {
        uint16_t reserved0 : 4; ///< Reserved bits
        uint16_t adcRange : 1;  ///< ADC range
        uint16_t reserved1 : 1; ///< Reserved bit
        uint16_t convDly : 8;   ///< Conversion delay (0x00->0s 0xFF->510ms)
        uint16_t reserved2 : 1; ///< Reserved bit
        uint16_t rst : 1;       ///< Reset bit (set to 1 to reset)

        REG_CAST_16(Config);

        enum AdcRange : uint8_t {
            ADC_RANGE_163_84_V = 0b0,
            ADC_RANGE_40_96_V = 0b1,
        };
    };

    /// Register ADC config
    struct AdcConfig {
        uint16_t avg : 3;    ///< ADC averaging count
        uint16_t vtct : 3;   ///< Temperature conversion time
        uint16_t vshct : 3;  ///< Shunt voltage conversion time
        uint16_t vbusct : 3; ///< Bus voltage conversion time
        uint16_t mode : 4;   ///< Mode

        REG_CAST_16(AdcConfig);

        enum AveragingCount : uint8_t {
            AVR_1 = 0,
            AVR_4,
            AVR_16,
            AVR_64,
            AVR_128,
            AVR_256,
            AVR_512,
            AVR_1024,
        };

        enum ConversionTime : uint8_t {
            CT_50_US = 0,
            CT_84_US,
            CT_150_US,
            CT_280_US,
            CT_540_US,
            CT_1052_US,
            CT_2074_US,
            CT_4120_US,
        };

        /* @brief ADC mode
         * When in trigger mode, the mode will go back to SHUTDOWN after one read.
         */
        enum Mode : uint8_t {
            MODE_SHUTDOWN = 0x0,
            MODE_TRIG_BUS = 0x1,
            MODE_TRIG_SHUNT = 0x2,
            MODE_TRIG_BUS_SHUNT = 0x3,
            MODE_TRIG_TEMP = 0x4,
            MODE_TRIG_BUS_TEMP = 0x5,
            MODE_TRIG_SHUNT_TEMP = 0x6,
            MODE_TRIG_BUS_SHUNT_TEMP = 0x7,
            MODE_CONT_BUS = 0x9,
            MODE_CONT_SHUNT = 0xA,
            MODE_CONT_BUS_SHUNT = 0xB,
            MODE_CONT_TEMP = 0xC,
            MODE_CONT_BUS_TEMP = 0xD,
            MODE_CONT_SHUNT_TEMP = 0xE,
            MODE_CONT_BUS_SHUNT_TEMP = 0xF,
        };
    };

    /// Register shunt calibration
    struct ShuntCal {
        uint16_t shuntCal : 15; ///< Shunt calibration
        uint16_t reserved : 1;  ///< Reserved bit

        REG_CAST_16(ShuntCal);

        uint16_t calibrate(float maxCurrent, float shuntResistance) { return 819.2e6 * (maxCurrent / 32768) * shuntResistance; }
    };

    // Register diagnostic flags and alert
    struct DiagAlrt {
        uint16_t memStat : 1;   ///< Memory status (0=error 1=normal)
        uint16_t cnvrf : 1;     ///< Conversion completed
        uint16_t pol : 1;       ///< Power threshold limit
        uint16_t busul : 1;     ///< Bus under-limit threshold
        uint16_t busol : 1;     ///< Bus over-limit threshold
        uint16_t shntul : 1;    ///< Shunt under-limit threshold
        uint16_t shntol : 1;    ///< Shunt over-limit threshold
        uint16_t tmpol : 1;     ///< Temperature over-limit threshold
        uint16_t reserved0 : 1; ///< Reserved bit
        uint16_t mathof : 1;    ///< Mathematical overflow
        uint16_t reserved1 : 2; ///< Reserved bits
        uint16_t apol : 1;      ///< Alert pin polarity
        uint16_t slowAlert : 1; ///< Alert averaged values
        uint16_t cnvr : 1;      ///< Alert pin on conversion ready
        uint16_t alatch : 1;    ///< Alert pin active until read

        REG_CAST_16(DiagAlrt);
    };

    // Register device identification
    struct DeviceId {
        uint16_t revId : 4;  ///< Device revision identification
        uint16_t devId : 12; ///< Device identification

        REG_CAST_16(DeviceId);
    };

    static constexpr uint16_t MANUFACTURER_ID_DEFAULT = 0x5449;
    static constexpr uint16_t DEVICE_ID_DEFAULT = 0x0239;
    static constexpr uint16_t REV_ID_DEFAULT = 0x001;

    PhaseId _phase;              ///< Motor phase
    Spi::Peripheral _peripheral; ///< SPI peripehral
    Gpio::Gpio _chipSelect;      ///< SPI chip select

    Config _regConfigValue;       ///< Register config value
    AdcConfig _regAdcConfigValue; ///< Register ADC config value
};

inline Phase phaseU;
inline Phase phaseV;
inline Phase phaseW;

#endif // BLDC_DRIVERS_PHASE_PHASE_H
