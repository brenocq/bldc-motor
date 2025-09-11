//--------------------------------------------------
// BLDC Motor Controller
// gpio.h
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_GPIO_GPIO_H
#define BLDC_DRIVERS_GPIO_GPIO_H
#include <drivers/gpio/modes.h>
#include <drivers/gpio/ports.h>

namespace Gpio {

bool init();
bool deinit();

void write(Gpio gpio, bool high);
bool read(Gpio gpio);

static constexpr bool HIGH = true;
static constexpr bool LOW = false;

//---------- GPIO pins ----------//
constexpr Gpio SWO_PIN = PB3;    // Serial wire output
constexpr Gpio SWDIO_PIN = PA13; // Serial wire data input/output
constexpr Gpio SWCLK_PIN = PA14; // Serial wire clock

constexpr Gpio UART_TX_PIN = PC6; // UART TX
constexpr Gpio UART_RX_PIN = PC7; // UART RX

constexpr Gpio USB_DN_PIN = PA11; // USB D-
constexpr Gpio USB_DP_PIN = PA12; // USB D+

constexpr Gpio CAN_RX_PIN = PB8; // CAN RX
constexpr Gpio CAN_TX_PIN = PB9; // CAN TX

constexpr Gpio CURR_U_PIN = PC0; // Motor U phase current
constexpr Gpio CURR_V_PIN = PC1; // Motor V phase current
constexpr Gpio CURR_W_PIN = PC2; // Motor W phase current

constexpr Gpio VOLT_SRC_PIN = PA6; // Board VIN source voltage
constexpr Gpio VOLT_U_PIN = PC3;   // Motor U phase voltage
constexpr Gpio VOLT_V_PIN = PC4;   // Motor V phase voltage
constexpr Gpio VOLT_W_PIN = PC5;   // Motor W phase voltage

constexpr Gpio MOTOR_UH_PIN = PA8;   // Motor U phase high side
constexpr Gpio MOTOR_UL_PIN = PA7;   // Motor U phase low side
constexpr Gpio MOTOR_VH_PIN = PA9;   // Motor V phase high side
constexpr Gpio MOTOR_VL_PIN = PB0;   // Motor V phase low side
constexpr Gpio MOTOR_WH_PIN = PA10;  // Motor W phase high side
constexpr Gpio MOTOR_WL_PIN = PB1;   // Motor W phase low side
constexpr Gpio MOTOR_DIAG_PIN = PA4; // Motor diagnostics

constexpr Gpio ENC_CS_PIN = PA15;   // Encoder chip select
constexpr Gpio ENC_CLK_PIN = PC10;  // Encoder clock
constexpr Gpio ENC_POCI_PIN = PC11; // Encoder POCI
constexpr Gpio ENC_PICO_PIN = PC12; // Encoder PICO

constexpr Gpio IMU_CS_PIN = PA3;    // IMU chip select
constexpr Gpio IMU_CLK_PIN = PB13;  // IMU clock
constexpr Gpio IMU_POCI_PIN = PB14; // IMU POCI
constexpr Gpio IMU_PICO_PIN = PB15; // IMU PICO
constexpr Gpio IMU_INT1_PIN = PB7;  // IMU interrupt 1
constexpr Gpio IMU_INT2_PIN = PB6;  // IMU interrupt 2

constexpr Gpio LED_DI_PIN = PA0; // RGB LED ring

constexpr Gpio DEBUG0_PIN = PC13; // Debug pin 0
constexpr Gpio DEBUG1_PIN = PC14; // Debug pin 1
constexpr Gpio DEBUG2_PIN = PC15; // Debug pin 2

//---------- GPIO configs ----------//
struct GpioConfig {
    Gpio gpio;
    Mode mode;
};

// clang-format off
inline const std::array gpioList{
    GpioConfig{SWO_PIN,        Mode::SWO},
    GpioConfig{SWDIO_PIN,      Mode::SWDIO},
    GpioConfig{SWCLK_PIN,      Mode::SWCLK},
    GpioConfig{UART_TX_PIN,    Mode::USART6_TX},
    GpioConfig{UART_RX_PIN,    Mode::USART6_RX},
    //GpioConfig{USB_DM_PIN,     Mode::OTG_FS_DM},
    //GpioConfig{USB_DP_PIN,     Mode::OTG_FS_DP},
    GpioConfig{IMU_CS_PIN,     Mode::OUTPUT},
    GpioConfig{IMU_CLK_PIN ,   Mode::SPI2_SCK},
    GpioConfig{IMU_POCI_PIN,   Mode::SPI2_MISO},
    GpioConfig{IMU_PICO_PIN,   Mode::SPI2_MOSI},
    GpioConfig{LED_DI_PIN,     Mode::TIM2_CH1},
    GpioConfig{ENC_CS_PIN,     Mode::OUTPUT},
    GpioConfig{ENC_CLK_PIN,    Mode::SPI3_SCK},
    GpioConfig{ENC_POCI_PIN,   Mode::SPI3_MISO},
    GpioConfig{ENC_PICO_PIN,   Mode::SPI3_MOSI},
    GpioConfig{VOLT_SRC_PIN,   Mode::ANALOG},
    GpioConfig{VOLT_U_PIN,     Mode::ANALOG},
    GpioConfig{VOLT_V_PIN,     Mode::ANALOG},
    GpioConfig{VOLT_W_PIN,     Mode::ANALOG},
    GpioConfig{CURR_U_PIN,     Mode::ANALOG},
    GpioConfig{CURR_V_PIN,     Mode::ANALOG},
    GpioConfig{CURR_W_PIN,     Mode::ANALOG},
    GpioConfig{MOTOR_UH_PIN,   Mode::TIM1_CH1},
    GpioConfig{MOTOR_UL_PIN,   Mode::TIM1_CH1N},
    GpioConfig{MOTOR_VH_PIN,   Mode::TIM1_CH2},
    GpioConfig{MOTOR_VL_PIN,   Mode::TIM1_CH2N},
    GpioConfig{MOTOR_WH_PIN,   Mode::TIM1_CH3},
    GpioConfig{MOTOR_WL_PIN,   Mode::TIM1_CH3N},
    GpioConfig{DEBUG0_PIN,     Mode::OUTPUT},
    GpioConfig{DEBUG1_PIN,     Mode::OUTPUT},
    GpioConfig{DEBUG2_PIN,     Mode::OUTPUT},
};
// clang-format on

} // namespace Gpio

#endif // BLDC_DRIVERS_GPIO_GPIO_H
