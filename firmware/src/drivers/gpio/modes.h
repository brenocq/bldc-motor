//--------------------------------------------------
// BLDC Motor Controller
// modes.h
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_GPIO_MODES_H
#define BLDC_DRIVERS_GPIO_MODES_H

namespace Gpio {

enum Mode {
    NA = 0, ///< No mode configured (not applicable)
    INPUT,  ///< Input
    OUTPUT, ///< Output

    //----- I2C -----//
    I2C1_SDA,     ///< I2C1 SDA
    I2C1_SCL,     ///< I2C1 SCL
    I2C1_SMBA,    ///< I2C1 SMBA (system management bus address)
    I2C2_SDA,     ///< I2C2 SDA
    I2C2_SCL,     ///< I2C2 SCL
    I2C2_SMBA,    ///< I2C2 SMBA (system management bus address)
    I2C3_SDA,     ///< I2C3 SDA
    I2C3_SCL,     ///< I2C3 SCL
    I2C3_SMBA,    ///< I2C3 SMBA (system management bus address)
    FMPI2C1_SDA,  ///< I2C1 SDA (fast-mode plus serial data)
    FMPI2C1_SCL,  ///< I2C1 SCL (fast-mode plus serial clock)
    FMPI2C1_SMBA, ///< I2C1 SMBA (fast-mode plus system management bus address)

    //----- SPI -----//
    SPI1_NSS,  ///< SPI1 negative slave select
    SPI1_SCK,  ///< SPI1 clock signal
    SPI1_MISO, ///< SPI1 master in slave out
    SPI1_MOSI, ///< SPI1 master out slave in
    SPI2_NSS,  ///< SPI2 negative slave select
    SPI2_SCK,  ///< SPI2 clock signal
    SPI2_MISO, ///< SPI2 master in slave out
    SPI2_MOSI, ///< SPI2 master out slave in
    SPI3_NSS,  ///< SPI3 negative slave select
    SPI3_SCK,  ///< SPI3 clock signal
    SPI3_MISO, ///< SPI3 master in slave out
    SPI3_MOSI, ///< SPI3 master out slave in
    SPI4_NSS,  ///< SPI4 negative slave select
    SPI4_SCK,  ///< SPI4 clock signal
    SPI4_MISO, ///< SPI4 master in slave out
    SPI4_MOSI, ///< SPI4 master out slave in

    //----- UART/USART -----//
    USART1_TX,  ///< USART1 transmit
    USART1_RX,  ///< USART1 receive
    USART1_CTS, ///< USART1 clear to send
    USART1_RTS, ///< USART1 request to send
    USART1_CK,  ///< USART1 clock
    USART2_TX,  ///< USART2 transmit
    USART2_RX,  ///< USART2 receive
    USART2_CTS, ///< USART2 clear to send
    USART2_RTS, ///< USART2 request to send
    USART2_CK,  ///< USART2 clock
    USART3_TX,  ///< USART2 transmit
    USART3_RX,  ///< USART2 receive
    USART3_CTS, ///< USART2 clear to send
    USART3_RTS, ///< USART2 request to send
    USART3_CK,  ///< USART2 clock
    UART4_TX,   ///< UART4 transmit
    UART4_RX,   ///< UART4 receive
    UART4_CTS,  ///< UART4 clear to send
    UART4_RTS,  ///< UART4 request to send
    UART5_TX,   ///< UART5 transmit
    UART5_RX,   ///< UART5 receive
    UART5_CTS,  ///< UART5 clear to send
    UART5_RTS,  ///< UART5 request to send
    USART6_TX,  ///< USART6 transmit
    USART6_RX,  ///< USART6 receive
    USART6_CTS, ///< USART6 clear to send
    USART6_RTS, ///< USART6 request to send
    USART6_CK,  ///< USART6 clock

    //----- Timer -----//
    TIM1_CH1,  ///< Timer 1 channel 1
    TIM1_CH1N, ///< Timer 1 channel 1 negative
    TIM1_CH2,  ///< Timer 1 channel 2
    TIM1_CH2N, ///< Timer 1 channel 2 negative
    TIM1_CH3,  ///< Timer 1 channel 3
    TIM1_CH3N, ///< Timer 1 channel 3 negative
    TIM1_CH4,  ///< Timer 1 channel 4
    TIM1_ETR,  ///< Timer 1 external trigger input
    TIM1_BKIN, ///< Timer 1 brake fault input
    TIM2_CH1,  ///< Timer 2 channel 1
    TIM2_CH2,  ///< Timer 2 channel 2
    TIM2_CH3,  ///< Timer 2 channel 3
    TIM2_CH4,  ///< Timer 2 channel 4
    TIM3_CH1,  ///< Timer 3 channel 1
    TIM3_CH2,  ///< Timer 3 channel 2
    TIM3_CH3,  ///< Timer 3 channel 3
    TIM3_CH4,  ///< Timer 3 channel 4
    TIM3_ETR,  ///< Timer 3 external trigger input
    TIM4_CH1,  ///< Timer 4 channel 1
    TIM4_CH2,  ///< Timer 4 channel 2
    TIM4_CH3,  ///< Timer 4 channel 3
    TIM4_CH4,  ///< Timer 4 channel 4
    TIM4_ETR,  ///< Timer 4 external trigger input
    TIM5_CH1,  ///< Timer 5 channel 1
    TIM5_CH2,  ///< Timer 5 channel 2
    TIM5_CH3,  ///< Timer 5 channel 3
    TIM5_CH4,  ///< Timer 5 channel 4
    TIM8_CH1,  ///< Timer 8 channel 1
    TIM8_CH1N, ///< Timer 8 channel 1 negative
    TIM8_CH2,  ///< Timer 8 channel 2
    TIM8_CH2N, ///< Timer 8 channel 2 negative
    TIM8_CH3,  ///< Timer 8 channel 3
    TIM8_CH3N, ///< Timer 8 channel 3 negative
    TIM8_CH4,  ///< Timer 8 channel 4
    TIM8_ETR,  ///< Timer 8 external trigger input
    TIM8_BKIN, ///< Timer 8 brake fault input
    TIM9_CH1,  ///< Timer 9 channel 1
    TIM9_CH2,  ///< Timer 9 channel 2
    TIM10_CH1, ///< Timer 10 channel 1
    TIM11_CH1, ///< Timer 11 channel 1
    TIM12_CH1, ///< Timer 12 channel 1
    TIM12_CH2, ///< Timer 12 channel 2
    TIM13_CH1, ///< Timer 13 channel 1
    TIM14_CH1, ///< Timer 14 channel 1

    //----- DCMI -----//
    DCMI_VSYNC,  ///< DCMI vertical sync
    DCMI_HSYNC,  ///< DCMI horizontal sync
    DCMI_PIXCLK, ///< DCMI pixel clock
    DCMI_D0,     ///< DCMI data bit 0
    DCMI_D1,     ///< DCMI data bit 1
    DCMI_D2,     ///< DCMI data bit 2
    DCMI_D3,     ///< DCMI data bit 3
    DCMI_D4,     ///< DCMI data bit 4
    DCMI_D5,     ///< DCMI data bit 5
    DCMI_D6,     ///< DCMI data bit 6
    DCMI_D7,     ///< DCMI data bit 7
    DCMI_D8,     ///< DCMI data bit 8
    DCMI_D9,     ///< DCMI data bit 9
    DCMI_D10,    ///< DCMI data bit 10
    DCMI_D11,    ///< DCMI data bit 11
    DCMI_D12,    ///< DCMI data bit 12
    DCMI_D13,    ///< DCMI data bit 13

    //----- USB OTG -----//
    OTG_FS_DM,       ///< USB OTG FS data- signal
    OTG_FS_DP,       ///< USB OTG FS data+ signal
    OTG_FS_ID,       ///< USB OTG FS ID pin
    OTG_FS_SOF,      ///< USB OTG FS start of frame
    OTG_HS_DM,       ///< USB OTG HS data- signal
    OTG_HS_DP,       ///< USB OTG HS data+ signal
    OTG_HS_ID,       ///< USB OTG HS ID pin
    OTG_HS_SOF,      ///< USB OTG HS start of frame
    OTG_HS_ULPI_D0,  ///< USB OTG HS ULPI data 0
    OTG_HS_ULPI_D1,  ///< USB OTG HS ULPI data 1
    OTG_HS_ULPI_D2,  ///< USB OTG HS ULPI data 2
    OTG_HS_ULPI_D3,  ///< USB OTG HS ULPI data 3
    OTG_HS_ULPI_D4,  ///< USB OTG HS ULPI data 4
    OTG_HS_ULPI_D5,  ///< USB OTG HS ULPI data 5
    OTG_HS_ULPI_D6,  ///< USB OTG HS ULPI data 6
    OTG_HS_ULPI_D7,  ///< USB OTG HS ULPI data 7
    OTG_HS_ULPI_DIR, ///< USB OTG HS ULPI direction control
    OTG_HS_ULPI_CK,  ///< USB OTG HS ULPI clock
    OTG_HS_ULPI_STP, ///< USB OTG HS ULPI stop signal
    OTG_HS_ULPI_NXT, ///< USB OTG HS ULPI next signal

    //----- CAN -----//
    CAN1_RX, ///< CAN receive
    CAN1_TX, ///< CAN transmit
    CAN2_RX, ///< CAN receive
    CAN2_TX, ///< CAN transmit

    //----- I2S -----//
    I2S_CKIN, ///< I2S clock input
    I2S1_MCK, ///< I2S1 master clock
    I2S1_CK,  ///< I2S1 clock
    I2S1_SD,  ///< I2S1 data
    I2S1_WS,  ///< I2S1 word select
    I2S2_MCK, ///< I2S2 master clock
    I2S2_CK,  ///< I2S2 clock
    I2S2_SD,  ///< I2S2 data
    I2S2_WS,  ///< I2S2 word select
    I2S3_MCK, ///< I2S3 master clock
    I2S3_CK,  ///< I2S3 clock
    I2S3_SD,  ///< I2S3 data
    I2S3_WS,  ///< I2S3 word select

    //----- SAI -----//
    SAI1_FS_A,   // SAI1 frame sync A
    SAI1_FS_B,   // SAI1 frame sync B
    SAI1_SD_A,   // SAI1 serial data A
    SAI1_SD_B,   // SAI1 serial data B
    SAI1_SCK_A,  // SAI1 serial clock A
    SAI1_SCK_B,  // SAI1 serial clock B
    SAI1_MCLK_A, // SAI1 master clock A
    SAI1_MCLK_B, // SAI1 master clock B
    SAI2_FS_A,   // SAI2 frame sync A
    SAI2_FS_B,   // SAI2 frame sync B
    SAI2_SD_A,   // SAI2 serial data A
    SAI2_SD_B,   // SAI2 serial data B
    SAI2_SCK_A,  // SAI2 serial clock A
    SAI2_SCK_B,  // SAI2 serial clock B
    SAI2_MCLK_A, // SAI2 master clock A
    SAI2_MCLK_B, // SAI2 master clock B

    //----- QUADSPI -----//
    QUADSPI_CLK,     ///< QUADSPI clock
    QUADSPI_BK1_NCS, ///< QUADSPI bank 1 negative chip select
    QUADSPI_BK1_IO0, ///< QUADSPI bank 1 io 0
    QUADSPI_BK1_IO1, ///< QUADSPI bank 1 io 1
    QUADSPI_BK1_IO2, ///< QUADSPI bank 1 io 2
    QUADSPI_BK1_IO3, ///< QUADSPI bank 1 io 3
    QUADSPI_BK2_NCS, ///< QUADSPI bank 2 negative chip select
    QUADSPI_BK2_IO0, ///< QUADSPI bank 2 io 0
    QUADSPI_BK2_IO1, ///< QUADSPI bank 2 io 1
    QUADSPI_BK2_IO2, ///< QUADSPI bank 2 io 2
    QUADSPI_BK2_IO3, ///< QUADSPI bank 2 io 3

    //----- SDIO -----//
    SDIO_CK,  ///< SDIO clock line
    SDIO_CD,  ///< SDIO card detect line
    SDIO_WP,  ///< SDIO write protect line
    SDIO_CMD, ///< SDIO command line
    SDIO_D0,  ///< SDIO data line 0
    SDIO_D1,  ///< SDIO data line 1
    SDIO_D2,  ///< SDIO data line 2
    SDIO_D3,  ///< SDIO data line 3
    SDIO_D4,  ///< SDIO data line 4
    SDIO_D5,  ///< SDIO data line 5
    SDIO_D6,  ///< SDIO data line 6
    SDIO_D7,  ///< SDIO data line 7

    //----- FMC -----//
    FMC_SDNWE,
    FMC_SDNCAS,
    FMC_SDNRAS,
    FMC_NBL0,
    FMC_NBL1,
    FMC_SDCLK,
    FMC_SDCKE1,
    FMC_SDNE1,
    FMC_CLK,
    FMC_NOE,
    FMC_NWE,
    FMC_NE1,
    FMC_NE2,
    FMC_NE3,
    FMC_NE4,
    FMC_NWAIT,
    FMC_NL,
    FMC_INT,
    FMC_SDNE0,
    FMC_SDCKE0,
    FMC_D0,
    FMC_D1,
    FMC_D2,
    FMC_D3,
    FMC_D4,
    FMC_D5,
    FMC_D6,
    FMC_D7,
    FMC_D8,
    FMC_D9,
    FMC_D10,
    FMC_D11,
    FMC_D12,
    FMC_D13,
    FMC_D14,
    FMC_D15,
    FMC_A0,
    FMC_A1,
    FMC_A2,
    FMC_A3,
    FMC_A4,
    FMC_A5,
    FMC_A6,
    FMC_A7,
    FMC_A8,
    FMC_A9,
    FMC_A10,
    FMC_A11,
    FMC_A12,
    FMC_A13,
    FMC_A14,
    FMC_A15,
    FMC_A16,
    FMC_A17,
    FMC_A18,
    FMC_A19,
    FMC_A20,
    FMC_A21,
    FMC_A22,
    FMC_A23,
    FMC_A24,
    FMC_A25,

    //----- SPDIF -----//
    SPDIFRX_IN0, ///< SPDIF receiver input 0
    SPDIFRX_IN1, ///< SPDIF receiver input 1
    SPDIFRX_IN2, ///< SPDIF receiver input 2
    SPDIFRX_IN3, ///< SPDIF receiver input 3
    SPDIF_RX0,   ///< SPDIF receiver input 0
    SPDIF_RX1,   ///< SPDIF receiver input 1
    SPDIF_RX2,   ///< SPDIF receiver input 2
    SPDIF_RX3,   ///< SPDIF receiver input 3
    SPDIF_IN0,   ///< SPDIF input 0
    SPDIF_IN1,   ///< SPDIF input 1
    SPDIF_IN2,   ///< SPDIF input 2
    SPDIF_IN3,   ///< SPDIF input 3

    //----- HDMI -----//
    HDMI_CEC,

    //----- JTAG -----//
    SWO,       ///< Serial wire output
    JTDI,      ///< JTAG test data in
    NJTRST,    ///< Non-JTAG reset
    SWDIO,     ///< Serial wire debug input/output
    SWCLK,     ///< Serial wire clock
    TRACE_CLK, ///<  Trace clock
    TRACE_D0,  ///<  Trace data 0
    TRACE_D1,  ///<  Trace data 1
    TRACE_D2,  ///<  Trace data 2
    TRACE_D3,  ///<  Trace data 3

    //----- Clock -----//
    MCO1,      ///< Master clock output 1
    MCO2,      ///< Master clock output 2
    RTC_REFIN, ///< Real-time clock reference input
};

}

#endif // BLDC_DRIVERS_GPIO_MODES_H
