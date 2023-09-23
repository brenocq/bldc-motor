//--------------------------------------------------
// BLDC Motor Controller
// attaConnector.inl
// Date: 2023-09-23
// By Breno Cunha Queiroz
//--------------------------------------------------
template <typename T>
bool AttaConnector::transmit(const T& cmd) {
    return false;
}

template <typename T>
bool AttaConnector::receive(T* cmd) {
    return false;
}
