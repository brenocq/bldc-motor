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
constexpr Gpio SWO_PIN = PB3;
constexpr Gpio SWDIO_PIN = PA13;
constexpr Gpio SWCLK_PIN = PA14;

constexpr Gpio LED_DI_PIN = PB8;

// constexpr Gpio USB_DM_PIN = PA11;
// constexpr Gpio USB_DP_PIN = PA12;

constexpr Gpio IMU_CS_PIN = PA3;    // IMU chip select
constexpr Gpio IMU_CLK_PIN = PA9;   // IMU clock
constexpr Gpio IMU_POCI_PIN = PB14; // IMU POCI
constexpr Gpio IMU_PICO_PIN = PB15; // IMU PICO
constexpr Gpio IMU_INT1_PIN = PB7;  // IMU interrupt 1
constexpr Gpio IMU_INT2_PIN = PB6;  // IMU interrupt 2

constexpr Gpio ENC_CSN_PIN = PA15; // Encoder chip select
constexpr Gpio ENC_CLK_PIN = PC10; // Encoder clock
constexpr Gpio ENC_DO_PIN = PC1;   // Encoder data output

constexpr Gpio VOLT_PIN = PA0; // Motor input voltage

constexpr Gpio CURR_SCL_PIN = PA8; // Current SCL
constexpr Gpio CURR_SDA_PIN = PB4; // Current SDA

constexpr Gpio MOTOR_UH_PIN = PA1;   // Motor U phase high side
constexpr Gpio MOTOR_UL_PIN = PA2;   // Motor U phase low side
constexpr Gpio MOTOR_VH_PIN = PA6;   // Motor V phase high side
constexpr Gpio MOTOR_VL_PIN = PA7;   // Motor V phase low side
constexpr Gpio MOTOR_WH_PIN = PA5;   // Motor W phase high side
constexpr Gpio MOTOR_WL_PIN = PA10;  // Motor W phase low side
constexpr Gpio MOTOR_DIAG_PIN = PA4; // Motor diagnostics

constexpr Gpio UART_TX_PIN = PC6; // UART TX
constexpr Gpio UART_RX_PIN = PC7; // UART RX

constexpr Gpio CAN_RX_PIN = PA11; // CAN RX
constexpr Gpio CAN_TX_PIN = PA12; // CAN TX

//---------- GPIO configs ----------//
struct GpioConfig {
    Gpio gpio;
    Mode mode;
};

// clang-format off
inline const std::array gpioList{
    GpioConfig{SWO_PIN,     Mode::SWO},
    GpioConfig{SWDIO_PIN,   Mode::SWDIO},
    GpioConfig{SWCLK_PIN,   Mode::SWCLK},
    GpioConfig{UART_TX_PIN, Mode::USART6_TX},
    GpioConfig{UART_RX_PIN, Mode::USART6_RX},
    // GpioConfig{ENC_CSN_PIN, Mode::OUTPUT},
    // GpioConfig{ENC_CLK_PIN, Mode::OUTPUT},
    // GpioConfig{ENC_DO_PIN,  Mode::INPUT},
    GpioConfig{VOLT_PIN,    Mode::ANALOG},
    GpioConfig{CURR_SDA_PIN, Mode::I2C3_SDA},
    GpioConfig{CURR_SCL_PIN, Mode::I2C3_SCL},
    // GpioConfig{MOTOR_UH_PIN, Mode::OUTPUT},
    // GpioConfig{MOTOR_UL_PIN, Mode::OUTPUT},
    // GpioConfig{MOTOR_VH_PIN, Mode::OUTPUT},
    // GpioConfig{MOTOR_VL_PIN, Mode::OUTPUT},
    // GpioConfig{MOTOR_WH_PIN, Mode::OUTPUT},
    // GpioConfig{MOTOR_WL_PIN, Mode::OUTPUT},
    // GpioConfig{USB_DM_PIN,  Mode::OTG_FS_DM},
    // GpioConfig{USB_DP_PIN,  Mode::OTG_FS_DP},
};
// clang-format on

} // namespace Gpio

#endif // BLDC_DRIVERS_GPIO_GPIO_H
