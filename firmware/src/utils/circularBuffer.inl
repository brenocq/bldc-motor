//--------------------------------------------------
// BLDC Motor Controller
// circularBuffer.inl
// Date: 2025-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <cstring>
#include <algorithm>

template <size_t BufferSize>
CircularBuffer<BufferSize>::CircularBuffer() : _head(0), _tail(0), _size(0) {}

template <size_t BufferSize>
bool CircularBuffer<BufferSize>::push(const uint8_t* data, size_t size) {
    if (size > getAvailableSpace())
        return false; // Not enough space

    // Copy in two parts if it wraps around
    size_t first_copy_size = std::min(size, BufferSize - _head);
    std::memcpy(&_buffer[_head], data, first_copy_size);

    if (first_copy_size < size)
        std::memcpy(&_buffer[0], data + first_copy_size, size - first_copy_size);

    _head = (_head + size) % BufferSize;
    _size += size;
    return true;
}

template <size_t BufferSize>
size_t CircularBuffer<BufferSize>::getContiguousReadSize() const {
    if (_head >= _tail) {
        return _size;
    } else {
        return BufferSize - _tail;
    }
}

template <size_t BufferSize>
uint8_t* CircularBuffer<BufferSize>::getReadPointer() {
    return &_buffer[_tail];
}

template <size_t BufferSize>
void CircularBuffer<BufferSize>::advanceRead(size_t size) {
    if (size > _size) {
        size = _size; // Cannot advance more than what's available
    }
    _tail = (_tail + size) % BufferSize;
    _size -= size;
}

template <size_t BufferSize>
size_t CircularBuffer<BufferSize>::getAvailableSpace() const {
    return BufferSize - _size;
}

template <size_t BufferSize>
void CircularBuffer<BufferSize>::clear() {
    _head = 0;
    _tail = 0;
    _size = 0;
}
