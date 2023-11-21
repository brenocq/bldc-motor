//--------------------------------------------------
// BLDC Motor Controller
// error.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <utils/error.h>
#include <utils/log.h>

void Error::hardFault(const char* reason) {
    Log::error("Error", "HardFault: $0", reason);
    while (true) {
        //Gpio::write(Gpio::LED_PIN, true);
        //Hardware::delay(0.1f);
        //Gpio::write(Gpio::LED_PIN, false);
        //Hardware::delay(0.1f);
    }
}

void Error::memFault() {
    while (true) {
    }
}

void Error::busFault() {
    while (true) {
    }
}

void Error::usageFault() {
    while (true) {
    }
}
