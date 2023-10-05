//--------------------------------------------------
// BLDC Motor Controller
// attaConnector.cpp
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "attaConnector.h"
#include <array>
#include <cmath>
#include <cstring>
#include <utils/log.h>

namespace AttaConnector {

//---------- Platform specific ----------//
__attribute__((weak)) bool transmitBytes(uint8_t* data, uint32_t len) { return false; }
__attribute__((weak)) uint32_t numAvailableBytes() { return 0; }
__attribute__((weak)) bool receiveBytes(uint8_t* data, uint32_t len) { return false; }

//---------- Buffer helpers ----------//
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

template <uint32_t Size>
class StackBuffer {
  public:
    StackBuffer();
    uint32_t size() const;
    uint8_t* data();
    bool full() const;
    bool empty() const;
    void reset();
    uint8_t* push(uint8_t* data, uint32_t len);

    static constexpr uint32_t ALIGN = sizeof(uint8_t*);

  private:
    std::array<uint8_t, Size> _buffer;
    uint32_t _end;
};
//---------- CRC ----------//
constexpr uint8_t _crcPoly = 0b00000111;
constexpr std::array<uint8_t, 256> generateCRCTable();
uint8_t crc(uint8_t pktId, uint8_t cmdId, uint8_t* payload, uint32_t payloadSize);
//---------- COBS ----------//
void cobsEncode(uint8_t* message, uint32_t size, uint8_t* encoded, uint32_t* encodedSize);
void cobsDecode(uint8_t* encoded, uint32_t* decodedSize);
constexpr uint32_t cobsMaxEncodedSize(uint32_t size) { return size + std::ceil(size / 254.0f) + 1; }
std::array<uint8_t, cobsMaxEncodedSize(MAX_CMD_SIZE)> _packetTx;
std::array<uint8_t, cobsMaxEncodedSize(MAX_CMD_SIZE)> _packetRx;
uint32_t _packetRxIdx;
//---------- TX Handler ----------//
#define MAX_PACKET_ID 0xFF
class TxHandler {
  public:
    TxHandler();

    bool createPacket(uint8_t cmdId, uint8_t* payload, uint32_t payloadSize);

    bool getNextPacketToTransmit(uint8_t* pktId, uint8_t** data, uint32_t* size);

    void markTransmitted(uint8_t pktId);
    void markACK(uint8_t pktId);
    void markNACK(uint8_t pktId);

  private:
    struct PacketInfo {
        uint32_t idx;
        uint32_t size;
        bool transmitted = false;
    };

    CircularBuffer<TX_SIZE> _txBuffer;
    std::array<PacketInfo, MAX_PACKET_ID + 1> _packets;
    uint8_t _begin;
    uint8_t _end;
    uint8_t _full;
    uint8_t _lastTransmitted;
};
TxHandler _txHandler;

//---------- RX Handler ----------//
#define MAX_COMMAND_ID 0xFF
struct AckCmd {
    ATTA_CONNECTOR_CMD(RESERVED_CMD);
    enum Status {
        ACK = 0x00,       // Packet received
        NACK_CRC = 0x01,  // Packet corrupted (wrong CRC)
        NACK_LOST = 0x02, // Packet lost
        NACK_FULL = 0x03, // RX full, packet could not be received
    };
    uint8_t pkt;
    Status status;
};
bool _firstPktId;
uint8_t _lastRxPktId; // Packet ID of the last received packet

class RxHandler {
  public:
    RxHandler();

    void clear();
    bool canPush(uint32_t payloadSize);
    bool pushCommand(uint8_t cmdId, uint8_t* payload, uint32_t size);
    bool getNextCommandSize(uint8_t cmdId, uint32_t* size);
    bool popCommand(uint8_t cmdId, uint8_t* payload, uint32_t* size);

  private:
    StackBuffer<RX_SIZE> _rxBuffer;
    std::array<uint8_t*, MAX_COMMAND_ID + 1> _rxCommands; // Pointer to first received payload of each command
};
RxHandler _rxHandler;

} // namespace AttaConnector

bool AttaConnector::init() {
    _firstPktId = true;
    return true;
}

void AttaConnector::update() {
    // Clear old received commands
    _rxHandler.clear();

    // Transmit packets
    uint8_t pktId;
    uint8_t* pktStart;
    uint32_t pktSize;
    while (_txHandler.getNextPacketToTransmit(&pktId, &pktStart, &pktSize)) {
        uint32_t encodedPacketSize = 0;
        cobsEncode(pktStart, pktSize, _packetTx.data(), &encodedPacketSize);
        if (transmitBytes(_packetTx.data(), encodedPacketSize))
            _txHandler.markTransmitted(pktId);
    }

    // Receive packets
    bool fullPacketReceived = false;
    uint8_t data;
    while (numAvailableBytes()) {
        fullPacketReceived = false;
        // For now, receive bytes one by one
        if (receiveBytes(&data, 1)) {
            _packetRx[_packetRxIdx] = data;
            _packetRxIdx++;
            if (data == 0) {
                fullPacketReceived = true;
                _packetRxIdx = 0;
            }
        }

        // Process received packet
        if (fullPacketReceived) {
            LOG_DEBUG("AttaConnector", "Full packet received");
            uint32_t packetSize = 0;
            cobsDecode(_packetRx.data(), &packetSize);
            uint8_t pktId = _packetRx[0];
            uint8_t cmdId = _packetRx[1];
            uint8_t* payload = &_packetRx[2];
            uint8_t pktCRC = _packetRx[packetSize - 1];
            uint8_t receivedCRC = crc(pktId, cmdId, payload, packetSize - 3 * sizeof(uint8_t));
            LOG_DEBUG("AttaConnector", "Received pktId $0 cmdId $1 payloadSize $2", (int)pktId, (int)cmdId, packetSize - 3);
            if (pktCRC == receivedCRC) {
                // Accept first packet with any ID
                if (_firstPktId) {
                    _lastRxPktId = pktId - 1;
                    LOG_DEBUG("AttaConnector", "First pkt -> $0 $1", (int)(_lastRxPktId + 1), (int)pktId);
                    _firstPktId = false;
                }

                // Check if some packet was lost (pktIds should be sequential)
                if (pktId != uint8_t(_lastRxPktId + 1)) {
                    AckCmd ack{};
                    ack.pkt = _lastRxPktId + 1;
                    ack.status = AckCmd::NACK_LOST;
                    transmit<AckCmd>(ack);
                    LOG_DEBUG("AttaConnector", "NACK LOST $0 but last was $1", (int)pktId, (int)_lastRxPktId);
                    continue;
                }

                // Check if payload fits RX buffer
                if (!_rxHandler.canPush(packetSize)) {
                    AckCmd ack{};
                    ack.pkt = pktId;
                    ack.status = AckCmd::NACK_FULL;
                    transmit<AckCmd>(ack);
                    LOG_DEBUG("AttaConnector", "NACK FULL");
                    continue;
                }

                LOG_DEBUG("AttaConnector", "Push command");
                _lastRxPktId = pktId;
                _rxHandler.pushCommand(cmdId, payload, packetSize - sizeof(pktId) - sizeof(cmdId) - sizeof(pktCRC));

                // Transmit ACK
                LOG_DEBUG("AttaConnector", "Transmit ACK");
                if (cmdId != RESERVED_CMD) {
                    AckCmd ack{};
                    ack.pkt = pktId;
                    ack.status = AckCmd::ACK;
                    transmit<AckCmd>(ack);
                }
            }
            LOG_DEBUG("AttaConnector", "CRC match: $0", pktCRC == receivedCRC);
        }
    }

    // Process received ACK command
    AckCmd ack;
    while (receive<AckCmd>(&ack)) {
        switch (ack.status) {
            case AckCmd::ACK:
                LOG_DEBUG("AttaConnector", "Process received ACK");
                _txHandler.markACK(ack.pkt);
                break;
            case AckCmd::NACK_CRC:
                LOG_DEBUG("AttaConnector", "Process received NACK_CRC");
                _txHandler.markNACK(ack.pkt);
                break;
            case AckCmd::NACK_LOST:
                LOG_DEBUG("AttaConnector", "Process received NACK_LOST");
                _txHandler.markNACK(ack.pkt);
                break;
            case AckCmd::NACK_FULL:
                LOG_DEBUG("AttaConnector", "Process received NACK_FULL");
                _txHandler.markNACK(ack.pkt);
                break;
        }
    }
}

bool AttaConnector::transmit(uint8_t cmdId, uint8_t* data, uint32_t len) { return pushPacket(cmdId, data, len); }

bool AttaConnector::receive(uint8_t cmdId, uint8_t* data, uint32_t* len) { return popPacket(cmdId, data, len); }

//-------------------- Push Packet --------------------//
bool AttaConnector::pushPacket(uint8_t cmdId, uint8_t* payload, uint32_t payloadSize) { return _txHandler.createPacket(cmdId, payload, payloadSize); }

//-------------------- Pop Packet --------------------//
bool AttaConnector::popPacket(uint8_t cmdId, uint8_t* payload, uint32_t* payloadSize) { return _rxHandler.popCommand(cmdId, payload, payloadSize); }

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
    uint8_t* encodedStart = encoded;
    uint8_t* zeroPos = encoded++; // Remember where the last length byte was written
    uint32_t nonZeroCount = 0;
    uint32_t mi = 0;

    while (mi < size) {
        if (message[mi] == 0) {
            *zeroPos = nonZeroCount + 1;
            zeroPos = encoded++;
            nonZeroCount = 0;
            mi++;
        } else {
            *encoded++ = message[mi++];
            if (++nonZeroCount == 0xFE) { // If we've copied 254 non-zero bytes
                *zeroPos = 0xFF;
                zeroPos = encoded++;
                nonZeroCount = 0;
            }
        }
    }

    *zeroPos = nonZeroCount + 1;
    *encoded++ = 0; // COBS end delimiter
    *encodedSize = encoded - encodedStart;
}

void AttaConnector::cobsDecode(uint8_t* encoded, uint32_t* decodedSize) {
    uint8_t* readPos = encoded;
    uint8_t* writePos = encoded;

    while (true) {
        uint8_t length = *readPos++;
        // COBS end delimiter
        if (length == 0)
            break;

        // Copy encoded non-zero data
        for (uint8_t i = 1; i < length; i++)
            *writePos++ = *readPos++;

        // Handle encoded zero data
        if (length < 0xFF)
            *writePos++ = 0;
    }

    // Set decodedSize to the length of the decoded data
    *decodedSize = (writePos - encoded) - 1;
}

//-------------------- Circular Buffer --------------------//
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

//-------------------- Stack Buffer --------------------//
template <uint32_t Size>
AttaConnector::StackBuffer<Size>::StackBuffer() : _end(0) {}

template <uint32_t Size>
uint32_t AttaConnector::StackBuffer<Size>::size() const {
    return _end;
}

template <uint32_t Size>
uint8_t* AttaConnector::StackBuffer<Size>::data() {
    return _buffer.data();
}

template <uint32_t Size>
bool AttaConnector::StackBuffer<Size>::full() const {
    return _end == Size;
}

template <uint32_t Size>
bool AttaConnector::StackBuffer<Size>::empty() const {
    return _end == 0;
}

template <uint32_t Size>
void AttaConnector::StackBuffer<Size>::reset() {
    _end = 0;
}

template <uint32_t Size>
uint8_t* AttaConnector::StackBuffer<Size>::push(uint8_t* data, uint32_t len) {
    // Check if data can fit
    uint32_t spaceNeeded = len + (ALIGN - (len % ALIGN)) % ALIGN;
    if (_end + spaceNeeded > Size)
        return nullptr;

    // Copy data
    std::memcpy(_buffer.data() + _end, data, len);

    // Adjust end for the next push
    _end += spaceNeeded;

    // Return pointer to the pushed data
    return _buffer.data() + _end - spaceNeeded;
}

//-------------------- TX Handler --------------------//
AttaConnector::TxHandler::TxHandler() : _begin(0), _end(0), _full(false), _lastTransmitted(MAX_PACKET_ID) {}

bool AttaConnector::TxHandler::createPacket(uint8_t cmdId, uint8_t* payload, uint32_t payloadSize) {
    // Check if there are packet IDs available
    if (_full)
        return false;

    // Check if packet data fits in the TX buffer continuously
    uint8_t pktId = 0;
    uint8_t pktCRC = 0;
    uint32_t pktSize = sizeof(pktId) + sizeof(cmdId) + payloadSize + sizeof(pktCRC);
    if (!_txBuffer.makeContinuous(pktSize))
        return false;

    // Push packet info
    _packets[_end].idx = _txBuffer.end();
    _packets[_end].size = pktSize;
    _packets[_end].transmitted = false;

    pktId = _end;
    pktCRC = crc(pktId, cmdId, payload, payloadSize);
    _end = (_end + 1) % _packets.size();
    if (_end == _begin)
        _full = true;

    // Push packet data
    _txBuffer.push(pktId);
    _txBuffer.push(cmdId);
    _txBuffer.push(payload, payloadSize);
    _txBuffer.push(pktCRC);

    return true;
}

bool AttaConnector::TxHandler::getNextPacketToTransmit(uint8_t* pktId, uint8_t** data, uint32_t* size) {
    // If all packets have been transmitted
    if ((_lastTransmitted + 1) % _packets.size() == _end)
        return false;

    // Set the last transmitted to the next packet in the range
    uint8_t nextToTransmit = (_lastTransmitted + 1) % _packets.size();
    *pktId = nextToTransmit;
    *data = _txBuffer.data() + _packets[nextToTransmit].idx;
    *size = _packets[nextToTransmit].size;
    return true;
}

void AttaConnector::TxHandler::markTransmitted(uint8_t pktId) { _lastTransmitted = pktId; }

void AttaConnector::TxHandler::markACK(uint8_t pktId) {
    // Clear packets up to and including the acknowledged one
    _begin = (pktId + 1) % _packets.size();
    _full = false;
}

void AttaConnector::TxHandler::markNACK(uint8_t pktId) {
    // If the NACK is for a packet after the last transmitted packet, reset the last transmitted
    _lastTransmitted = (pktId - 1 + _packets.size()) % _packets.size();
}

//-------------------- RX Handler --------------------//
AttaConnector::RxHandler::RxHandler() { clear(); }

void AttaConnector::RxHandler::clear() {
    _rxBuffer.reset();
    for (auto& ptr : _rxCommands)
        ptr = nullptr;
}

bool AttaConnector::RxHandler::canPush(uint32_t payloadSize) { return (RX_SIZE - _rxBuffer.size()) > payloadSize + StackBuffer<RX_SIZE>::ALIGN; }

bool AttaConnector::RxHandler::pushCommand(uint8_t cmdId, uint8_t* payload, uint32_t size) {
    uint32_t startIdx = _rxBuffer.size();

    uint32_t next = 0; // This will always be 0 since we are appending at the end of buffer
    uint8_t* nextLocation = _rxBuffer.push(reinterpret_cast<uint8_t*>(&next), sizeof(uint32_t));
    if (!nextLocation)
        return false;

    uint8_t* sizeLocation = _rxBuffer.push(reinterpret_cast<uint8_t*>(&size), sizeof(uint32_t));
    if (!sizeLocation)
        return false;

    uint8_t* payloadLocation = _rxBuffer.push(payload, size);
    if (!payloadLocation)
        return false;

    // If not first command inserted, update next pointer
    if (_rxCommands[cmdId])
        *((uint32_t*)_rxCommands[cmdId]) = startIdx;

    // If the cmdId was not previously set, set it now
    if (!_rxCommands[cmdId])
        _rxCommands[cmdId] = nextLocation;

    return true;
}

bool AttaConnector::RxHandler::getNextCommandSize(uint8_t cmdId, uint32_t* size) {
    if (!_rxCommands[cmdId])
        return false;

    uint8_t* currentLocation = _rxCommands[cmdId];
    *size = *reinterpret_cast<uint32_t*>(currentLocation + sizeof(uint32_t));
    return true;
}

bool AttaConnector::RxHandler::popCommand(uint8_t cmdId, uint8_t* payload, uint32_t* size) {
    if (!_rxCommands[cmdId])
        return false;

    uint8_t* currentLocation = _rxCommands[cmdId];
    *size = *reinterpret_cast<uint32_t*>(currentLocation + sizeof(uint32_t));
    std::memcpy(payload, currentLocation, *size);

    uint32_t next = *reinterpret_cast<uint32_t*>(currentLocation + sizeof(uint32_t) + sizeof(uint32_t));
    if (next == 0)
        _rxCommands[cmdId] = nullptr; // We've read the last command of this ID
    else
        _rxCommands[cmdId] = _rxBuffer.data() + next;

    return true;
}
