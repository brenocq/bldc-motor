//--------------------------------------------------
// BLDC Motor Controller
// imu.cpp
// Date: 2023-11-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/imu/imu.h>
#include <utils/log.h>

bool Imu::init(Spi::Peripheral peripheral, Gpio::Gpio chipSelect) {
    _peripheral = peripheral;
    _chipSelect = chipSelect;
    Gpio::write(_chipSelect, true);

    // Check WHO_AM_I
    uint8_t whoAmI = readReg(REG_WHO_AM_I);
    if (whoAmI != WHO_AM_I_DEFAULT) {
        LOG_ERROR("Imu", "WHO_AM_I mismatch! It is $x0 but it should be $x1", int(whoAmI), int(WHO_AM_I_DEFAULT));
        return false;
    }

    LOG_SUCCESS("Imu", "Initialized");
    return true;
}

void Imu::getGyroAccel(int16_t* gx, int16_t* gy, int16_t* gz, int16_t* ax, int16_t* ay, int16_t* az) {}

void Imu::getAccel(int16_t* x, int16_t* y, int16_t* z) {}

void Imu::getGyro(int16_t* x, int16_t* y, int16_t* z) {}

float Imu::getTemperature() { return 0; }

uint8_t Imu::readReg(Reg reg) {
    uint8_t address = uint8_t(reg);
    address |= 0b10000000; // Read bit
    uint8_t data;
    Gpio::write(_chipSelect, false);
    Spi::transmit(_peripheral, &address, sizeof(uint8_t));
    Spi::receive(_peripheral, &data, sizeof(uint8_t));
    Gpio::write(_chipSelect, true);
    return data;
}

void Imu::writeReg(Reg reg, uint8_t data) {
    uint8_t address = uint8_t(reg);
    Gpio::write(_chipSelect, false);
    Spi::transmit(_peripheral, &address, sizeof(uint8_t));
    Spi::transmit(_peripheral, &data, sizeof(uint8_t));
    Gpio::write(_chipSelect, true);
}
