//--------------------------------------------------
// BLDC Motor Controller
// log.cpp
// Date: 2023-09-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <utils/log.h>
#include <drivers/uart/uart.h>

void Log::transmit(std::string str) {
    if(Uart::isInitialized())
        Uart::transmit(str);
}
