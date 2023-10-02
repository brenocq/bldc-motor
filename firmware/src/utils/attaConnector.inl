//--------------------------------------------------
// BLDC Motor Controller
// attaConnector.inl
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <array>
#include <utils/log.h>

constexpr float constexprCeil(float num) {
    int inum = static_cast<int>(num);
    if (num == float(inum)) {
        return inum;
    }
    return inum + (num > 0);
}

template <typename T>
bool AttaConnector::transmit(const T& cmd) {
    static uint8_t pktId = 0;
    static constexpr uint8_t cmdId = T::CMD_ID;
    static uint8_t msgCRC = 0;
    static constexpr uint32_t size = sizeof(pktId) + sizeof(cmdId) + sizeof(cmd) + sizeof(msgCRC);
    static std::array<uint8_t, size> message{};
    static constexpr uint32_t maxSize = size + constexprCeil(size / 254.0f) + 1;
    static std::array<uint8_t, maxSize> encoded{};

    // Populate message
    // [PKT ID] [CMD ID] [CMD PAYLOAD...] [CRC]
    message[0] = pktId;
    message[1] = cmdId;
    for (uint32_t i = 0; i < sizeof(T); i++)
        message[2 + i] = ((uint8_t*)&cmd)[i];
    msgCRC = (int)crc(message.data(), sizeof(pktId) + sizeof(cmdId) + sizeof(T));
    message[size - sizeof(msgCRC)] = msgCRC;
    LOG_DEBUG("AttaConnector", "Input $x0 $x1 $x2 $x3 $x4", (int)message[0], (int)message[1], (int)message[2], (int)message[3], (int)message[4]);

    // Encode message (COBS)
    uint32_t encodedSize = 0;
    // cobsEncode(message.data(), message.size(), encoded.data(), &encodedSize);
    std::array<uint8_t, 3> test = {0x00, 0x11, 0x00};
    cobsEncode(test.data(), test.size(), encoded.data(), &encodedSize);
    for (int i = 0; i < encodedSize; i++)
        LOG_DEBUG("AttaConnector", "Output $6 -> $x0", (int)encoded[i]);

    // Save to TX buffer

    return false;
}

template <typename T>
bool AttaConnector::receive(T* cmd) {
    return false;
}
