//--------------------------------------------------
// BLDC Motor Controller
// phase.cpp
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/hardware.h>
#include <drivers/phase/phase.h>
#include <utils/log.h>

// Phase::PhaseId overload
template <typename Tstream>
std::ostream& operator<<(Tstream& s, const Phase::PhaseId& p) {
    s << char('U' + int(p));
    return s;
}

bool Phase::init(PhaseId phase, I2c::Peripheral peripheral, I2c::Address address) {
    _phase = phase;
    _peripheral = peripheral;
    _address = address;
    if (!I2c::checkReady(_peripheral, _address)) {
        Log::error("Phase", "Could not find phase $0 in I2C", _phase);
        return false;
    }

    //---------- Configuration ----------//
    RegConfig config{};
    config.rst = 0;
    config.brng = RegConfig::BRNG_16V; // Phase voltage 16V full scale
    config.badc = RegConfig::ADC_9_BIT_84_US;
    config.sadc = RegConfig::ADC_9_BIT_84_US;
    config.mode = RegConfig::MODE_SHUNT_BUS_CONT;
    config.pg = RegConfig::PG_80MV; // Shunt resistor 80mV full scale
    writeReg(REG_CONFIG, config);
    _regConfigValue = readReg(REG_CONFIG);
    if (_regConfigValue != config) {
        Log::error("Phase", "Could not configure phase $0", _phase);
        return false;
    }

    Log::success("Phase", "$0 initialized", phase);
    return true;
}

float Phase::readCurrent() {
    const float shuntResistance = 0.120; // 0.120 Ohms
    float shuntVoltDrop = 0.0f;

    // Read shunt voltage drop
    uint16_t raw = readReg(REG_SHUNT_VOLTAGE);
    if (raw >> 15) {
        // Negative sign
        uint8_t numSign = 4 - _regConfigValue.pg;    // Num sign bits
        uint16_t mask = ~(0xFFFF << (16 - numSign)); // Mask to remove sign bits
        raw = (~((raw & mask) - 1)) & mask;          // Two's complement
        shuntVoltDrop = -raw / float(100 * 1000);    // 0.01mV resolution
    } else {
        // Positive sign
        shuntVoltDrop = raw / float(100 * 1000); // 0.01mV resolution
    }

    // Calculate current
    return shuntVoltDrop / shuntResistance;
}

float Phase::readVoltage() {
    uint16_t raw = readReg(REG_BUS_VOLTAGE);
    return (raw >> 3) * 0.004f; // 4mV resolution
}

uint16_t Phase::readReg(Reg reg) {
    I2c::transmit(_peripheral, _address, (uint8_t*)&reg, sizeof(uint8_t));
    uint8_t rx[2];
    I2c::receive(_peripheral, _address, rx, sizeof(rx));
    return uint16_t(rx[0] << 8) | rx[1];
}

void Phase::writeReg(Reg reg, uint16_t data) {
    uint8_t tx[3] = {uint8_t(reg), uint8_t(data >> 8), uint8_t(data & 0xFF)};
    I2c::transmit(_peripheral, _address, tx, sizeof(tx));
}
