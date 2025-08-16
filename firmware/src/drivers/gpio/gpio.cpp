//--------------------------------------------------
// BLDC Motor Controller
// gpio.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/gpio/gpio.h>
#include <system/hal.h>
#include <utils/log.h>

namespace Gpio {

void initInput(Gpio gpio);
void initOutput(Gpio gpio);
void initAnalog(Gpio gpio);
void initI2c(Gpio gpio);
void initSpi(Gpio gpio, Mode mode);
void initUart(Gpio gpio, Mode mode);
void initTimer(Gpio gpio, Mode mode);
void initTrace(Gpio gpio, Mode mode);
void initUsb(Gpio gpio, Mode mode);

/**
 * @brief Check if GPIO configuration is valid
 *
 * It will check the GPIO mode with alternate function table to check if the selected mode is available for that GPIO.
 *
 * @param gpioConfig GPIO configuration to check
 *
 * @return True if GPIO configuration is valid
 */
bool validGpioConfig(GpioConfig gpioConfig);

/**
 * @brief Get alternate function number given the gpio and mode
 *
 * The alternate function list is checked to get the number from the table
 *
 * @warning It will return NUM_AFS if could not find
 *
 * @param gpio Gpio port
 * @param mode Alternate function mode
 *
 * @return Column number in the alternate function table
 */
static uint8_t getAlternateFunc(Gpio gpio, Mode mode);

static constexpr GPIO_TypeDef* convert(Port port);
static constexpr uint16_t convert(Pin pin);

} // namespace Gpio

bool Gpio::init() {
    // Check for conflicts
    bool conflict = false;
    for (uint32_t i = 0; i < gpioList.size(); i++)
        for (uint32_t j = i + 1; j < gpioList.size(); j++)
            if (gpioList[i].gpio == gpioList[j].gpio)
                conflict = true;
    if (conflict)
        return false;

    // Enable GPIOs clock
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    // Initialize GPIOs
    for (GpioConfig gpioConfig : gpioList) {
        // Check if mode is available for that GPIO
        if (!validGpioConfig(gpioConfig))
            return false;

        // Initialize GPIO
        if (gpioConfig.mode == Mode::INPUT)
            initInput(gpioConfig.gpio);
        else if (gpioConfig.mode == Mode::OUTPUT)
            initOutput(gpioConfig.gpio);
        else if (gpioConfig.mode == Mode::ANALOG)
            initAnalog(gpioConfig.gpio);
        else if (gpioConfig.mode >= Mode::TIM1_CH1 && gpioConfig.mode <= Mode::TIM14_CH1)
            initTimer(gpioConfig.gpio, gpioConfig.mode);
        else if (gpioConfig.mode >= Mode::I2C1_SDA && gpioConfig.mode <= Mode::FMPI2C1_SMBA)
            initI2c(gpioConfig.gpio);
        else if (gpioConfig.mode >= Mode::SPI1_NSS && gpioConfig.mode <= Mode::SPI4_MOSI)
            initSpi(gpioConfig.gpio, gpioConfig.mode);
        else if (gpioConfig.mode >= Mode::USART1_TX && gpioConfig.mode <= Mode::USART6_CK)
            initUart(gpioConfig.gpio, gpioConfig.mode);
        else if (gpioConfig.mode >= OTG_FS_DM && gpioConfig.mode <= OTG_HS_ULPI_NXT)
            initUsb(gpioConfig.gpio, gpioConfig.mode);
        else if (gpioConfig.mode >= Mode::SWO && gpioConfig.mode <= Mode::TRACE_D3)
            initTrace(gpioConfig.gpio, gpioConfig.mode);
        else
            return false;
    }

    Log::success("Gpio", "Initialized");
    return true;
}

bool Gpio::deinit() { return true; }

void Gpio::write(Gpio gpio, bool high) { HAL_GPIO_WritePin(convert(gpio.port), convert(gpio.pin), high ? GPIO_PIN_SET : GPIO_PIN_RESET); }

bool Gpio::read(Gpio gpio) { return HAL_GPIO_ReadPin(convert(gpio.port), convert(gpio.pin)) == GPIO_PIN_SET; }

void Gpio::initInput(Gpio gpio) {
    GPIO_InitTypeDef gpioInit{};
    gpioInit.Pin = convert(gpio.pin);
    gpioInit.Mode = GPIO_MODE_INPUT;
    gpioInit.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(convert(gpio.port), &gpioInit);
}

void Gpio::initOutput(Gpio gpio) {
    GPIO_InitTypeDef gpioInit{};
    gpioInit.Pin = convert(gpio.pin);
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(convert(gpio.port), &gpioInit);
}

void Gpio::initAnalog(Gpio gpio) {
    GPIO_InitTypeDef gpioInit{};
    gpioInit.Pin = convert(gpio.pin);
    gpioInit.Mode = GPIO_MODE_ANALOG;
    gpioInit.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(convert(gpio.port), &gpioInit);
}

void Gpio::initI2c(Gpio gpio) {
    constexpr uint8_t alternateFuncI2C = 0x04;
    GPIO_InitTypeDef gpioInit{};
    gpioInit.Pin = convert(gpio.pin);
    gpioInit.Mode = GPIO_MODE_AF_OD;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpioInit.Alternate = alternateFuncI2C;
    HAL_GPIO_Init(convert(gpio.port), &gpioInit);
}

void Gpio::initSpi(Gpio gpio, Mode mode) {
    GPIO_InitTypeDef gpioInit{};
    gpioInit.Pin = convert(gpio.pin);
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpioInit.Alternate = getAlternateFunc(gpio, mode);
    HAL_GPIO_Init(convert(gpio.port), &gpioInit);
}

void Gpio::initUart(Gpio gpio, Mode mode) {
    GPIO_InitTypeDef gpioInit{};
    gpioInit.Pin = convert(gpio.pin);
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpioInit.Alternate = getAlternateFunc(gpio, mode);
    HAL_GPIO_Init(convert(gpio.port), &gpioInit);
}

void Gpio::initTimer(Gpio gpio, Mode mode) {
    GPIO_InitTypeDef gpioInit{};
    gpioInit.Pin = convert(gpio.pin);
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_LOW;
    gpioInit.Alternate = getAlternateFunc(gpio, mode);
    HAL_GPIO_Init(convert(gpio.port), &gpioInit);
}

void Gpio::initTrace(Gpio gpio, Mode mode) {
    GPIO_InitTypeDef gpioInit{};
    gpioInit.Pin = convert(gpio.pin);
    gpioInit.Mode = GPIO_MODE_AF_PP;
    switch (mode) {
        case Mode::SWO:
            gpioInit.Pull = GPIO_NOPULL;
            break;
        case Mode::SWDIO:
            gpioInit.Pull = GPIO_PULLUP;
            break;
        case Mode::SWCLK:
            gpioInit.Pull = GPIO_PULLDOWN;
            break;
        default:
            gpioInit.Pull = GPIO_PULLUP;
    }
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioInit.Alternate = getAlternateFunc(gpio, mode);
    HAL_GPIO_Init(convert(gpio.port), &gpioInit);
}

void Gpio::initUsb(Gpio gpio, Mode mode) {
    GPIO_InitTypeDef gpioInit{};
    gpioInit.Pin = convert(gpio.pin);
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioInit.Alternate = getAlternateFunc(gpio, mode);
    HAL_GPIO_Init(convert(gpio.port), &gpioInit);
}

bool Gpio::validGpioConfig(GpioConfig gpioConfig) {
    Gpio gpio = gpioConfig.gpio;
    Mode mode = gpioConfig.mode;

    // Return true if mode is always valid
    if (mode == Mode::NA || mode == Mode::INPUT || mode == Mode::OUTPUT || mode == Mode::ANALOG)
        return true;

    // Check alternate function list
    std::array<Mode, NUM_AFS> afs = afList[gpio.getIdx()];
    for (size_t i = 0; i < afs.size(); i++)
        if (afs[i] == mode)
            return true;
    return false;
}

uint8_t Gpio::getAlternateFunc(Gpio gpio, Mode mode) {
    std::array<Mode, NUM_AFS> afs = afList[gpio.getIdx()];
    for (size_t i = 0; i < NUM_AFS; i++)
        if (afs[i] == mode)
            return i;
    return NUM_AFS;
}

constexpr GPIO_TypeDef* Gpio::convert(Port port) {
    switch (port) {
        case Port::A:
            return GPIOA;
        case Port::B:
            return GPIOB;
        case Port::C:
            return GPIOC;
        case Port::D:
            return GPIOD;
        case Port::E:
            return GPIOE;
        case Port::F:
            return GPIOF;
        default:
            return nullptr;
    }
}

constexpr uint16_t Gpio::convert(Pin pin) { return 1 << (uint8_t)pin; }

