//--------------------------------------------------
// BLDC Motor Controller
// circularBuffer.h
// Date: 2025-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_UTILS_CIRCULAR_BUFFER_H
#define BLDC_UTILS_CIRCULAR_BUFFER_H

#include <cstddef>
#include <cstdint>

/**
 * @brief A template-based circular buffer implementation.
 *
 * This class provides a simple and efficient circular buffer.
 * @tparam BufferSize The maximum number of bytes the buffer can hold.
 */
template <size_t BufferSize>
class CircularBuffer {
  public:
    CircularBuffer();

    bool push(const uint8_t* data, size_t size);
    size_t getContiguousReadSize() const;
    uint8_t* getReadPointer();
    void advanceRead(size_t size);
    size_t getAvailableSpace() const;
    void clear();

  private:
    uint8_t _buffer[BufferSize];
    volatile size_t _head;
    volatile size_t _tail;
    volatile size_t _size;
};

#include <utils/circularBuffer.inl>

#endif // BLDC_UTILS_CIRCULAR_BUFFER_H
