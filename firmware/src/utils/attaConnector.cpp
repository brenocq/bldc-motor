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

uint32_t _txSize;
uint32_t _rxSize;

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

} // namespace AttaConnector

bool AttaConnector::init() {
    _txSize = 0;
    _rxSize = 0;
    return true;
}

void AttaConnector::update() {
    if (numAvailableBytes()) {
    }
}

uint32_t AttaConnector::getTxSize() { return _txSize; }

uint32_t AttaConnector::getRxSize() { return _rxSize; }

void AttaConnector::txPush(uint8_t data) {
    if (_txSize < TX_SIZE)
        _tx[_txSize++] = data;
}

uint8_t AttaConnector::rxPop() { return 0; }

uint8_t AttaConnector::crc(uint8_t* message, uint32_t size) {
    uint8_t remainder = 0;
    for (uint8_t i = 0; i < size; i++)
        remainder = _crcTable[message[i] ^ remainder];
    return remainder;
}

void AttaConnector::cobsEncode(uint8_t* message, uint32_t size, uint8_t* encoded, uint32_t* encodedSize) {
    uint8_t ei0 = 0; // Last index in encoded where 0 was found
    uint32_t mi = 0;
    uint32_t ei = 1;
    while (mi < size) {
        if (ei - ei0 == 0xFF && message[mi] != 0) {
            // Handle zero pointer maximum value
            encoded[ei0] = ei - ei0;
            ei0 = ei;
            ei++;
        }

        if (message[mi] == 0) {
            // Encode zero
            encoded[ei0] = ei - ei0;
            ei0 = ei;
            ei++;
            mi++;
        } else {
            // Encode non-zero
            encoded[ei++] = message[mi];
            mi++;
        }
    }
    encoded[ei0] = ei - ei0;
    encoded[ei++] = 0;// COBS end delimiter
    *encodedSize = ei;
}
