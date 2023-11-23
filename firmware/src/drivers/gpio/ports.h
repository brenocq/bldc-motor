//--------------------------------------------------
// BLDC Motor Controller
// ports.h
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_GPIO_PORTS_H
#define BLDC_DRIVERS_GPIO_PORTS_H
#include <array>
#include <cstdint>
#include <string>

// clang-format off
#define DEFINE_GPIO_PORT(x) \
    static constexpr Gpio P##x##0  = { Port::x,  0 }; \
    static constexpr Gpio P##x##1  = { Port::x,  1 }; \
    static constexpr Gpio P##x##2  = { Port::x,  2 }; \
    static constexpr Gpio P##x##3  = { Port::x,  3 }; \
    static constexpr Gpio P##x##4  = { Port::x,  4 }; \
    static constexpr Gpio P##x##5  = { Port::x,  5 }; \
    static constexpr Gpio P##x##6  = { Port::x,  6 }; \
    static constexpr Gpio P##x##7  = { Port::x,  7 }; \
    static constexpr Gpio P##x##8  = { Port::x,  8 }; \
    static constexpr Gpio P##x##9  = { Port::x,  9 }; \
    static constexpr Gpio P##x##10 = { Port::x, 10 }; \
    static constexpr Gpio P##x##11 = { Port::x, 11 }; \
    static constexpr Gpio P##x##12 = { Port::x, 12 }; \
    static constexpr Gpio P##x##13 = { Port::x, 13 }; \
    static constexpr Gpio P##x##14 = { Port::x, 14 }; \
    static constexpr Gpio P##x##15 = { Port::x, 15 }
// clang-format on

namespace Gpio {

enum class Port : uint8_t { A = 0, B, C, D, E, F, G };
using Pin = uint8_t;

struct Gpio {
    Port port;
    Pin pin;

    bool operator==(const Gpio& o) const { return port == o.port && pin == o.pin; }
    bool operator!=(const Gpio& o) const { return !(*this == o); }
    bool operator<(const Gpio& o) const { return port < o.port || pin < o.pin; }
    uint8_t getIdx() const { return uint8_t(port) * 16 + pin; }
    std::string toString() const { return char('A' + (int)port) + std::to_string(pin); }
};

// Create definitions for PA0, PA1, ..., PH2
DEFINE_GPIO_PORT(A);
DEFINE_GPIO_PORT(B);
DEFINE_GPIO_PORT(C);
DEFINE_GPIO_PORT(D);
DEFINE_GPIO_PORT(E);
DEFINE_GPIO_PORT(F);
DEFINE_GPIO_PORT(G);

constexpr uint8_t NUM_AFS = 16; ///< Number of alternate functions
/**
 * @brief Alternate functions available for each Gpio
 */
inline const std::array<std::array<Mode, NUM_AFS>, 112> afList{{
    /* PA0 */ {NA, TIM2_CH1, TIM5_CH1, TIM8_ETR, NA, NA, NA, USART2_CTS, UART4_TX, NA, NA, NA, NA, NA, NA, NA},
    /* PA1 */ {NA, TIM2_CH2, TIM5_CH2, NA, NA, NA, NA, USART2_RTS, UART4_RX, QUADSPI_BK1_IO3, SAI2_MCLK_B, NA, NA, NA, NA, NA},
    /* PA2 */ {NA, TIM2_CH3, TIM5_CH3, TIM9_CH1, NA, NA, NA, USART2_TX, SAI2_SCK_B, NA, NA, NA, NA, NA, NA, NA},
    /* PA3 */ {NA, TIM2_CH4, TIM5_CH4, TIM9_CH2, NA, NA, SAI1_FS_A, USART2_RX, NA, NA, OTG_HS_ULPI_D0, NA, NA, NA, NA, NA},
    /* PA4 */ {NA, NA, NA, NA, NA, SPI1_NSS, SPI3_NSS, USART2_CK, NA, NA, NA, NA, OTG_HS_SOF, DCMI_HSYNC, NA, NA},
    /* PA5 */ {NA, TIM2_CH1, NA, TIM8_CH1N, NA, SPI1_SCK, NA, NA, NA, NA, OTG_HS_ULPI_CK, NA, NA, NA, NA, NA},
    /* PA6 */ {NA, TIM1_BKIN, TIM3_CH1, TIM8_BKIN, NA, SPI1_MISO, I2S2_MCK, NA, NA, TIM13_CH1, NA, NA, NA, DCMI_PIXCLK, NA, NA},
    /* PA7 */ {NA, TIM1_CH1N, TIM3_CH2, TIM8_CH1N, NA, SPI1_MOSI, NA, NA, NA, TIM14_CH1, NA, NA, FMC_SDNWE, NA, NA, NA},
    /* PA8 */ {MCO1, TIM1_CH1, NA, NA, I2C3_SCL, NA, NA, USART1_CK, NA, NA, OTG_FS_SOF, NA, NA, NA, NA, NA},
    /* PA9 */ {NA, TIM1_CH2, NA, NA, I2C3_SMBA, SPI2_SCK, SAI1_SD_B, USART1_TX, NA, NA, NA, NA, NA, DCMI_D0, NA, NA},
    /* PA10 */ {NA, TIM1_CH3, NA, NA, NA, NA, NA, USART1_RX, NA, NA, OTG_FS_ID, NA, NA, DCMI_D1, NA, NA},
    /* PA11 */ {NA, TIM1_CH4, NA, NA, NA, NA, NA, USART1_CTS, NA, CAN1_RX, OTG_FS_DM, NA, NA, NA, NA, NA},
    /* PA12 */ {NA, TIM1_ETR, NA, NA, NA, NA, NA, USART1_RTS, SAI2_FS_B, CAN1_TX, OTG_FS_DP, NA, NA, NA, NA, NA},
    /* PA13 */ {SWDIO, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    /* PA14 */ {SWCLK, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    /* PA15 */ {JTDI, TIM2_CH1, NA, NA, HDMI_CEC, SPI1_NSS, SPI3_NSS, NA, UART4_RTS, NA, NA, NA, NA, NA, NA, NA},

    /* PB0 */ {NA, TIM1_CH2N, TIM3_CH3, TIM8_CH2N, NA, NA, SPI3_MOSI, UART4_CTS, NA, OTG_HS_ULPI_D1, SDIO_D1, NA, NA, NA},
    /* PB1 */ {NA, TIM1_CH3N, TIM3_CH4, TIM8_CH3N, NA, NA, NA, NA, NA, OTG_HS_ULPI_D2, SDIO_D2, NA, NA, NA},
    /* PB2 */ {NA, TIM2_CH4, NA, NA, NA, SAI1_SD_A, SPI3_MOSI, NA, QUADSPI_CLK, OTG_HS_ULPI_D4, SDIO_CK, NA, NA, NA},
    /* PB3 */ {SWO, TIM2_CH2, NA, NA, I2C2_SDA, SPI1_SCK, SPI3_SCK, NA, NA, NA, NA, NA, NA},
    /* PB4 */ {NJTRST, TIM3_CH1, NA, I2C3_SDA, SPI1_MISO, SPI3_MISO, SPI2_NSS, NA, NA, NA, NA, NA, NA},
    /* PB5 */ {NA, NA, TIM3_CH2, NA, I2C1_SMBA, SPI1_MOSI, SPI3_MOSI, NA, NA, CAN2_RX, OTG_HS_ULPI_D7, FMC_SDCKE1, DCMI_D10, NA},
    /* PB6 */ {NA, NA, TIM4_CH1, HDMI_CEC, I2C1_SCL, NA, NA, USART1_TX, NA, CAN2_TX, QUADSPI_BK1_NCS, FMC_SDNE1, DCMI_D5, NA},
    /* PB7 */ {NA, NA, TIM4_CH2, NA, I2C1_SDA, NA, NA, USART1_RX, SPDIF_RX0, NA, NA, NA, FMC_NL, DCMI_VSYNC, NA},
    /* PB8 */ {NA, TIM2_CH1, TIM4_CH3, TIM10_CH1, I2C1_SCL, NA, NA, NA, NA, CAN1_RX, NA, SDIO_D4, DCMI_D6, NA},
    /* PB9 */ {NA, TIM2_CH2, TIM4_CH4, TIM11_CH1, I2C1_SDA, SPI2_NSS, SAI1_FS_B, NA, NA, CAN1_TX, NA, SDIO_D5, DCMI_D7, NA},
    /* PB10 */ {NA, TIM2_CH3, NA, NA, I2C2_SCL, SPI2_SCK, SAI1_SCK_A, USART3_TX, NA, NA, OTG_HS_ULPI_D3, NA, NA, NA},
    /* PB11 */ {NA, TIM2_CH4, NA, NA, I2C2_SDA, NA, NA, USART3_RX, SAI2_SD_A, NA, NA, NA, NA, NA},
    /* PB12 */ {NA, TIM1_BKIN, NA, NA, I2C2_SMBA, SPI2_NSS, SAI1_SCK_B, USART3_CK, NA, CAN2_RX, OTG_HS_ULPI_D5, OTG_HS_ID, NA, NA, NA},
    /* PB13 */ {NA, TIM1_CH1N, NA, NA, NA, SPI2_SCK, NA, USART3_CTS, NA, CAN2_TX, OTG_HS_ULPI_D6, NA, NA, NA},
    /* PB14 */ {NA, TIM1_CH2N, TIM8_CH2N, NA, SPI2_MISO, NA, USART3_RTS, TIM12_CH1, NA, NA, OTG_HS_DM, NA, NA, NA},
    /* PB15 */ {RTC_REFIN, TIM1_CH3N, NA, TIM8_CH3N, SPI2_MOSI, NA, NA, NA, TIM12_CH2, NA, NA, OTG_HS_DP, NA, NA, NA},

    /* PC0 */ {NA, NA, NA, NA, NA, NA, SAI1_MCLK_B, NA, NA, OTG_HS_ULPI_STP, FMC_SDNWE, NA, NA, NA},
    /* PC1 */ {NA, NA, NA, NA, NA, SPI3_MOSI, SAI1_SD_A, SPI2_MOSI, NA, NA, NA, NA, NA, NA},
    /* PC2 */ {NA, NA, NA, NA, NA, SPI2_MISO, NA, NA, NA, NA, OTG_HS_ULPI_DIR, FMC_SDNE0, NA, NA, NA},
    /* PC3 */ {NA, NA, NA, NA, NA, SPI2_MOSI, NA, NA, NA, I2S2_SD, OTG_HS_ULPI_NXT, FMC_SDCKE0, NA, NA, NA},
    /* PC4 */ {NA, NA, NA, NA, NA, I2S1_MCK, NA, NA, SPDIF_RX2, NA, NA, FMC_SDNE0, NA, NA, NA},
    /* PC5 */ {NA, NA, NA, NA, NA, NA, NA, USART3_RX, SPDIF_RX3, NA, NA, FMC_SDCKE0, NA, NA, NA},
    /* PC6 */ {NA, NA, TIM3_CH1, TIM8_CH1, FMPI2C1_SCL, I2S2_MCK, NA, NA, USART6_TX, NA, NA, NA, SDIO_D6, DCMI_D0, NA},
    /* PC7 */ {NA, NA, TIM3_CH2, TIM8_CH2, FMPI2C1_SDA, SPI2_SCK, I2S2_CK, I2S3_MCK, SPDIF_RX1, USART6_RX, NA, NA, NA, SDIO_D7, DCMI_D1, NA},
    /* PC8 */ {TRACE_D0, NA, TIM3_CH3, TIM8_CH3, NA, NA, NA, UART5_RTS, USART6_CK, NA, NA, NA, SDIO_D0, DCMI_D2, NA},
    /* PC9 */ {MCO2, NA, TIM3_CH4, TIM8_CH4, I2C3_SDA, I2S_CKIN, NA, UART5_CTS, NA, QUADSPI_BK1_IO0, NA, NA, SDIO_D1, DCMI_D3, NA},
    /* PC10 */ {NA, NA, NA, NA, NA, NA, SPI3_SCK, USART3_TX, UART4_TX, QUADSPI_BK1_IO1, NA, NA, SDIO_D2, DCMI_D8, NA},
    /* PC11 */ {NA, NA, NA, NA, NA, NA, SPI3_MISO, USART3_RX, UART4_RX, QUADSPI_BK2_NCS, NA, NA, SDIO_D3, DCMI_D4, NA},
    /* PC12 */ {NA, NA, NA, NA, I2C2_SDA, NA, SPI3_MOSI, USART3_CK, UART5_TX, NA, NA, NA, SDIO_CK, DCMI_D9, NA},
    /* PC13 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    /* PC14 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    /* PC15 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},

    /* PD0 */ {NA, NA, NA, NA, NA, SPI4_MISO, SPI3_MOSI, NA, NA, CAN1_RX, NA, NA, FMC_D2, NA, NA, NA},
    /* PD1 */ {NA, NA, NA, NA, NA, NA, NA, SPI2_NSS, NA, CAN1_TX, NA, NA, FMC_D3, NA, NA, NA},
    /* PD2 */ {NA, TIM3_ETR, NA, NA, NA, NA, NA, UART5_RX, NA, NA, NA, SDIO_CMD, DCMI_D11, NA, NA},
    /* PD3 */ {TRACE_D1, NA, NA, NA, SPI2_SCK, NA, USART2_CTS, NA, QUADSPI_CLK, NA, NA, FMC_CLK, DCMI_D5, NA, NA},
    /* PD4 */ {NA, NA, NA, NA, NA, NA, NA, USART2_RTS, NA, NA, NA, NA, FMC_NOE, NA, NA, NA},
    /* PD5 */ {NA, NA, NA, NA, NA, NA, NA, USART2_TX, NA, NA, NA, NA, FMC_NWE, NA, NA, NA},
    /* PD6 */ {NA, SPI3_MOSI, SAI1_SD_A, USART2_RX, NA, NA, NA, FMC_NWAIT, DCMI_D10, NA, NA},
    /* PD7 */ {NA, NA, NA, NA, NA, NA, NA, USART2_CK, SPDIF_RX0, NA, NA, NA, FMC_NE1, NA, NA, NA},
    /* PD8 */ {NA, NA, NA, NA, NA, NA, NA, USART3_TX, SPDIF_RX1, NA, NA, NA, FMC_D13, NA, NA, NA},
    /* PD9 */ {NA, NA, NA, NA, NA, NA, NA, USART3_RX, NA, NA, NA, NA, FMC_D14, NA, NA, NA},
    /* PD10 */ {NA, NA, NA, NA, NA, NA, NA, USART3_CK, NA, NA, NA, NA, FMC_D15, NA, NA, NA},
    /* PD11 */ {NA, NA, NA, FMPI2C1_SMBA, NA, NA, USART3_CTS, NA, QUADSPI_BK1_IO0, SAI2_SD_A, NA, FMC_A16, NA, NA, NA},
    /* PD12 */ {NA, NA, TIM4_CH1, NA, FMPI2C1_SCL, NA, NA, USART3_RTS, NA, QUADSPI_BK1_IO1, SAI2_FS_A, NA, FMC_A17, NA, NA, NA},
    /* PD13 */ {NA, NA, TIM4_CH2, NA, FMPI2C1_SDA, NA, NA, NA, NA, QUADSPI_BK1_IO3, SAI2_SCK_A, NA, FMC_A18, NA, NA, NA},
    /* PD14 */ {NA, NA, TIM4_CH3, NA, FMPI2C1_SCL, NA, NA, SAI2_SCK_A, NA, NA, NA, FMC_D0, NA, NA, NA},
    /* PD15 */ {NA, NA, TIM4_CH4, NA, FMPI2C1_SDA, NA, NA, NA, NA, NA, NA, NA, FMC_D1, NA, NA, NA},

    /* PE0 */ {NA, NA, TIM4_ETR, NA, NA, NA, NA, NA, NA, NA, SAI2_MCLK_A, NA, FMC_NBL0, DCMI_D2, NA, NA},
    /* PE1 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_NBL1, DCMI_D3, NA, NA},
    /* PE2 */ {TRACE_CLK, NA, NA, NA, NA, SPI4_SCK, SAI1_MCLK_A, NA, NA, QUADSPI_BK1_IO2, NA, NA, FMC_A23, NA, NA, NA},
    /* PE3 */ {TRACE_D0, NA, NA, NA, NA, NA, SAI1_SD_B, NA, NA, NA, NA, NA, FMC_A19, NA, NA, NA},
    /* PE4 */ {TRACE_D1, NA, NA, NA, SPI4_NSS, SAI1_FS_A, NA, NA, NA, NA, NA, FMC_A20, DCMI_D4, NA, NA},
    /* PE5 */ {TRACE_D2, NA, NA, TIM9_CH1, NA, SPI4_MISO, SAI1_SCK_A, NA, NA, NA, NA, NA, FMC_A21, DCMI_D6, NA, NA},
    /* PE6 */ {TRACE_D3, NA, NA, TIM9_CH2, NA, SPI4_MOSI, SAI1_SD_A, NA, NA, NA, NA, NA, FMC_A22, DCMI_D7, NA, NA},
    /* PE7 */ {NA, TIM1_ETR, NA, NA, NA, NA, NA, NA, UART5_RX, NA, QUADSPI_BK2_IO0, NA, FMC_D4, NA, NA, NA},
    /* PE8 */ {NA, TIM1_CH1N, NA, NA, NA, NA, NA, NA, UART5_TX, NA, QUADSPI_BK2_IO1, NA, FMC_D5, NA, NA, NA},
    /* PE9 */ {NA, TIM1_CH1, NA, NA, NA, NA, NA, NA, NA, NA, QUADSPI_BK2_IO2, NA, FMC_D6, NA, NA, NA},
    /* PE10 */ {NA, TIM1_CH2N, NA, NA, NA, NA, NA, NA, NA, NA, QUADSPI_BK2_IO3, NA, FMC_D7, NA, NA, NA},
    /* PE11 */ {NA, TIM1_CH2, NA, NA, NA, SPI4_NSS, NA, NA, NA, NA, SAI2_SD_B, NA, FMC_D8, NA, NA, NA},
    /* PE12 */ {NA, TIM1_CH3N, NA, NA, NA, SPI4_SCK, NA, NA, NA, NA, SAI2_SCK_B, NA, FMC_D9, NA, NA, NA},
    /* PE13 */ {NA, TIM1_CH3, NA, NA, NA, SPI4_MISO, NA, NA, NA, NA, SAI2_FS_B, NA, FMC_D10, NA, NA, NA},
    /* PE14 */ {NA, TIM1_CH4, NA, NA, NA, SPI4_MOSI, NA, NA, NA, NA, SAI2_MCLK_B, NA, FMC_D11, NA, NA, NA},
    /* PE15 */ {NA, TIM1_BKIN, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_D12, NA, NA, NA},

    /* PF0 */ {NA, NA, NA, NA, I2C2_SDA, NA, NA, NA, NA, NA, NA, NA, FMC_A0, NA, NA, NA},
    /* PF1 */ {NA, NA, NA, NA, I2C2_SCL, NA, NA, NA, NA, NA, NA, NA, FMC_A1, NA, NA, NA},
    /* PF2 */ {NA, NA, NA, NA, I2C2_SMBA, NA, NA, NA, NA, NA, NA, NA, FMC_A2, NA, NA, NA},
    /* PF3 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A3, NA, NA, NA},
    /* PF4 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A4, NA, NA, NA},
    /* PF5 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A5, NA, NA, NA},
    /* PF6 */ {NA, NA, NA, TIM10_CH1, NA, NA, SAI1_SD_B, NA, NA, QUADSPI_BK1_IO3, NA, NA, NA, NA, NA, NA},
    /* PF7 */ {NA, NA, NA, TIM11_CH1, NA, NA, SAI1_MCLK_B, NA, NA, QUADSPI_BK1_IO2, NA, NA, NA, NA, NA, NA},
    /* PF8 */ {NA, NA, NA, NA, NA, NA, SAI1_SCK_B, NA, NA, TIM13_CH1, QUADSPI_BK1_IO0, NA, NA, NA, NA, NA},
    /* PF9 */ {NA, NA, NA, NA, NA, NA, SAI1_FS_B, NA, NA, TIM14_CH1, QUADSPI_BK1_IO1, NA, NA, NA, NA, NA},
    /* PF10 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, DCMI_D11, NA, NA},
    /* PF11 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, SAI2_SD_B, NA, FMC_SDNRAS, DCMI_D12, NA, NA},
    /* PF12 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A6, NA, NA, NA},
    /* PF13 */ {NA, NA, NA, NA, FMPI2C1_SMBA, NA, NA, NA, NA, NA, NA, NA, FMC_A7, NA, NA, NA},
    /* PF14 */ {NA, NA, NA, NA, FMPI2C1_SCL, NA, NA, NA, NA, NA, NA, NA, FMC_A8, NA, NA, NA},
    /* PF15 */ {NA, NA, NA, NA, FMPI2C1_SDA, NA, NA, NA, NA, NA, NA, NA, FMC_A9, NA, NA, NA},

    /* PG0 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A10, NA, NA, NA},
    /* PG1 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A11, NA, NA, NA},
    /* PG2 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A12, NA, NA, NA},
    /* PG3 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A13, NA, NA, NA},
    /* PG4 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A14, NA, NA, NA},
    /* PG5 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, FMC_A15, NA, NA, NA},
    /* PG6 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, QUADSPI_BK1_NCS, NA, NA, DCMI_D12, NA, NA},
    /* PG7 */ {NA, NA, NA, NA, NA, NA, NA, NA, USART6_CK, NA, NA, NA, FMC_INT, DCMI_D13, NA, NA},
    /* PG8 */ {NA, NA, NA, NA, NA, NA, NA, SPDIFRX_IN2, USART6_RTS, NA, NA, NA, FMC_SDCLK, NA, NA, NA},
    /* PG9 */ {NA, NA, NA, NA, NA, NA, NA, SPDIFRX_IN3, USART6_RX, QUADSPI_BK2_IO2, SAI2_FS_B, NA, FMC_NE2, DCMI_VSYNC, NA, NA},
    /* PG10 */ {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, SAI2_SD_B, NA, FMC_NE3, DCMI_D2, NA, NA},
    /* PG11 */ {NA, NA, NA, NA, NA, NA, SPI4_SCK, SPDIFRX_IN0, NA, NA, NA, NA, NA, DCMI_D3, NA, NA},
    /* PG12 */ {NA, NA, NA, NA, NA, NA, SPI4_MISO, SPDIFRX_IN1, USART6_RTS, NA, NA, NA, FMC_NE4, NA, NA, NA},
    /* PG13 */ {TRACE_D2, NA, NA, NA, NA, NA, SPI4_MOSI, NA, USART6_CTS, NA, NA, NA, FMC_A24, NA, NA, NA},
    /* PG14 */ {TRACE_D3, NA, NA, NA, NA, NA, SPI4_NSS, NA, USART6_TX, QUADSPI_BK2_IO3, NA, NA, FMC_A25, NA, NA, NA},
    /* PG15 */ {NA, NA, NA, NA, NA, NA, NA, NA, USART6_CTS, NA, NA, NA, FMC_SDNCAS, DCMI_D13, NA, NA},
}};

} // namespace Gpio

#endif // BLDC_DRIVERS_GPIO_PORTS_H
