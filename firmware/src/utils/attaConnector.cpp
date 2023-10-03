//--------------------------------------------------
// BLDC Motor Controller
// attaConnector.cpp
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "attaConnector.h"
#include <array>
#include <cmath>
#include <utils/log.h>

namespace AttaConnector {

//---------- Platform specific ----------//
__attribute__((weak)) bool transmitBytes(uint8_t* data, uint32_t len) { return false; }
__attribute__((weak)) uint32_t numAvailableBytes() { return 0; }
__attribute__((weak)) bool receiveBytes(uint8_t* data, uint32_t len) { return false; }

//---------- TX/RX buffers ----------//
template <uint32_t Size>
class CircularBuffer {
  public:
    CircularBuffer();
    uint32_t size() const;
    uint8_t* data();
    bool full() const;
    bool empty() const;
    bool makeContinuous(uint32_t len);
    void push(uint8_t value);
    void push(uint8_t* data, uint32_t len);
    void pop();
    void pop(uint32_t index);
    uint32_t begin() const;
    uint32_t end() const;

  private:
    std::array<uint8_t, Size> _buffer;
    uint32_t _begin, _end;
    bool _full;
};
CircularBuffer<TX_SIZE> _txBuffer;
CircularBuffer<RX_SIZE> _rxBuffer;
//---------- CRC ----------//
constexpr uint8_t _crcPoly = 0b00000111;
constexpr std::array<uint8_t, 256> generateCRCTable();
uint8_t crc(uint8_t pktId, uint8_t cmdId, uint8_t* payload, uint32_t payloadSize);
//---------- COBS ----------//
void cobsEncode(uint8_t* message, uint32_t size, uint8_t* encoded, uint32_t* encodedSize);
constexpr uint32_t cobsMaxEncodedSize(uint32_t size) { return size + std::ceil(size / 254.0f) + 1; }
std::array<uint8_t, cobsMaxEncodedSize(MAX_CMD_SIZE)> _encodedPacket;
//---------- TX Packet Handler ----------//
#define MAX_PACKET_ID 0b01111111
class TxPacketHandler {
  public:
    TxPacketHandler();

    bool canCreatePacket();
    uint8_t createPacket(uint32_t index, uint32_t size);

    bool getNextPacketToTransmit(uint8_t* pktId, uint32_t* index, uint32_t* size);

    void markTransmitted(uint8_t pktId);
    void markACK(uint8_t pktId);
    void markNACK(uint8_t pktId);

  private:
    struct PacketInfo {
        uint32_t idx;
        uint32_t size;
        bool transmitted = false;
    };

    std::array<PacketInfo, MAX_PACKET_ID + 1> _packets;
    uint8_t _begin;
    uint8_t _end;
    uint8_t _full;
    uint8_t _lastTransmitted;
};
TxPacketHandler _txPacketHandler;

} // namespace AttaConnector

bool AttaConnector::init() { return true; }

void AttaConnector::update() {
    // Transmit packets
    uint8_t pktId;
    uint32_t pktStart;
    uint32_t pktSize;
    while (_txPacketHandler.getNextPacketToTransmit(&pktId, &pktStart, &pktSize)) {
        uint32_t encodedPacketSize = 0;
        cobsEncode(&_txBuffer.data()[pktStart], pktSize, _encodedPacket.data(), &encodedPacketSize);
        if (transmitBytes(_encodedPacket.data(), encodedPacketSize))
            _txPacketHandler.markTransmitted(pktId);
    }

    // TODO Receive packets
    // if (numAvailableBytes()) {
    //}
}

//-------------------- Push Packet --------------------//
bool AttaConnector::pushPacket(uint8_t cmdId, uint8_t* payload, uint32_t payloadSize) {
    uint8_t pktId = 0;
    uint8_t pktCRC = crc(pktId, cmdId, payload, payloadSize);
    uint32_t pktSize = sizeof(pktId) + sizeof(cmdId) + payloadSize + sizeof(pktCRC);

    // Check if there are packet IDs available
    if (!_txPacketHandler.canCreatePacket())
        return false;

    // Check if packet data fits in the TX buffer continuously
    if (!_txBuffer.makeContinuous(pktSize))
        return false;

    // Create packet
    pktId = _txPacketHandler.createPacket(_txBuffer.end(), pktSize);

    // Push packet data
    _txBuffer.push(pktId);
    _txBuffer.push(cmdId);
    _txBuffer.push(payload, payloadSize);
    _txBuffer.push(pktCRC);

    return true;
}

//-------------------- Pop Packet --------------------//
bool AttaConnector::popPacket(uint8_t cmdId, uint8_t* payload, uint32_t payloadSize) { return false; }

//-------------------- CRC --------------------//
constexpr std::array<uint8_t, 256> AttaConnector::generateCRCTable() {
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
namespace AttaConnector {
constexpr std::array<uint8_t, 256> _crcTable = generateCRCTable();
}

uint8_t AttaConnector::crc(uint8_t pktId, uint8_t cmdId, uint8_t* payload, uint32_t payloadSize) {
    uint8_t remainder = 0;
    remainder = _crcTable[pktId ^ remainder];
    remainder = _crcTable[cmdId ^ remainder];
    for (uint8_t i = 0; i < payloadSize; i++)
        remainder = _crcTable[payload[i] ^ remainder];
    return remainder;
}

//-------------------- COBS --------------------//
void AttaConnector::cobsEncode(uint8_t* message, uint32_t size, uint8_t* encoded, uint32_t* encodedSize) {
    uint8_t ei0 = 0; // Last index in encoded where 0 was found
    uint32_t mi = 0;
    uint32_t ei = 1;
    while (mi < size) {
        if (ei - ei0 == 0xFF) {
            // Handle zero pointer maximum value
            encoded[ei0] = ei - ei0;
            ei0 = ei;
            ei++;
        } else if (message[mi] == 0) {
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
    encoded[ei0] = ei - ei0; // Update last zero pointer
    encoded[ei++] = 0;       // COBS end delimiter
    *encodedSize = ei;
}

//-------------------- TX/RX buffer --------------------//
template <uint32_t Size>
AttaConnector::CircularBuffer<Size>::CircularBuffer() : _begin(0), _end(0), _full(false) {}

template <uint32_t Size>
bool AttaConnector::CircularBuffer<Size>::makeContinuous(uint32_t len) {
    // Can't fit
    if (Size - size() < len)
        return false;

    if (_begin < _end) {
        if (Size - _end > len) {
            // Can fit at the end
            return true;
        } else if (_begin > len) {
            // Can fit at the beginning
            _end = 0;
            return true;
        } else {
            // Can't fit continuously
            return false;
        }
    }

    // If _end < _begin, can fit because available size >= len

    return true;
}

template <uint32_t Size>
uint32_t AttaConnector::CircularBuffer<Size>::size() const {
    if (_full)
        return Size;
    else if (_end >= _begin)
        return _end - _begin;
    else
        return Size + _end - _begin;
}

template <uint32_t Size>
uint8_t* AttaConnector::CircularBuffer<Size>::data() {
    return _buffer.data();
}

template <uint32_t Size>
bool AttaConnector::CircularBuffer<Size>::full() const {
    return _full;
}

template <uint32_t Size>
bool AttaConnector::CircularBuffer<Size>::empty() const {
    return (!_full && (_begin == _end));
}

template <uint32_t Size>
void AttaConnector::CircularBuffer<Size>::push(uint8_t value) {
    if (!_full) {
        _buffer[_end] = value;
        _end = (_end + 1) % Size;
        _full = (_begin == _end);
    }
}

template <uint32_t Size>
void AttaConnector::CircularBuffer<Size>::push(uint8_t* data, uint32_t len) {
    if (len > (Size - size()))
        return;
    for (uint32_t i = 0; i < len; i++)
        push(data[i]);
}

template <uint32_t Size>
void AttaConnector::CircularBuffer<Size>::pop() {
    if (empty())
        return;
    _begin = (_begin + 1) % Size;
    _full = false;
}

template <uint32_t Size>
void AttaConnector::CircularBuffer<Size>::pop(uint32_t len) {
    if (len > size())
        return;
    _begin = (_begin + size) % Size;
    _full = false;
}

template <uint32_t Size>
uint32_t AttaConnector::CircularBuffer<Size>::begin() const {
    return _begin;
}

template <uint32_t Size>
uint32_t AttaConnector::CircularBuffer<Size>::end() const {
    return _end;
}

//-------------------- TX Packet Handler --------------------//
AttaConnector::TxPacketHandler::TxPacketHandler() : _begin(0), _end(0), _full(false), _lastTransmitted(MAX_PACKET_ID) {}

bool AttaConnector::TxPacketHandler::canCreatePacket() { return !_full; }

uint8_t AttaConnector::TxPacketHandler::createPacket(uint32_t index, uint32_t size) {
    if (!canCreatePacket())
        return -1;

    _packets[_end].idx = index;
    _packets[_end].size = size;
    _packets[_end].transmitted = false;

    uint8_t currentEnd = _end;

    _end = (_end + 1) % _packets.size();

    if (_end == _begin)
        _full = true;

    return currentEnd;
}

bool AttaConnector::TxPacketHandler::getNextPacketToTransmit(uint8_t* pktId, uint32_t* index, uint32_t* size) {
    // If all packets have been transmitted
    if ((_lastTransmitted + 1) % _packets.size() == _end)
        return false;

    // Set the last transmitted to the next packet in the range
    uint8_t nextToTransmit = (_lastTransmitted + 1) % _packets.size();
    *pktId = nextToTransmit;
    *index = _packets[nextToTransmit].idx;
    *size = _packets[nextToTransmit].size;
}

void AttaConnector::TxPacketHandler::markTransmitted(uint8_t pktId) { _lastTransmitted = pktId; }

void AttaConnector::TxPacketHandler::markACK(uint8_t pktId) {
    // Clear packets up to and including the acknowledged one
    _begin = (pktId + 1) % _packets.size();
    _full = false;
}

void AttaConnector::TxPacketHandler::markNACK(uint8_t pktId) {
    // If the NACK is for a packet after the last transmitted packet, reset the last transmitted
    _lastTransmitted = (pktId - 1 + _packets.size()) % _packets.size();
}
