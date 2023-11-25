//--------------------------------------------------
// BLDC Motor Controller
// i2c.cpp
// Date: 2023-11-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/gpio/gpio.h>
#include <drivers/i2c/i2c.h>
#include <set>
#include <system/hal.h>
#include <utils/log.h>

namespace I2c {

constexpr uint32_t timeout = 100; ///< Timeout 100ms

I2C_HandleTypeDef* getHandle(Peripheral peripheral);
I2C_TypeDef* getInstance(Peripheral peripheral);
void enableClock(Peripheral peripheral);
void disableClock(Peripheral peripheral);

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

} // namespace I2c

bool I2c::init() {
    // Get peripherals in use
    std::array<bool, 3> inUse;
    for (Gpio::GpioConfig conf : Gpio::gpioList) {
        if (conf.mode == Gpio::Mode::I2C1_SDA || conf.mode == Gpio::Mode::I2C1_SCL)
            inUse[int(Peripheral::I2C1)] = true;
        else if (conf.mode == Gpio::Mode::I2C2_SDA || conf.mode == Gpio::Mode::I2C2_SCL)
            inUse[int(Peripheral::I2C2)] = true;
        else if (conf.mode == Gpio::Mode::I2C3_SDA || conf.mode == Gpio::Mode::I2C3_SCL)
            inUse[int(Peripheral::I2C3)] = true;
    }

    // Initialize peripherals
    for (size_t i = 0; i <= inUse.size(); i++) {
        Peripheral peripheral = Peripheral(i);
        if (!inUse[int(peripheral)])
            continue;
        enableClock(peripheral);

        I2C_HandleTypeDef* hi2c = getHandle(peripheral);
        hi2c->Instance = getInstance(peripheral);
        hi2c->Init.ClockSpeed = 100000;
        hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
        hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        hi2c->Init.OwnAddress1 = 0;
        hi2c->Init.OwnAddress2 = 0;
        HAL_I2C_Init(hi2c);
    }

    // Check slaves
    for (I2cConfig cfg : i2cList) {
        if (!inUse[int(cfg.peripheral)]) {
            LOG_WARN("I2c", "Slave $x1 in peripheral $0, but peripheal was not initialized", (int)cfg.peripheral + 1, (int)cfg.address);
            return false;
        }
        if (!checkReady(cfg.peripheral, cfg.address)) {
            LOG_WARN("I2c", "Slave $x1 in peripheral $0 not responding", (int)cfg.peripheral + 1, (int)cfg.address);
            return false;
        }
    }
    return true;
}

bool I2c::deinit() {
    HAL_I2C_DeInit(getHandle(Peripheral::I2C1));
    HAL_I2C_DeInit(getHandle(Peripheral::I2C2));
    HAL_I2C_DeInit(getHandle(Peripheral::I2C3));
    disableClock(Peripheral::I2C1);
    disableClock(Peripheral::I2C2);
    disableClock(Peripheral::I2C3);
    return true;
}

bool I2c::checkReady(Peripheral peripheral, Address address) {
    constexpr uint32_t numTries = 3;
    return HAL_I2C_IsDeviceReady(getHandle(peripheral), address, numTries, timeout) == HAL_OK;
}

bool I2c::transmit(Peripheral peripheral, Address address, uint8_t* data, uint16_t len) {
    if (HAL_I2C_Master_Transmit(getHandle(peripheral), address, data, len, timeout) == HAL_OK)
        return true;
    else {
        LOG_WARN("I2c", "Failed to transmit $0 bytes to slave $x1 (peripheral $2)", len, (int)address, (int)peripheral + 1);
        return false;
    }
}

bool I2c::receive(Peripheral peripheral, Address address, uint8_t* data, uint16_t len) {
    if (HAL_I2C_Master_Receive(getHandle(peripheral), address, data, len, timeout) == HAL_OK)
        return true;
    else {
        LOG_WARN("I2c", "Failed to receive $0 bytes from slave $x1 (peripheral $2)", len, (int)address, (int)peripheral + 1);
        return false;
    }
}

bool I2c::readRegister(Peripheral peripheral, Address address, uint8_t reg, uint8_t* val) {
    if (!transmit(peripheral, address, &reg, sizeof(uint8_t)))
        return false;
    return receive(peripheral, address, val, sizeof(uint8_t));
}

bool I2c::readRegister(Peripheral peripheral, Address address, uint8_t reg, uint16_t* val) {
    if (!transmit(peripheral, address, &reg, sizeof(uint16_t)))
        return false;

    // Read 2 bytes and fix endianness
    uint8_t data[2];
    if (!receive(peripheral, address, data, sizeof(uint16_t)))
        return false;
    *val = (data[0] << 8) | data[1];

    return true;
}

bool I2c::writeRegister(Peripheral peripheral, Address address, uint8_t reg, uint8_t val) {
    uint8_t data[2] = {reg, val};
    return transmit(peripheral, address, data, 2 * sizeof(uint8_t));
}

bool I2c::writeRegister(Peripheral peripheral, Address address, uint8_t reg, uint16_t val) {
    uint8_t data[3] = {reg, uint8_t(val >> 8), uint8_t(val & 0xFF)};
    LOG_DEBUG("I2c", "Write reg 16bit $x0", val);
    return transmit(peripheral, address, data, 3 * sizeof(uint8_t));
}

I2C_HandleTypeDef* I2c::getHandle(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::I2C1:
            return &hi2c1;
            break;
        case Peripheral::I2C2:
            return &hi2c2;
            break;
        case Peripheral::I2C3:
            return &hi2c3;
            break;
    }
    return nullptr;
}

I2C_TypeDef* I2c::getInstance(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::I2C1:
            return (I2C_TypeDef*)I2C1_BASE;
            break;
        case Peripheral::I2C2:
            return (I2C_TypeDef*)I2C2_BASE;
            break;
        case Peripheral::I2C3:
            return (I2C_TypeDef*)I2C3_BASE;
            break;
    }
    return nullptr;
}

void I2c::enableClock(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::I2C1:
            __HAL_RCC_I2C1_CLK_ENABLE();
            break;
        case Peripheral::I2C2:
            __HAL_RCC_I2C2_CLK_ENABLE();
            break;
        case Peripheral::I2C3:
            __HAL_RCC_I2C3_CLK_ENABLE();
            break;
    }
}

void I2c::disableClock(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::I2C1:
            __HAL_RCC_I2C1_CLK_DISABLE();
            break;
        case Peripheral::I2C2:
            __HAL_RCC_I2C2_CLK_DISABLE();
            break;
        case Peripheral::I2C3:
            __HAL_RCC_I2C3_CLK_DISABLE();
            break;
    }
}
