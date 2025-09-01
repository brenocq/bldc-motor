//--------------------------------------------------
// BLDC Motor Controller
// imu.cpp
// Date: 2023-11-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/hardware.h>
#include <drivers/imu/imu.h>
#include <limits>
#include <utils/log.h>
#include <vector>

bool Imu::init(Spi::Peripheral peripheral, Gpio::Gpio chipSelect) {
    _peripheral = peripheral;
    _chipSelect = chipSelect;

    // Initial read to initialize SPI (not sure why, but first transaction always fails)
    readReg(REG_WHO_AM_I);

    // Check WHO_AM_I
    uint8_t whoAmI = readReg(REG_WHO_AM_I);
    if (whoAmI != WHO_AM_I_DEFAULT) {
        Log::error("Imu", "WHO_AM_I mismatch! It is $x0 but it should be $x1", int(whoAmI), int(WHO_AM_I_DEFAULT));
        return false;
    }

    // Configure accelerometer
    Ctrl1XL ctrl1;
    ctrl1.odr = Ctrl1XL::ODR_416Hz;
    ctrl1.fullScale = Ctrl1XL::FS_4G;
    ctrl1.enableLPF2 = true;
    writeReg(REG_CTRL1_XL, ctrl1);

    // Configure gyroscope
    Ctrl2G ctrl2;
    ctrl2.odr = Ctrl2G::ODR_416Hz;
    ctrl2.fullScale = Ctrl2G::FS_500DPS;
    writeReg(REG_CTRL2_G, ctrl2);

    Log::success("Imu", "Initialized");
    return true;
}

void Imu::getGyrAcc(int16_t* gx, int16_t* gy, int16_t* gz, int16_t* ax, int16_t* ay, int16_t* az) {
    std::array<int16_t, 6> rawData;
    readMultiReg(REG_OUTX_L_G, (uint8_t*)rawData.data(), 12);
    *gx = rawData[0];
    *gy = rawData[1];
    *gz = rawData[2];
    *ax = rawData[3];
    *ay = rawData[4];
    *az = rawData[5];
}

std::array<int16_t, 3> Imu::getAcc() {
    std::array<int16_t, 3> accRaw;
    readMultiReg(REG_OUTX_L_A, (uint8_t*)accRaw.data(), 6);
    return accRaw;
}

std::array<int16_t, 3> Imu::getGyr() {
    std::array<int16_t, 3> gyrRaw;
    readMultiReg(REG_OUTX_L_G, (uint8_t*)gyrRaw.data(), 6);
    return gyrRaw;
}

float Imu::getTemp() {
    // Read temperature data
    int16_t tempRaw;
    readMultiReg(REG_OUT_TEMP_L, (uint8_t*)&tempRaw, 2);
    return (float)tempRaw / 256.0f + 25.0f;
}

uint8_t Imu::readReg(Reg reg) {
    uint8_t txBuffer[2] = {uint8_t(uint8_t(reg) | 0x80), 0x00}; // Read bit + dummy byte
    uint8_t rxBuffer[2] = {0, 0};

    Gpio::write(_chipSelect, false);
    Spi::transmitReceive(_peripheral, txBuffer, rxBuffer, 2);
    Gpio::write(_chipSelect, true);

    return rxBuffer[1];
}

void Imu::writeReg(Reg reg, uint8_t data) {
    uint8_t txBuffer[2] = {uint8_t(reg), data};

    Gpio::write(_chipSelect, false);
    Spi::transmit(_peripheral, txBuffer, 2);
    Gpio::write(_chipSelect, true);
}

bool Imu::readMultiReg(Reg startReg, uint8_t* data, uint16_t len) {
    uint8_t address = uint8_t(startReg) | 0x80; // Set read bit

    // Create a transmit buffer with the address and dummy bytes for the read
    std::vector<uint8_t> txBuffer(len + 1, 0x00);
    txBuffer[0] = address;

    // Create a receive buffer
    std::vector<uint8_t> rxBuffer(len + 1, 0x00);

    Gpio::write(_chipSelect, false);
    bool result = Spi::transmitReceive(_peripheral, txBuffer.data(), rxBuffer.data(), len + 1);
    Gpio::write(_chipSelect, true);

    // Copy the received data (skipping the first dummy byte)
    if (result)
        std::copy(rxBuffer.begin() + 1, rxBuffer.end(), data);
    return result;
}
