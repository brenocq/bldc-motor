//--------------------------------------------------
// BLDC Motor Controller
// log.cpp
// Date: 2023-09-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/uart/uart.h>
#include <utils/log.h>

void Log::transmit(std::string str) {
    if (Uart::isInitialized() && !str.empty())
        Uart::transmit((uint8_t*)str.data(), str.size());
}
