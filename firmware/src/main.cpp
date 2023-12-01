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
#include <drivers/encoder/encoder.h>
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/i2c/i2c.h>
#include <drivers/imu/imu.h>
#include <drivers/led/led.h>
#include <drivers/motor/motor.h>
#include <drivers/phase/phase.h>
#include <drivers/spi/spi.h>
#include <drivers/timer/timer.h>
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
    if (!I2c::init())
        Error::hardFault("Failed to initialize I2C driver");
    if (!Spi::init())
        Error::hardFault("Failed to initialize SPI driver");
    if (!Adc::init())
        Error::hardFault("Failed to initialize ADC driver");
    if (!Timer::init())
        Error::hardFault("Failed to initialize TIMER driver");
    if (!led.init())
        Error::hardFault("Failed to initialize led driver");
    // if (!encoder.init())
    //     Error::hardFault("Failed to initialize encoder driver");
    if (!voltage.init())
        Error::hardFault("Failed to initialize voltage driver");
    if (!phaseU.init(Phase::U, I2c::Peripheral::I2C3, I2c::PHASE_U_ADDR))
        Error::hardFault("Failed to initialize phase driver for phase U");
    if (!phaseV.init(Phase::V, I2c::Peripheral::I2C3, I2c::PHASE_V_ADDR))
        Error::hardFault("Failed to initialize phase driver for phase V");
    if (!phaseW.init(Phase::W, I2c::Peripheral::I2C3, I2c::PHASE_W_ADDR))
        Error::hardFault("Failed to initialize phase driver for phase W");
    if (!imu.init(Spi::Peripheral::SPI2, Gpio::IMU_CS_PIN))
        Error::hardFault("Failed to initialize IMU");
    // if (!motor.init())
    //     Error::hardFault("Failed to initialize motor driver");
    if (!AttaConnector::init())
        Error::hardFault("Failed to initialize atta connector");

    Log::success("Main", "Initialized");

    // bool b = true;
    while (true) {
        AttaConnector::update();
        Log::debug("Main", "Voltage Source $0", voltage.read());
        Log::debug("Main", "Voltage Phase U $0V", phaseU.readVoltage());
        Log::debug("Main", "Voltage Phase V $0V", phaseV.readVoltage());
        Log::debug("Main", "Voltage Phase W $0V", phaseW.readVoltage());
        Log::debug("Main", "Current Phase U $0A", phaseU.readCurrent());
        Log::debug("Main", "Current Phase V $0A", phaseV.readCurrent());
        Log::debug("Main", "Current Phase W $0A", phaseW.readCurrent());
        imu.getAcc(nullptr, nullptr, nullptr);
        imu.getGyr(nullptr, nullptr, nullptr);
        MyTest1 t1;
        t1.f = 4.5f;
        t1.u = 42;
        AttaConnector::transmit(t1);
        led.setColor(0, 255, 255, 255);
        Hardware::delayMs(1000);

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
