//--------------------------------------------------
// BLDC Motor Controller
// attaConnector.cpp
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "attaConnector.h"

namespace AttaConnector {

__attribute__((weak)) bool transmitBytes(uint8_t* data, uint32_t len) { return false; }
__attribute__((weak)) uint32_t numAvailableBytes() { return 0; }
__attribute__((weak)) bool receiveBytes(uint8_t* data, uint32_t len) { return false; }

uint8_t _tx[TX_SIZE];
uint8_t _rx[RX_SIZE];

} // namespace AttaConnector

bool AttaConnector::init() { return true; }

void AttaConnector::update() {
    if (numAvailableBytes()) {

    }
}
