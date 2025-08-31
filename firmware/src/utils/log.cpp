//--------------------------------------------------
// BLDC Motor Controller
// log.cpp
// Date: 2023-09-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/uart/uart.h>
#include <utils/log.h>

//#define ENABLE_UART_LOG
#define ENABLE_ITM_LOG

void Log::transmit(const std::string& str) {
#ifdef ENABLE_UART_LOG
    // Send log through UART
    if (Uart::isInitialized() && !str.empty())
        Uart::transmit((uint8_t*)str.data(), str.size());
#endif

#ifdef ENABLE_ITM_LOG
    // Send log through ITM
    for (size_t i = 0; i < str.size(); i++)
        ITM_SendChar(str[i]);
#endif
}
