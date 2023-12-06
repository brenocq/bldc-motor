//--------------------------------------------------
// BLDC Motor Controller
// DMA.h
// Date: 2023-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_DMA_DMA_H
#define BLDC_DRIVERS_DMA_DMA_H
#include <cstdint>
#include <drivers/dma/dmaMapping.h>
#include <system/hal.h>

namespace Dma {

enum Dma : uint8_t { NONE = 0, DMA1, DMA2 };
enum Stream : uint8_t { STREAM0 = 0, STREAM1, STREAM2, STREAM3, STREAM4, STREAM5, STREAM6, STREAM7 };
enum Direction : uint8_t { PER_TO_MEM = 0, MEM_TO_PER, MEM_TO_MEM };
enum Channel : uint8_t { CH0 = 0, CH1, CH2, CH3, CH4, CH5, CH6, CH7 };
enum Alignment : uint8_t { ALIGN_8BIT = 0, ALIGN_16BIT, ALIGN_32BIT };
enum Priority : uint8_t { PRIORITY_LOW = 0, PRIORITY_MEDIUM, PRIORITY_HIGH, PRIORITY_VERY_HIGH };
enum Increment : uint8_t { INC_NONE = 0, INC_PER, INC_MEM, INC_MEM_PER };
using Handle = DMA_HandleTypeDef;

/**
 * @brief Initialize DMA driver
 *
 * Initialize DMA peripherals
 */
bool init();

/**
 * @brief Deinitialize DMA driver
 *
 * Deinitialize DMA peripherals
 */
bool deinit();

/**
 * @brief Get DMA handle
 *
 * @param dma DMA peripheral
 * @param stream DMA stream
 *
 * @return DMA handle
 */
Handle* getHandle(Dma dma, Stream stream);

//---------- DMA streams ----------//
static constexpr Dma LED_DMA = DMA1;
static constexpr Stream LED_STREAM = STREAM5;

//---------- DMA configs ----------//
struct DmaConfig {
    Request request;
    Dma dma;
    Stream stream;
    Channel channel;
    Direction direction;
    Alignment alignment = ALIGN_8BIT; ///< Alignment for both peripheral and memory
    Priority priority = PRIORITY_LOW;
    Increment increment = INC_NONE;
};

inline const std::array dmaList{
    DmaConfig{TIM2_CH1, LED_DMA, LED_STREAM, CH3, MEM_TO_PER, ALIGN_32BIT, PRIORITY_LOW, INC_MEM},
};

}; // namespace Dma

#endif // BLDC_DRIVERS_DMA_DMA_H
