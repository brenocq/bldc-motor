//--------------------------------------------------
// BLDC Motor Controller
// phase.cpp
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <cstring>
#include <drivers/hardware.h>
#include <drivers/phase/phase.h>
#include <utils/log.h>

// Phase::PhaseId overload
template <typename Tstream>
std::ostream& operator<<(Tstream& s, const Phase::PhaseId& p) {
    s << char('U' + int(p));
    return s;
}

bool Phase::init(PhaseId phase, Spi::Peripheral peripheral, Gpio::Gpio chipSelect) {
    _phase = phase;
    _peripheral = peripheral;
    _chipSelect = chipSelect;

    // XXX Quick fix to make sure SPI clock is low
    uint8_t fix;
    Spi::receive(_peripheral, &fix, sizeof(uint8_t));

    // Check DEV_ID (device identification)
    uint16_t manfId = readReg(REG_MANUFACTURER_ID);
    DeviceId devIdReg = readReg(REG_DEVICE_ID);
    if (manfId != MANUFACTURER_ID_DEFAULT) {
        Log::error("Phase", "MANUFACTURER_ID mismatch! It is $x0 but it should be $x1", int(manfId), int(MANUFACTURER_ID_DEFAULT));
        return false;
    }
    if (devIdReg.devId != DEVICE_ID_DEFAULT) {
        Log::error("Phase", "DEVICE_ID mismatch! It is $x0 but it should be $x1", int(devIdReg.devId), int(DEVICE_ID_DEFAULT));
        return false;
    }

    //---------- Configuration ----------//
    // Reset
    Config config{};
    config.rst = 1;
    writeReg(REG_CONFIG, config);

    // General configuration
    config.adcRange = Config::ADC_RANGE_163_84_V;
    config.convDly = 0;
    config.rst = 0;
    writeReg(REG_CONFIG, config);
    _regConfigValue = readReg(REG_CONFIG);
    if (_regConfigValue != config) {
        Log::error("Phase", "Could not configure CONFIG register for phase $0 ($1 instead of $2)", _phase, _regConfigValue, config);
        return false;
    }

    // ADC configuration
    AdcConfig adcConfig{};
    adcConfig.avg = AdcConfig::AVR_1;
    adcConfig.vtct = AdcConfig::CT_1052_US;
    adcConfig.vshct = AdcConfig::CT_1052_US;
    adcConfig.vbusct = AdcConfig::CT_1052_US;
    adcConfig.mode = AdcConfig::MODE_CONT_BUS_SHUNT_TEMP;

    writeReg(REG_ADC_CONFIG, adcConfig);
    _regAdcConfigValue = readReg(REG_ADC_CONFIG);
    if (_regAdcConfigValue != adcConfig) {
        Log::error("Phase", "Could not configure ADC_CONFIG register for phase $0 ($1 instead of $2)", _phase, _regAdcConfigValue, adcConfig);
        return false;
    }

    Log::success("Phase", "$0 initialized", phase);
    return true;
}

float Phase::readCurrent() {
    constexpr float shuntResistance = 0.150f; // 0.150 Ohms

    // Read shunt voltage drop
    uint16_t raw = readReg(REG_VSHUNT);
    int16_t raw2Comp;
    std::memcpy(&raw2Comp, &raw, sizeof(int16_t));
    float shuntVoltDrop = raw2Comp * (_regConfigValue.adcRange == Config::ADC_RANGE_163_84_V ? 5e-6f : 1.25e-6f);

    // Convert to current
    float current = shuntVoltDrop / shuntResistance;
    return current;
}

float Phase::readVoltage() {
    // Read voltage
    uint16_t raw = readReg(REG_VBUS);
    int16_t raw2Comp;
    std::memcpy(&raw2Comp, &raw, sizeof(int16_t));
    // Convert to volts
    float busVolt = raw2Comp * 3.125e-3f;
    return busVolt;
}

float Phase::readTemperature() {
    // Read temperature
    uint16_t raw = readReg(REG_DIETEMP);
    raw = (raw & 0x8000) | (raw >> 4); // Fix 12-bit instead 16-bit
    int16_t raw2Comp;
    std::memcpy(&raw2Comp, &raw, sizeof(int16_t));
    // Convert to celcius
    float temp = raw2Comp * 0.125f;
    return temp;
}

uint16_t Phase::readReg(Reg reg) {
    uint8_t address = uint8_t(reg) << 2 | 0b00000001;
    uint8_t tx[3] = {address, 0x00, 0x00};
    uint8_t rx[3];
    Gpio::write(_chipSelect, false);
    Spi::transmitReceive(_peripheral, tx, rx, 3);
    Gpio::write(_chipSelect, true);
    return (rx[1] << 8) | rx[2];
}

void Phase::writeReg(Reg reg, uint16_t data) {
    uint8_t address = uint8_t(reg) << 2;
    uint8_t tx[3] = {address, uint8_t(data >> 8), uint8_t(data & 0xFF)};
    Gpio::write(_chipSelect, false);
    Spi::transmit(_peripheral, tx, 3);
    Gpio::write(_chipSelect, true);
}
