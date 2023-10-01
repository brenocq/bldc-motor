//--------------------------------------------------
// BLDC Motor Controller
// attaConnector.cpp
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "attaConnector.h"
#include <array>

namespace AttaConnector {

__attribute__((weak)) bool transmitBytes(uint8_t* data, uint32_t len) { return false; }
__attribute__((weak)) uint32_t numAvailableBytes() { return 0; }
__attribute__((weak)) bool receiveBytes(uint8_t* data, uint32_t len) { return false; }

uint8_t _tx[TX_SIZE];
uint8_t _rx[RX_SIZE];

uint32_t _txStart;
uint32_t _txEnd;
uint32_t _rxStart;
uint32_t _rxEnd;

constexpr uint8_t _crcPoly = 0b00000111;

constexpr std::array<uint8_t, 256> generateCRCTable() {
    std::array<uint8_t, 256> crcTable{};

    uint8_t remainder = 0;
    for (int dividend = 0; dividend < 256; dividend++) {
        remainder = dividend;
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (remainder & (1 << 7))
                remainder = (remainder << 1) ^ _crcPoly;
            else
                remainder = (remainder << 1);
        }
        crcTable[dividend] = remainder;
    }

    return crcTable;
}

constexpr std::array<uint8_t, 256> _crcTable = generateCRCTable();

uint8_t crc(uint8_t* message, uint32_t size) {
    uint8_t remainder = 0;
    for (uint8_t i = 0; i < size; i++)
        remainder = _crcTable[message[i] ^ remainder];
    return remainder;
}

} // namespace AttaConnector

bool AttaConnector::init() {
    _txStart = 0;
    _txEnd = 0;
    _rxStart = 0;
    _rxEnd = 0;
    return true;
}

void AttaConnector::update() {
    if (numAvailableBytes()) {
    }
}
