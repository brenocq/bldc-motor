//--------------------------------------------------
// BLDC Motor Controller
// uart.cpp
// Date: 2023-09-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/gpio/gpio.h>
#include <drivers/uart/uart.h>
#include <set>
#include <utils/log.h>

namespace Uart {

UART_HandleTypeDef* getHandle(Peripheral peripheral);
USART_TypeDef* getInstance(Peripheral peripheral);
void enableClock(Peripheral peripheral);
void disableClock(Peripheral peripheral);

UART_HandleTypeDef _huart1;
UART_HandleTypeDef _huart2;
UART_HandleTypeDef _huart3;
UART_HandleTypeDef _huart4;
UART_HandleTypeDef _huart5;
UART_HandleTypeDef _huart6;

bool _initialized;
Peripheral _default; ///< Peripheral to use if none is specified

} // namespace Uart

bool Uart::init() {
    _initialized = false;

    // Get peripherals in use
    std::set<Peripheral> inUse;
    for (Gpio::GpioConfig conf : Gpio::gpioList) {
        if (conf.mode == Gpio::Mode::USART1_TX || conf.mode == Gpio::Mode::USART1_RX)
            inUse.insert(Peripheral::UART1);
        else if (conf.mode == Gpio::Mode::USART2_TX || conf.mode == Gpio::Mode::USART2_RX)
            inUse.insert(Peripheral::UART2);
        else if (conf.mode == Gpio::Mode::USART3_TX || conf.mode == Gpio::Mode::USART3_RX)
            inUse.insert(Peripheral::UART3);
        else if (conf.mode == Gpio::Mode::UART4_TX || conf.mode == Gpio::Mode::UART4_RX)
            inUse.insert(Peripheral::UART4);
        else if (conf.mode == Gpio::Mode::UART5_TX || conf.mode == Gpio::Mode::UART5_RX)
            inUse.insert(Peripheral::UART5);
        else if (conf.mode == Gpio::Mode::USART6_TX || conf.mode == Gpio::Mode::USART6_RX)
            inUse.insert(Peripheral::UART6);
    }

    // Initialize peripherals
    for (Peripheral peripheral : inUse) {
        enableClock(peripheral);

        UART_HandleTypeDef* huart = getHandle(peripheral);
        huart->Instance = getInstance(peripheral);
        huart->Init.BaudRate = Uart::baudrate;
        huart->Init.WordLength = UART_WORDLENGTH_8B;
        huart->Init.StopBits = UART_STOPBITS_1;
        huart->Init.Parity = UART_PARITY_NONE;
        huart->Init.Mode = UART_MODE_TX_RX;
        huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart->Init.OverSampling = UART_OVERSAMPLING_16;
        HAL_UART_Init(huart);
    }

    // Set default peripheral
    if (!inUse.empty())
        _default = *inUse.begin();

    Log::success("Uart", "Initialized");
    return _initialized = true;
}

bool Uart::isInitialized() { return _initialized; }

void Uart::transmit(uint8_t* data, uint32_t size, Peripheral peripheral) { HAL_UART_Transmit(getHandle(peripheral), data, size, 100); }

uint32_t Uart::receive(uint8_t* data, uint32_t size, Peripheral peripheral) {
    if (HAL_UART_Receive(getHandle(peripheral), data, size, 100) == HAL_OK)
        return size;
    else
        return 0;
}

UART_HandleTypeDef* Uart::getHandle(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::DEFAULT:
            return getHandle(_default);
            break;
        case Peripheral::UART1:
            return &_huart1;
            break;
        case Peripheral::UART2:
            return &_huart2;
            break;
        case Peripheral::UART3:
            return &_huart3;
            break;
        case Peripheral::UART4:
            return &_huart4;
            break;
        case Peripheral::UART5:
            return &_huart5;
            break;
        case Peripheral::UART6:
            return &_huart6;
            break;
    }
    return nullptr;
}

USART_TypeDef* Uart::getInstance(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::DEFAULT:
            return getInstance(_default);
        case Peripheral::UART1:
            return (USART_TypeDef*)USART1_BASE;
            break;
        case Peripheral::UART2:
            return (USART_TypeDef*)USART2_BASE;
            break;
        case Peripheral::UART3:
            return (USART_TypeDef*)USART3_BASE;
            break;
        case Peripheral::UART4:
            return (USART_TypeDef*)UART4_BASE;
            break;
        case Peripheral::UART5:
            return (USART_TypeDef*)UART5_BASE;
            break;
        case Peripheral::UART6:
            return (USART_TypeDef*)USART6_BASE;
            break;
    }
    return nullptr;
}

void Uart::enableClock(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::DEFAULT:
            disableClock(_default);
            break;
        case Peripheral::UART1:
            __HAL_RCC_USART1_CLK_ENABLE();
            break;
        case Peripheral::UART2:
            __HAL_RCC_USART2_CLK_ENABLE();
            break;
        case Peripheral::UART3:
            __HAL_RCC_USART3_CLK_ENABLE();
            break;
        case Peripheral::UART4:
            __HAL_RCC_UART4_CLK_ENABLE();
            break;
        case Peripheral::UART5:
            __HAL_RCC_UART5_CLK_ENABLE();
            break;
        case Peripheral::UART6:
            __HAL_RCC_USART6_CLK_ENABLE();
            break;
    }
}

void Uart::disableClock(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::DEFAULT:
            disableClock(_default);
            break;
        case Peripheral::UART1:
            __HAL_RCC_USART1_CLK_DISABLE();
            break;
        case Peripheral::UART2:
            __HAL_RCC_USART2_CLK_DISABLE();
            break;
        case Peripheral::UART3:
            __HAL_RCC_USART3_CLK_DISABLE();
            break;
        case Peripheral::UART4:
            __HAL_RCC_UART4_CLK_DISABLE();
            break;
        case Peripheral::UART5:
            __HAL_RCC_UART5_CLK_DISABLE();
            break;
        case Peripheral::UART6:
            __HAL_RCC_USART6_CLK_DISABLE();
            break;
    }
}
