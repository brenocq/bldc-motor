//--------------------------------------------------
// BLDC Motor Controller
// DMA.cpp
// Date: 2023-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/dma/dma.h>
#include <utils/log.h>

namespace Dma {

DMA_HandleTypeDef* getHandle(Dma dma, Stream stream);
DMA_Stream_TypeDef* getInstance(Dma dma, Stream stream);

enum Source { PERIPHERAL = 0, MEMORY };
uint32_t convert(Channel channel);
uint32_t convert(Direction direction);
uint32_t convert(Increment increment, Source source);
uint32_t convert(Alignment alignment, Source source);
uint32_t convert(Priority priority);

DMA_HandleTypeDef hdma1stream0;
DMA_HandleTypeDef hdma1stream1;
DMA_HandleTypeDef hdma1stream2;
DMA_HandleTypeDef hdma1stream3;
DMA_HandleTypeDef hdma1stream4;
DMA_HandleTypeDef hdma1stream5;
DMA_HandleTypeDef hdma1stream6;
DMA_HandleTypeDef hdma1stream7;
DMA_HandleTypeDef hdma2stream0;
DMA_HandleTypeDef hdma2stream1;
DMA_HandleTypeDef hdma2stream2;
DMA_HandleTypeDef hdma2stream3;
DMA_HandleTypeDef hdma2stream4;
DMA_HandleTypeDef hdma2stream5;
DMA_HandleTypeDef hdma2stream6;
DMA_HandleTypeDef hdma2stream7;

} // namespace Dma

bool Dma::init() {
    // Check for dma/stream conflicts
    for (uint32_t i = 0; i < dmaList.size(); i++)
        for (uint32_t j = i + 1; j < dmaList.size(); j++)
            if (dmaList[i].dma == dmaList[j].dma && dmaList[i].stream == dmaList[j].stream) {
                Log::error("Dma", "Found conflict in dmaList: DMA$0 STREAM$1", int(dmaList[i].dma), int(dmaList[i].stream));
                return false;
            }

    // Check for request conflict
    for (uint32_t i = 0; i < dmaList.size(); i++) {
        const auto& mapping = dmaList[i].dma == DMA1 ? dma1Mapping : dma2Mapping;
        if (mapping[dmaList[i].channel][dmaList[i].stream] != dmaList[i].request) {
            Log::error("Dma", "Request for DMA$0 STREAM$1 CH$2 does not match dma mapping", int(dmaList[i].dma), int(dmaList[i].stream),
                       int(dmaList[i].channel));
            return false;
        }
    }

    // Initialize DMA
    bool usingDma1 = false;
    bool usingDma2 = false;
    for (uint32_t i = 0; i < dmaList.size(); i++) {
        if (dmaList[i].dma == DMA1 && !usingDma1) {
            __HAL_RCC_DMA1_CLK_ENABLE();
            usingDma1 = true;
        }
        if (dmaList[i].dma == DMA2 && !usingDma2) {
            __HAL_RCC_DMA2_CLK_ENABLE();
            usingDma2 = true;
        }
    }

    // Initialize DMA streams
    for (uint32_t i = 0; i < dmaList.size(); i++) {
        DMA_HandleTypeDef* hdma = getHandle(dmaList[i].dma, dmaList[i].stream);
        hdma->Instance = getInstance(dmaList[i].dma, dmaList[i].stream);
        hdma->Init.Channel = convert(dmaList[i].channel);
        hdma->Init.Direction = convert(dmaList[i].direction);
        hdma->Init.PeriphInc = convert(dmaList[i].increment, PERIPHERAL);
        hdma->Init.MemInc = convert(dmaList[i].increment, MEMORY);
        hdma->Init.PeriphDataAlignment = convert(dmaList[i].alignment, PERIPHERAL);
        hdma->Init.MemDataAlignment = convert(dmaList[i].alignment, MEMORY);
        hdma->Init.Mode = DMA_NORMAL;
        hdma->Init.Priority = convert(dmaList[i].priority);
        hdma->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(hdma) != HAL_OK)
            return false;
    }

    // TODO __HAL_LINKDMA
    // TODO NVIC

    Log::success("Dma", "Initialized");
    return true;
}

bool Dma::deinit() {
    __HAL_RCC_DMA1_CLK_DISABLE();
    __HAL_RCC_DMA2_CLK_DISABLE();
    return true;
}

DMA_HandleTypeDef* Dma::getHandle(Dma dma, Stream stream) {
    if (dma == DMA1) {
        switch (stream) {
            case STREAM0:
                return &hdma1stream0;
            case STREAM1:
                return &hdma1stream1;
            case STREAM2:
                return &hdma1stream2;
            case STREAM3:
                return &hdma1stream3;
            case STREAM4:
                return &hdma1stream4;
            case STREAM5:
                return &hdma1stream5;
            case STREAM6:
                return &hdma1stream6;
            case STREAM7:
                return &hdma1stream7;
        }
    } else if (dma == DMA2) {
        switch (stream) {
            case STREAM0:
                return &hdma2stream0;
            case STREAM1:
                return &hdma2stream1;
            case STREAM2:
                return &hdma2stream2;
            case STREAM3:
                return &hdma2stream3;
            case STREAM4:
                return &hdma2stream4;
            case STREAM5:
                return &hdma2stream5;
            case STREAM6:
                return &hdma2stream6;
            case STREAM7:
                return &hdma2stream7;
        }
    }
    return nullptr;
}

DMA_Stream_TypeDef* Dma::getInstance(Dma dma, Stream stream) {
    if (dma == DMA1) {
        switch (stream) {
            case STREAM0:
                return DMA1_Stream0;
            case STREAM1:
                return DMA1_Stream1;
            case STREAM2:
                return DMA1_Stream2;
            case STREAM3:
                return DMA1_Stream3;
            case STREAM4:
                return DMA1_Stream4;
            case STREAM5:
                return DMA1_Stream5;
            case STREAM6:
                return DMA1_Stream6;
            case STREAM7:
                return DMA1_Stream7;
        }
    } else if (dma == DMA2) {
        switch (stream) {
            case STREAM0:
                return DMA2_Stream0;
            case STREAM1:
                return DMA2_Stream1;
            case STREAM2:
                return DMA2_Stream2;
            case STREAM3:
                return DMA2_Stream3;
            case STREAM4:
                return DMA2_Stream4;
            case STREAM5:
                return DMA2_Stream5;
            case STREAM6:
                return DMA2_Stream6;
            case STREAM7:
                return DMA2_Stream7;
        }
    }
    return nullptr;
}

uint32_t Dma::convert(Channel channel) {
    switch (channel) {
        case CH0:
            return DMA_CHANNEL_0;
        case CH1:
            return DMA_CHANNEL_1;
        case CH2:
            return DMA_CHANNEL_2;
        case CH3:
            return DMA_CHANNEL_3;
        case CH4:
            return DMA_CHANNEL_4;
        case CH5:
            return DMA_CHANNEL_5;
        case CH6:
            return DMA_CHANNEL_6;
        case CH7:
            return DMA_CHANNEL_7;
    }
    return 0;
}

uint32_t Dma::convert(Direction direction) {
    switch (direction) {
        case PER_TO_MEM:
            return DMA_PERIPH_TO_MEMORY;
        case MEM_TO_PER:
            return DMA_MEMORY_TO_PERIPH;
        case MEM_TO_MEM:
            return DMA_MEMORY_TO_MEMORY;
    }
    return 0;
}

uint32_t Dma::convert(Increment increment, Source source) {
    if (source == PERIPHERAL)
        return (increment == INC_PER || increment == INC_MEM_PER) ? DMA_PINC_ENABLE : DMA_PINC_DISABLE;
    else if (source == MEMORY)
        return (increment == INC_MEM || increment == INC_MEM_PER) ? DMA_MINC_ENABLE : DMA_MINC_DISABLE;
    return 0;
}

uint32_t Dma::convert(Alignment alignment, Source source) {
    if (source == PERIPHERAL) {
        switch (alignment) {
            case ALIGN_8BIT:
                return DMA_PDATAALIGN_BYTE;
            case ALIGN_16BIT:
                return DMA_PDATAALIGN_HALFWORD;
            case ALIGN_32BIT:
                return DMA_PDATAALIGN_WORD;
        }
    } else if (source == MEMORY) {
        switch (alignment) {
            case ALIGN_8BIT:
                return DMA_MDATAALIGN_BYTE;
            case ALIGN_16BIT:
                return DMA_MDATAALIGN_HALFWORD;
            case ALIGN_32BIT:
                return DMA_MDATAALIGN_WORD;
        }
    }
    return 0;
}

uint32_t Dma::convert(Priority priority) {
    switch (priority) {
        case PRIORITY_LOW:
            return DMA_PRIORITY_LOW;
        case PRIORITY_MEDIUM:
            return DMA_PRIORITY_MEDIUM;
        case PRIORITY_HIGH:
            return DMA_PRIORITY_HIGH;
        case PRIORITY_VERY_HIGH:
            return DMA_PRIORITY_VERY_HIGH;
    }
    return 0;
}
