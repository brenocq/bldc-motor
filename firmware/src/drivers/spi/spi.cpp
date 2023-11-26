//--------------------------------------------------
// BLDC Motor Controller
// spi.cpp
// Date: 2023-11-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>
#include <system/hal.h>
#include <utils/log.h>

namespace Spi {

constexpr uint32_t timeout = 100; ///< Timeout 100ms

SPI_HandleTypeDef* getHandle(Peripheral peripheral);
SPI_TypeDef* getInstance(Peripheral peripheral);
void enableClock(Peripheral peripheral);
void disableClock(Peripheral peripheral);

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
SPI_HandleTypeDef hspi4;

} // namespace Spi

bool Spi::init() {
    // Get peripherals in use
    std::array<bool, size_t(Peripheral::NUM)> inUse;
    for (Gpio::GpioConfig conf : Gpio::gpioList) {
        if (conf.mode == Gpio::Mode::SPI1_SCK || conf.mode == Gpio::Mode::SPI1_MISO || conf.mode == Gpio::Mode::SPI1_MOSI)
            inUse[int(Peripheral::SPI1)] = true;
        else if (conf.mode == Gpio::Mode::SPI2_SCK || conf.mode == Gpio::Mode::SPI2_MISO || conf.mode == Gpio::Mode::SPI2_MOSI)
            inUse[int(Peripheral::SPI2)] = true;
        else if (conf.mode == Gpio::Mode::SPI3_SCK || conf.mode == Gpio::Mode::SPI3_MISO || conf.mode == Gpio::Mode::SPI3_MOSI)
            inUse[int(Peripheral::SPI3)] = true;
    }

    // Initialize peripherals
    for (size_t i = 0; i <= inUse.size(); i++) {
        Peripheral peripheral = Peripheral(i);
        if (!inUse[int(peripheral)])
            continue;
        enableClock(peripheral);

        SPI_HandleTypeDef* hspi = getHandle(peripheral);
        hspi->Instance = getInstance(peripheral);
        hspi->Init.Mode = SPI_MODE_MASTER;
        hspi->Init.Direction = SPI_DIRECTION_2LINES;
        hspi->Init.DataSize = SPI_DATASIZE_8BIT;
        hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
        hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi->Init.NSS = SPI_NSS_SOFT;
        hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // 5.625MHz
        hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
        hspi->Init.TIMode = SPI_TIMODE_DISABLE;
        hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        hspi->Init.CRCPolynomial = 10;
        if (HAL_SPI_Init(hspi) != HAL_OK) {
            LOG_ERROR("Spi", "Failed to initialize peripheral $0", i + 1);
            return false;
        }
    }

    LOG_SUCCESS("Spi", "Initialized");
    return true;
}

bool Spi::deinit() {
    HAL_SPI_DeInit(getHandle(Peripheral::SPI1));
    HAL_SPI_DeInit(getHandle(Peripheral::SPI2));
    HAL_SPI_DeInit(getHandle(Peripheral::SPI3));
    HAL_SPI_DeInit(getHandle(Peripheral::SPI4));
    disableClock(Peripheral::SPI1);
    disableClock(Peripheral::SPI2);
    disableClock(Peripheral::SPI3);
    disableClock(Peripheral::SPI4);
    return true;
}

bool Spi::transmit(Peripheral peripheral, uint8_t* data, uint16_t len) {
    if (HAL_SPI_Transmit(getHandle(peripheral), data, len, timeout) != HAL_OK) {
        LOG_ERROR("Spi", "Failed to transmit $0 bytes", len);
        return false;
    }
    return true;
}

bool Spi::receive(Peripheral peripheral, uint8_t* data, uint16_t len) {
    if (HAL_SPI_Receive(getHandle(peripheral), data, len, timeout) != HAL_OK) {
        LOG_ERROR("Spi", "Failed to receive $0 bytes", len);
        return false;
    }
    return true;
}

bool Spi::transmitReceive(Peripheral peripheral, uint8_t* txData, uint8_t* rxData, uint16_t len) {
    if (HAL_SPI_TransmitReceive(getHandle(peripheral), txData, rxData, len, timeout) != HAL_OK) {
        LOG_ERROR("Spi", "Failed to transmit/receive $0 bytes", len);
        return false;
    }
    return true;
}

SPI_HandleTypeDef* Spi::getHandle(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::SPI1:
            return &hspi1;
            break;
        case Peripheral::SPI2:
            return &hspi2;
            break;
        case Peripheral::SPI3:
            return &hspi3;
            break;
        case Peripheral::SPI4:
            return &hspi4;
            break;
        default:
            break;
    }
    return nullptr;
}

SPI_TypeDef* Spi::getInstance(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::SPI1:
            return (SPI_TypeDef*)SPI1_BASE;
            break;
        case Peripheral::SPI2:
            return (SPI_TypeDef*)SPI2_BASE;
            break;
        case Peripheral::SPI3:
            return (SPI_TypeDef*)SPI3_BASE;
            break;
        case Peripheral::SPI4:
            return (SPI_TypeDef*)SPI4_BASE;
            break;
        default:
            break;
    }
    return nullptr;
}

void Spi::enableClock(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::SPI1:
            __HAL_RCC_SPI1_CLK_ENABLE();
            break;
        case Peripheral::SPI2:
            __HAL_RCC_SPI2_CLK_ENABLE();
            break;
        case Peripheral::SPI3:
            __HAL_RCC_SPI3_CLK_ENABLE();
            break;
        case Peripheral::SPI4:
            __HAL_RCC_SPI4_CLK_ENABLE();
            break;
        default:
            break;
    }
}

void Spi::disableClock(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::SPI1:
            __HAL_RCC_SPI1_CLK_DISABLE();
            break;
        case Peripheral::SPI2:
            __HAL_RCC_SPI2_CLK_DISABLE();
            break;
        case Peripheral::SPI3:
            __HAL_RCC_SPI3_CLK_DISABLE();
            break;
        case Peripheral::SPI4:
            __HAL_RCC_SPI4_CLK_DISABLE();
            break;
        default:
            break;
    }
}
