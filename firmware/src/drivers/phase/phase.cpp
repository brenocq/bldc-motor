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
    // RegConfig config{};
    // config.rst = 0;
    // config.brng = RegConfig::BRNG_16V; // Phase voltage 16V full scale
    // config.badc = RegConfig::ADC_9_BIT_84_US;
    // config.sadc = RegConfig::ADC_9_BIT_84_US;
    // config.mode = RegConfig::MODE_SHUNT_BUS_CONT;
    // config.pg = RegConfig::PG_80MV; // Shunt resistor 80mV full scale
    // writeReg(REG_CONFIG, config);
    //_regConfigValue = readReg(REG_CONFIG);
    // if (_regConfigValue != config) {
    //    Log::error("Phase", "Could not configure phase $0", _phase);
    //    return false;
    //}

    Log::success("Phase", "$0 initialized", phase);
    return true;
}

float Phase::readCurrent() {
    return 0.0f;
    // const float shuntResistance = 0.150; // 0.150 Ohms
    // float shuntVoltDrop = 0.0f;

    //// Read shunt voltage drop
    // uint16_t raw = readReg(REG_SHUNT_VOLTAGE);
    // if (raw >> 15) {
    //     // Negative sign
    //     uint8_t numSign = 4 - _regConfigValue.pg;    // Num sign bits
    //     uint16_t mask = ~(0xFFFF << (16 - numSign)); // Mask to remove sign bits
    //     raw = (~((raw & mask) - 1)) & mask;          // Two's complement
    //     shuntVoltDrop = -raw / float(100 * 1000);    // 0.01mV resolution
    // } else {
    //     // Positive sign
    //     shuntVoltDrop = raw / float(100 * 1000); // 0.01mV resolution
    // }

    //// Calculate current
    // return shuntVoltDrop / shuntResistance;
}

float Phase::readVoltage() {
    // uint16_t raw = readReg(REG_BUS_VOLTAGE);
    // return (raw >> 3) * 0.004f; // 4mV resolution
    return 0.0f;
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
    Gpio::write(_chipSelect, false);
    Spi::transmit(_peripheral, &address, sizeof(uint8_t));
    Spi::transmit(_peripheral, (uint8_t*)&data, sizeof(uint16_t));
    Gpio::write(_chipSelect, true);
}
