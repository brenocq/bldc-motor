//--------------------------------------------------
// BLDC Motor Controller
// main.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <common/attaConnector.h>
#include <common/attaConnectorCmds.h>
#include <controller/focController.h>
#include <drivers/adc/adc.h>
#include <drivers/clock/clock.h>
#include <drivers/dma/dma.h>
#include <drivers/encoder/encoder.h>
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/i2c/i2c.h>
#include <drivers/imu/imu.h>
#include <drivers/interrupt/interrupt.h>
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
    if (!Dma::init())
        Error::hardFault("Failed to initialize DMA driver");
    if (!Interrupt::init())
        Error::hardFault("Failed to initialize Interrupt driver");

    // Timer-DMA
    Timer::linkDma(Timer::LED_TIM, Timer::LED_CH, Dma::getHandle(Dma::LED_DMA, Dma::LED_STREAM));

    if (!led.init())
        Error::hardFault("Failed to initialize led driver");
    if (!encoder.init())
        Error::hardFault("Failed to initialize encoder driver");
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
    if (!motor.init())
        Error::hardFault("Failed to initialize motor driver");
    if (!AttaConnector::init())
        Error::hardFault("Failed to initialize atta connector");

    Log::success("Main", "Initialized");

    // Test led
    led.setColor(0, 25, 0, 25);
    led.setColor(1, 0, 25, 0);
    led.show();

    FocController foc;
    while (true) {
        // AttaConnector::update();

        // Read motor state
        // MotorState state{};
        // state.sourceVoltage = voltage.read();
        // state.phaseVoltage[0] = phaseU.readVoltage();
        // state.phaseVoltage[1] = phaseV.readVoltage();
        // state.phaseVoltage[2] = phaseW.readVoltage();
        // state.phaseCurrent[0] = phaseU.readCurrent();
        // state.phaseCurrent[1] = phaseV.readCurrent();
        // state.phaseCurrent[2] = phaseW.readCurrent();
        // state.rotorPosition = encoder.readAngle();
        // AttaConnector::transmit(state);

        // Execute controller
        Controller::Output control = foc.control({}, {}, 0.00005);

        // Update motor signals
        // motor.set(control);
        // Log::debug("Main", "Control: $0", control);
    }
    return 0;
}
