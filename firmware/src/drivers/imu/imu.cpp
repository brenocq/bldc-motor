//--------------------------------------------------
// BLDC Motor Controller
// imu.cpp
// Date: 2023-11-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/imu/imu.h>
#include <limits>
#include <utils/log.h>

bool Imu::init(Spi::Peripheral peripheral, Gpio::Gpio chipSelect) {
    _peripheral = peripheral;
    _chipSelect = chipSelect;
    Gpio::write(_chipSelect, true);

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

void Imu::getGyrAcc(int16_t* gx, int16_t* gy, int16_t* gz, int16_t* ax, int16_t* ay, int16_t* az) {}

std::array<int16_t, 3> Imu::getAcc() {
    // Read raw
    uint8_t address = uint8_t(REG_OUTX_L_A) | 0b10000000; // Read bit
    std::array<int16_t, 3> accRaw;
    Gpio::write(_chipSelect, false);
    Spi::transmit(_peripheral, &address, sizeof(uint8_t));
    Spi::receive(_peripheral, (uint8_t*)accRaw.data(), 3 * sizeof(int16_t));
    Gpio::write(_chipSelect, true);

    // Convert to m/s^2
    // std::array<float, 3> acc;
    // const float k = (9.81f * 4.0f) / std::numeric_limits<int16_t>::max();
    // acc[0] = k * accRaw[0];
    // acc[1] = k * accRaw[1];
    // acc[2] = k * accRaw[2];
    // Log::debug("Imu", "Acc $0", acc);

    return accRaw;
}

std::array<int16_t, 3> Imu::getGyr() {
    // Read raw
    uint8_t address = uint8_t(REG_OUTX_L_G) | 0b10000000; // Read bit
    std::array<int16_t, 3> gyrRaw;
    Gpio::write(_chipSelect, false);
    Spi::transmit(_peripheral, &address, sizeof(uint8_t));
    Spi::receive(_peripheral, (uint8_t*)gyrRaw.data(), 3 * sizeof(int16_t));
    Gpio::write(_chipSelect, true);

    // Convert to m/s^2
    // std::array<float, 3> gyr;
    // const float k = 500.0f / std::numeric_limits<int16_t>::max();
    // gyr[0] = k * gyrRaw[0];
    // gyr[1] = k * gyrRaw[1];
    // gyr[2] = k * gyrRaw[2];
    // Log::debug("Imu", "Gyr $0", gyr);

    return gyrRaw;
}

float Imu::getTemperature() { return 0; }

uint8_t Imu::readReg(Reg reg) {
    uint8_t address = uint8_t(reg) | 0b10000000; // Read bit
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
