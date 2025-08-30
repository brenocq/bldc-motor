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
#include <utils/circularBuffer.h>
#include <utils/log.h>

namespace Uart {

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

// Handle DMA transactions
bool _rxDmaLinked; ///< True if RX DMA is linked
bool _txDmaLinked; ///< True if TX DMA is linked

// Transmit buffer
CircularBuffer<TX_BUFFER_SIZE> _txBuffer;
bool _txDmaBusy;
volatile size_t _lastTxDmaSize = 0;

// Receive buffer
// _rxDmaBuffer is the raw buffer written to by the DMA hardware
uint8_t _rxDmaBuffer[RX_BUFFER_SIZE];
bool _rxDmaBusy;
// _rxBuffer is the application-side buffer where complete messages are stored
CircularBuffer<RX_BUFFER_SIZE> _rxBuffer;
// Tracks the last known position of the DMA write pointer in _rxDmaBuffer
volatile uint16_t _lastRxDmaPos = 0;

// Internal function to be called by the DMA complete ISR
void txDmaComplete();
// Internal function to be called by the UART idle line ISR
void rxDmaEvent(uint16_t size);

} // namespace Uart

bool Uart::init() {
    _initialized = false;
    _txDmaLinked = false;
    _rxDmaLinked = false;

    // Initialize buffers
    _txBuffer.clear();
    _txDmaBusy = false;
    _rxBuffer.clear();
    _lastRxDmaPos = 0;
    _rxDmaBusy = false;

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
    }

    // Set default peripheral
    if (!inUse.empty())
        _default = *inUse.begin();

    Log::success("Uart", "Initialized");
    return _initialized = true;
}

bool Uart::isInitialized() { return _initialized; }

void Uart::update() {
    if (!_initialized || !_txDmaLinked || !_rxDmaLinked)
        return;

    // Transmit pending transactions
    if (!_txDmaBusy) {
        // DMA transmit
        uint32_t transmitSize = _txBuffer.getContiguousReadSize();
        if (transmitSize > 0) {
            _lastTxDmaSize = transmitSize;
            uint8_t* dataToTransmit = _txBuffer.getReadPointer();
            if (HAL_UART_Transmit_DMA(getHandle(Peripheral::DEFAULT), dataToTransmit, transmitSize) == HAL_OK) {
                // We advance the read pointer ONLY after the transfer is complete, which will happen in the txDmaComplete() function
                _txDmaBusy = true;
            } else {
                Log::error("Uart", "Failed to transmit, DMA error");
                _lastTxDmaSize = 0; // Reset on failure
            }
        }
    }

    // Start DMA circular receive (if not already started)
    if (!_rxDmaBusy) {
        if (HAL_UART_Receive_DMA(getHandle(Peripheral::DEFAULT), _rxDmaBuffer, RX_BUFFER_SIZE) == HAL_OK)
            _rxDmaBusy = true;
        else
            Log::error("Uart", "Failed to start DMA receive");
    }
}

void Uart::transmit(uint8_t* data, uint32_t size) {
    if (!_initialized || !_txDmaLinked)
        return;
    if (!_txBuffer.push(data, size))
        Log::error("Uart", "Failed to transmit, TX buffer full");
}

uint32_t Uart::receive(uint8_t* data, uint32_t size) {
    if (!_initialized || !_rxDmaLinked)
        return 0;

    uint32_t readSize = _rxBuffer.getContiguousReadSize();
    if (readSize > size)
        readSize = size;

    if (readSize > 0) {
        std::memcpy(data, _rxBuffer.getReadPointer(), readSize);
        _rxBuffer.advanceRead(readSize);
    }

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

void Uart::txDmaComplete() {
    // Advance the read pointer by the size of the last chunk that was sent
    _txBuffer.advanceRead(_lastTxDmaSize);
    _lastTxDmaSize = 0;
    _txDmaBusy = false;
}

void Uart::rxDmaEvent(uint16_t size) {
    // Calculate the length of the newly received data chunk
    uint16_t len = 0;
    if (size > _lastRxDmaPos) {
        // Simple case: no wrap-around
        len = size - _lastRxDmaPos;
        _rxBuffer.push(&_rxDmaBuffer[_lastRxDmaPos], len);
    } else {
        // DMA has wrapped around the buffer
        // Part 1: from last position to the end of the buffer
        len = RX_BUFFER_SIZE - _lastRxDmaPos;
        _rxBuffer.push(&_rxDmaBuffer[_lastRxDmaPos], len);

        // Part 2: from the beginning of the buffer to the new position
        if (size > 0)
            _rxBuffer.push(&_rxDmaBuffer[0], size);
    }

    // Update the last known position
    _lastRxDmaPos = size;

    // Re-arm the DMA reception to catch the next message
    HAL_UARTEx_ReceiveToIdle_DMA(getHandle(_default), _rxDmaBuffer, RX_BUFFER_SIZE);
    _rxDmaBusy = true;
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

// --- HAL Callback Implementations ---
extern "C" {

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == Uart::getInstance(Uart::Peripheral::DEFAULT))
        Uart::txDmaComplete();
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    if (huart->Instance == Uart::getInstance(Uart::Peripheral::DEFAULT))
        Uart::rxDmaEvent(Size);
}

} // extern "C"
