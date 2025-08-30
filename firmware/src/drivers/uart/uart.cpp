//--------------------------------------------------
// BLDC Motor Controller
// uart.cpp
// Date: 2023-09-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <cstring>
#include <drivers/gpio/gpio.h>
#include <drivers/uart/uart.h>
#include <set>
#include <utils/log.h>

namespace Uart {

USART_TypeDef* getInstance(Peripheral peripheral);
void enableClock(Peripheral peripheral);
void disableClock(Peripheral peripheral);

void transmitCompleteCallback(UART_HandleTypeDef* huart);

UART_HandleTypeDef _huart1;
UART_HandleTypeDef _huart2;
UART_HandleTypeDef _huart3;
UART_HandleTypeDef _huart4;
UART_HandleTypeDef _huart5;
UART_HandleTypeDef _huart6;

bool _initialized;
Peripheral _default; ///< Peripheral to use if none is specified

// Handle DMA transactions
bool _txDmaBusy;
bool _rxDmaBusy;
bool _rxDmaLinked; ///< True if RX DMA is linked
bool _txDmaLinked; ///< True if TX DMA is linked

uint8_t _txBuffer[TX_BUFFER_SIZE];
uint32_t _txBufferWriteIdx;
uint32_t _txBufferReadIdx;

uint8_t _rxBuffer[TX_BUFFER_SIZE];
uint32_t _rxBufferReadIdx;

} // namespace Uart

bool Uart::init() {
    _initialized = false;
    _txDmaLinked = false;
    _rxDmaLinked = false;

    _txDmaBusy = false;
    _rxDmaBusy = false;

    // Initialize circular buffer indices
    _txBufferWriteIdx = 0;
    _txBufferReadIdx = 0;
    _rxBufferReadIdx = 0;

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
        if (HAL_UART_Init(huart) != HAL_OK) {
            Log::error("Uart", "Failed to initialize UART$0", int(peripheral));
            return false;
        }

        // Register TX callback
        HAL_UART_RegisterCallback(huart, HAL_UART_TX_COMPLETE_CB_ID, transmitCompleteCallback);
    }

    // Set default peripheral
    if (!inUse.empty())
        _default = *inUse.begin();

    Log::success("Uart", "Initialized");
    return _initialized = true;
}

bool Uart::isInitialized() { return _initialized; }

void Uart::update() {
    if (!_initialized)
        return;
    // Transmit pending transactions
    if (!_txDmaBusy) {
        // Calculate transmission size
        uint32_t transmitSize = 0;
        if (_txBufferWriteIdx >= _txBufferReadIdx)
            transmitSize = _txBufferWriteIdx - _txBufferReadIdx;
        else
            transmitSize = RX_BUFFER_SIZE - _txBufferReadIdx;

        // DMA transmit
        if (transmitSize > 0) {
            if (HAL_UART_Transmit_DMA(getHandle(Peripheral::DEFAULT), &_txBuffer[_txBufferReadIdx], transmitSize) == HAL_OK) {
                _txBufferReadIdx = (_txBufferReadIdx + transmitSize) % TX_BUFFER_SIZE;
                _txDmaBusy = true;
            } else
                Log::error("Uart", "Failed to transmit, DMA error");
        }
    }

    // Start DMA circular receive
    if (!_rxDmaBusy) {
        if (HAL_UART_Receive_DMA(getHandle(Peripheral::DEFAULT), _rxBuffer, RX_BUFFER_SIZE) == HAL_OK)
            _rxDmaBusy = true;
        else
            Log::error("Uart", "Failed to start DMA receive");
    }
}

void Uart::transmit(uint8_t* data, uint32_t size) {
    if (!_initialized || !_txDmaLinked)
        return;
    uint32_t availableSpace = 0;
    uint32_t headToEnd = 0;

    // Calculate available space in the buffer
    if (_txBufferWriteIdx >= _txBufferReadIdx) {
        availableSpace = TX_BUFFER_SIZE - _txBufferWriteIdx + _txBufferReadIdx - 1;
        headToEnd = TX_BUFFER_SIZE - _txBufferWriteIdx;
    } else {
        availableSpace = _txBufferReadIdx - _txBufferWriteIdx - 1;
        headToEnd = availableSpace;
    }

    // Check if there is enough space
    if (size <= availableSpace) {
        // Copy data to buffer in two steps if needed (wrap-around case)
        uint32_t firstCopySize = std::min(size, headToEnd);
        std::memcpy(&_txBuffer[_txBufferWriteIdx], data, firstCopySize);
        if (size > headToEnd)
            std::memcpy(&_txBuffer[0], data + firstCopySize, size - firstCopySize);

        // Update write index
        _txBufferWriteIdx = (_txBufferWriteIdx + size) % TX_BUFFER_SIZE;

        update();
    } else
        Log::error("Uart", "Failed to transmit, not enough buffer space");
}

uint32_t Uart::receive(uint8_t* data, uint32_t size) {
    if (!_initialized || !_rxDmaLinked)
        return 0;
    uint32_t rxBufferWriteIdx = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(getHandle(Peripheral::DEFAULT)->hdmarx);

    uint32_t readSize = 0;
    if (rxBufferWriteIdx >= _rxBufferReadIdx)
        readSize = std::min(size, rxBufferWriteIdx - _rxBufferReadIdx);
    else
        readSize = std::min(size, RX_BUFFER_SIZE - _rxBufferReadIdx);
    std::memcpy(data, &_rxBuffer[_rxBufferReadIdx], readSize);
    _rxBufferReadIdx = (_rxBufferReadIdx + readSize) % RX_BUFFER_SIZE;

    return readSize;
}

// clang-format off
#define LINK_DMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__) \
    do {                                                        \
        (__HANDLE__)->__PPP_DMA_FIELD__ = (__DMA_HANDLE__);     \
        (__DMA_HANDLE__)->Parent = (__HANDLE__);                \
    } while (0U)
// clang-format on

void Uart::linkDmaTx(Peripheral peripheral, Dma::Handle* dmaHandle) {
    LINK_DMA(getHandle(peripheral), hdmatx, dmaHandle);
    _txDmaLinked = true;
}
void Uart::linkDmaRx(Peripheral peripheral, Dma::Handle* dmaHandle) {
    LINK_DMA(getHandle(peripheral), hdmarx, dmaHandle);
    _rxDmaLinked = true;
}

void Uart::transmitCompleteCallback(UART_HandleTypeDef* huart) { _txDmaBusy = false; }

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
