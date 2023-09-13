//--------------------------------------------------
// BLDC Motor Controller
// main.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/clock/clock.h>
#include <drivers/encoder/encoder.h>
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/uart/uart.h>
#include <system/hal.h>
#include <utils/error.h>

int main() {
    HAL_Init();
    Clock::init();
    if (!Gpio::init())
        Error::hardFault();
    if (!Uart::init())
        Error::hardFault();
    if (!encoder.init())
        Error::hardFault();

    while (true) {
        float angle = encoder.readAngle();
        Uart::transmit("Angle: " + std::to_string(angle) + "\n");

        Gpio::write(Gpio::LED_PIN, true);
        Hardware::delay(0.5f);
        Gpio::write(Gpio::LED_PIN, false);
        Hardware::delay(0.5f);
    }
    return 0;
}
