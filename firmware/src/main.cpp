//--------------------------------------------------
// BLDC Motor Controller
// main.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "attaConnector.h"
#include "attaConnectorCmds.h"
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
    if (!AttaConnector::init())
        Error::hardFault("Failed to initialize atta connector");

    Log::success("Main", "Initialized");

    MyTest0 t0;
    t0.u0 = 1;
    t0.u1 = 0b00001111;
    AttaConnector::transmit(t0);
    AttaConnector::transmit(t0);

    MyTest1 t1;
    t1.f = 1.5f;
    t1.u = 0b00001111;
    AttaConnector::transmit(t1);
    AttaConnector::transmit(t1);

    bool b = true;
    while (true) {
        AttaConnector::update();
        // while (AttaConnector::receive(&t0))
        //     Log::success("Main", "Received MyTest0 -> $x0 $x1", (int)t0.u0, (int)t0.u1);
        // while (AttaConnector::receive(&t1)) {
        //     Log::success("Main", "Received MyTest1 -> $0 $x1", t1.f, (int)t1.u);

        //    t0.u0 = 42;
        //    t0.u1 = 42;
        //    AttaConnector::transmit(t0);
        //}

        // float angle = encoder.readAngle();
        // float volt = voltage.read();
        // float currW = current.readW();
        // float currUV = current.readUV();
        // Uart::transmit("Angle: " + std::to_string(angle) + "\n");
        // Uart::transmit("Voltage: " + std::to_string(volt) + "V\n");
        // Uart::transmit("Currents: W(" + std::to_string(currW) + "A) UV(" + std::to_string(currUV) + "A)\n");

        Hardware::delay(0.1f);
        Gpio::write(Gpio::LED_PIN, b);
        b = !b;
        motor.test();
    }
    return 0;
}
