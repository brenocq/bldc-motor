//--------------------------------------------------
// BLDC Motor Controller
// dmaMapping.h
// Date: 2023-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_DMA_DMA_MAPPING_H
#define BLDC_DRIVERS_DMA_DMA_MAPPING_H
#include <array>

namespace Dma {

enum Request {
    NA = 0, ///< Not Available
    ADC1,
    ADC2,
    ADC3,
    DAC1,
    DAC2,
    DCMI,
    FMPI2C1_RX,
    FMPI2C1_TX,
    I2C1_RX,
    I2C1_TX,
    I2C2_RX,
    I2C2_TX,
    I2C3_RX,
    I2C3_TX,
    QUADSPI,
    SAI1_A,
    SAI1_B,
    SAI2_A,
    SAI2_B,
    SDIO,
    SPDIFRX_CS,
    SPDIFRX_DT,
    SPI1_RX,
    SPI1_TX,
    SPI2_RX,
    SPI2_TX,
    SPI3_RX,
    SPI3_TX,
    SPI4_RX,
    SPI4_TX,
    TIM1_CH1,
    TIM1_CH1_CH2_CH3,
    TIM1_CH2,
    TIM1_CH3,
    TIM1_CH4_TRIG_COM,
    TIM1_TRIG,
    TIM1_UP,
    TIM2_CH1,
    TIM2_CH2_CH4,
    TIM2_CH3,
    TIM2_CH4,
    TIM2_UP,
    TIM2_UP_CH3,
    TIM2_UP_CH4,
    TIM3_CH1_TRIG,
    TIM3_CH2,
    TIM3_CH3,
    TIM3_CH4_UP,
    TIM3_TRIG,
    TIM3_UP,
    TIM4_CH1,
    TIM4_CH2,
    TIM4_CH3,
    TIM4_UP,
    TIM5_CH1,
    TIM5_CH2,
    TIM5_CH3_UP,
    TIM5_CH4,
    TIM5_CH4_TRIG,
    TIM5_TRIG,
    TIM5_UP,
    TIM6_UP,
    TIM7_UP,
    TIM8_CH1,
    TIM8_CH1_CH2_CH3,
    TIM8_CH2,
    TIM8_CH3,
    TIM8_CH4_TRIG_COM,
    TIM8_UP,
    UART4_RX,
    UART4_TX,
    UART5_RX,
    UART5_TX,
    USART1_RX,
    USART1_TX,
    USART2_RX,
    USART2_TX,
    USART3_RX,
    USART3_TX,
    USART6_RX,
    USART6_TX
};

constexpr uint8_t NUM_STREAMS = 8;  ///< Number of streams
constexpr uint8_t NUM_CHANNELS = 8; ///< Number of stream channels

// clang-format off
/**
 * @brief DMA1 request mapping
 */
inline const std::array<std::array<Request, NUM_STREAMS>, NUM_CHANNELS> dma1Mapping{{
    {SPI3_RX,     SPDIFRX_DT,    SPI3_RX,     SPI2_RX,       SPI2_TX,       SPI3_TX,    SPDIFRX_CS,   SPI3_TX},
    {I2C1_RX,     I2C3_RX,       TIM7_UP,     NA,            TIM7_UP,       I2C1_RX,    I2C1_TX,      I2C1_TX},
    {TIM4_CH1,    NA,            FMPI2C1_RX,  TIM4_CH2,      NA,            FMPI2C1_TX, TIM4_UP,      TIM4_CH3},
    {NA,          TIM2_UP_CH3,   I2C3_RX,     NA,            I2C3_TX,       TIM2_CH1,   TIM2_CH2_CH4, TIM2_UP_CH4},
    {UART5_RX,    USART3_RX,     UART4_RX,    USART3_TX,     UART4_TX,      USART2_RX,  USART2_TX,    UART5_TX},
    {NA,          NA,            TIM3_CH4_UP, NA,            TIM3_CH1_TRIG, TIM3_CH2,   NA,           TIM3_CH3},
    {TIM5_CH3_UP, TIM5_CH4_TRIG, TIM5_CH1,    TIM5_CH4_TRIG, TIM5_CH2,      NA,         TIM5_UP,      NA},
    {NA,          TIM6_UP,       I2C2_RX,     I2C2_RX,       USART3_TX,     DAC1,       DAC2,         I2C2_TX},
}};

/**
 * @brief DMA2 request mapping
 */
inline const std::array<std::array<Request, NUM_STREAMS>, NUM_CHANNELS> dma2Mapping{{
    {ADC1,      SAI1_A,    TIM8_CH1_CH2_CH3, SAI1_A,   ADC1,              SAI1_B,    TIM1_CH1_CH2_CH3, SAI2_B},
    {NA,        DCMI,      ADC2,             ADC2,     SAI1_B,            NA,        NA,               DCMI},
    {ADC3,      ADC3,      NA,               NA,       NA,                NA,        NA,               NA},
    {SPI1_RX,   NA,        SPI1_RX,          SPI1_TX,  SAI2_A,            SPI1_TX,   SAI2_B,           QUADSPI},
    {SPI4_RX,   SPI4_TX,   USART1_RX,        SDIO,     NA,                USART1_RX, SDIO,             USART1_TX},
    {NA,        USART6_RX, USART6_RX,        SPI4_RX,  SPI4_TX,           NA,        USART6_TX,        USART6_TX},
    {TIM1_TRIG, TIM1_CH1,  TIM1_CH2,         TIM1_CH1, TIM1_CH4_TRIG_COM, TIM1_UP,   TIM1_CH3,         NA},
    {NA,        TIM8_UP,   TIM8_CH1,         TIM8_CH2, TIM8_CH3,          NA,        NA,               TIM8_CH4_TRIG_COM},
}};
// clang-format on

} // namespace Dma

#endif // BLDC_DRIVERS_DMA_DMA_MAPPING_H
