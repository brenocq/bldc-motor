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

constexpr Gpio LED_PIN = PA5;

constexpr Gpio USB_DM_PIN = PA11; //< XXX Not connected in the NUCLEO board
constexpr Gpio USB_DP_PIN = PA12; //< XXX Not connected in the NUCLEO board

constexpr Gpio UART_TX_PIN = PA2;
constexpr Gpio UART_RX_PIN = PA3;

constexpr Gpio ENC_CSN_PIN = PC8; // Encoder chip select
constexpr Gpio ENC_CLK_PIN = PC6; // Encoder clock
constexpr Gpio ENC_DO_PIN = PC5;  // Encoder data output

constexpr Gpio VOLT_PIN = PA0; // Motor input voltage

constexpr Gpio MOTOR_UH_PIN = PB10; // Motor U phase high side
constexpr Gpio MOTOR_UL_PIN = PB15; // Motor U phase low side
constexpr Gpio MOTOR_VH_PIN = PB4;  // Motor V phase high side
constexpr Gpio MOTOR_VL_PIN = PB14; // Motor V phase low side
constexpr Gpio MOTOR_WH_PIN = PB5;  // Motor W phase high side
constexpr Gpio MOTOR_WL_PIN = PB13; // Motor W phase low side

//---------- GPIO configs ----------//
struct GpioConfig {
    Gpio gpio;
    Mode mode;
};

// clang-format off
inline const std::array gpioList{
    GpioConfig{LED_PIN,     Mode::OUTPUT},
    GpioConfig{SWO_PIN,     Mode::SWO},
    GpioConfig{SWDIO_PIN,   Mode::SWDIO},
    GpioConfig{SWCLK_PIN,   Mode::SWCLK},
    GpioConfig{UART_TX_PIN, Mode::USART2_TX},
    GpioConfig{UART_RX_PIN, Mode::USART2_RX},
    GpioConfig{ENC_CSN_PIN, Mode::OUTPUT},
    GpioConfig{ENC_CLK_PIN, Mode::OUTPUT},
    GpioConfig{ENC_DO_PIN,  Mode::INPUT},
    GpioConfig{VOLT_PIN,    Mode::ANALOG},
    GpioConfig{MOTOR_UH_PIN, Mode::OUTPUT},
    GpioConfig{MOTOR_UL_PIN, Mode::OUTPUT},
    GpioConfig{MOTOR_VH_PIN, Mode::OUTPUT},
    GpioConfig{MOTOR_VL_PIN, Mode::OUTPUT},
    GpioConfig{MOTOR_WH_PIN, Mode::OUTPUT},
    GpioConfig{MOTOR_WL_PIN, Mode::OUTPUT},
    // GpioConfig{USB_DM_PIN,  Mode::OTG_FS_DM},
    // GpioConfig{USB_DP_PIN,  Mode::OTG_FS_DP},
};
// clang-format on

} // namespace Gpio

#endif // BLDC_DRIVERS_GPIO_GPIO_H
