//--------------------------------------------------
// BLDC Simulation
// attaConnectorPlatform.cpp
// Date: 2023-10-07
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace AttaConnector {

bool transmitBytes(uint8_t* data, uint32_t size) {
    std::shared_ptr<atta::io::Serial> serial = _gSerial.lock();
    if (serial) {
        serial->transmit(data, size);
        return true;
    } else
        return false;
}

uint32_t receiveBytes(uint8_t* data, uint32_t size) {
    std::shared_ptr<atta::io::Serial> serial = _gSerial.lock();
    if (serial)
        return serial->receive(data, size);
    else
        return 0;
}

void log(const char* str) { LOG_INFO("AttaConnector", str); }

} // namespace AttaConnector
