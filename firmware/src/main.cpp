//--------------------------------------------------
// BLDC Motor Controller
// main.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/adc/adc.h>
#include <drivers/clock/clock.h>
#include <drivers/encoder/encoder.h>
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/uart/uart.h>
#include <drivers/voltage/voltage.h>
#include <system/hal.h>
#include <utils/error.h>

int main() {
    HAL_Init();
    Clock::init();
    if (!Gpio::init())
        Error::hardFault();
    if (!Uart::init())
        Error::hardFault();
    if (!Adc::init())
        Error::hardFault();
    if (!encoder.init())
        Error::hardFault();
    if (!voltage.init())
        Error::hardFault();

    while (true) {
        // float angle = encoder.readAngle();
        float volt = voltage.read();
        Uart::transmit("Voltage: " + std::to_string(volt) + "V\n");

        Gpio::write(Gpio::LED_PIN, true);
        Hardware::delay(0.5f);
        Gpio::write(Gpio::LED_PIN, false);
        Hardware::delay(0.5f);
    }
    return 0;
}
