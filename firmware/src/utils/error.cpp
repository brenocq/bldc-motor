//--------------------------------------------------
// BLDC Motor Controller
// error.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <utils/error.h>
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>

void Error::hardFault() {
    while (true) {
        Gpio::write(Gpio::LED_PIN, true);
        Hardware::delay(0.1f);
        Gpio::write(Gpio::LED_PIN, false);
        Hardware::delay(0.1f);
    }
}

void Error::memFault() {
    while (true) {
    }
}

void Error::logicFault() {
    while (true) {
    }
}
