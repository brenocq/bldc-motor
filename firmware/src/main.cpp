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
    // if (!Adc::init())
    //     Error::hardFault("Failed to initialize ADC driver");
    // if (!encoder.init())
    //     Error::hardFault("Failed to initialize encoder driver");
    // if (!voltage.init())
    //     Error::hardFault("Failed to initialize voltage driver");
    // if (!current.init())
    //     Error::hardFault("Failed to initialize current driver");
    // if (!motor.init())
    //     Error::hardFault("Failed to initialize motor driver");
    if (!AttaConnector::init())
        Error::hardFault("Failed to initialize atta connector");

    Log::success("Main", "Initialized");

    // bool b = true;
    while (true) {
        AttaConnector::update();
        //Log::success("Main", "Test");
        MyTest1 t1;
        t1.f = 4.5f;
        t1.u = 42;
        AttaConnector::transmit(t1);
        Hardware::delayMs(100);

        // MotorState state{};
        // state.batteryVoltage = voltage.read();
        // state.currentUV = current.readUV();
        // state.currentW = current.readW();
        //// state.rotorPosition = encoder.readAngle();
        // AttaConnector::transmit(state);

        // Gpio::write(Gpio::LED_PIN, b);
        // b = !b;
        // motor.test();
    }
    return 0;
}
