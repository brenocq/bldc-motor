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
#include <utils/log.h>

int main() {
    HAL_Init();
    Clock::init();
    if (!Gpio::init())
        Error::hardFault("Failed to initialize GPIO driver");
    if (!Uart::init())
        Error::hardFault("Failed to initialize UART driver");
    if (!Adc::init())
        Error::hardFault("Failed to initialize ADC driver");
    if (!encoder.init())
        Error::hardFault("Failed to initialize encoder driver");
    if (!voltage.init())
        Error::hardFault("Failed to initialize voltage driver");
    if (!current.init())
        Error::hardFault("Failed to initialize current driver");
    if (!motor.init())
        Error::hardFault("Failed to initialize motor driver");

    Gpio::write(Gpio::LED_PIN, true);
    Log::success("Main", "Initialized");
    while (true) {
        // float angle = encoder.readAngle();
        // float volt = voltage.read();
        // float currW = current.readW();
        // float currUV = current.readUV();
        //// Uart::transmit("Angle: " + std::to_string(angle) + "\n");
        // Uart::transmit("Voltage: " + std::to_string(volt) + "V\n");
        // Uart::transmit("Currents: W(" + std::to_string(currW) + "A) UV(" + std::to_string(currUV) + "A)\n");

        motor.test();
    }
    return 0;
}
