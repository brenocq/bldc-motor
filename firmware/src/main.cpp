//--------------------------------------------------
// BLDC Motor Controller
// main.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/adc/adc.h>
#include <drivers/clock/clock.h>
#include <drivers/current/current.h>
#include <drivers/encoder/encoder.h>
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/motor/motor.h>
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
    if (!current.init())
        Error::hardFault();
    if (!motor.init())
        Error::hardFault();

    while (true) {
        // float angle = encoder.readAngle();
        float volt = voltage.read();
        float currW = current.readW();
        float currUV = current.readUV();
        // Uart::transmit("Angle: " + std::to_string(angle) + "\n");
        Uart::transmit("Voltage: " + std::to_string(volt) + "V\n");
        Uart::transmit("Currents: W(" + std::to_string(currW) + "A) UV(" + std::to_string(currUV) + "A)\n");

        Gpio::write(Gpio::LED_PIN, true);
        Hardware::delay(0.5f);
        Gpio::write(Gpio::LED_PIN, false);
        Hardware::delay(0.5f);
        motor.test();
    }
    return 0;
}
