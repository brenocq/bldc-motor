//--------------------------------------------------
// BLDC Motor Controller
// main.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/clock/clock.h>
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/usb/usb.h>
#include <system/hal.h>
#include <utils/error.h>

int main() {
    HAL_Init();
    Clock::init();
    if (!Gpio::init())
        Error::hardFault();
    //if (!Usb::init())
    //    Error::hardFault();

    uint8_t str[] = "Hi!\n";
    while (true) {
        Gpio::write(Gpio::LED_PIN, true);
        Hardware::delay(0.5f);
        Gpio::write(Gpio::LED_PIN, false);
        Hardware::delay(0.5f);
        // Usb::transmit(str, 4);
    }
    return 0;
}
