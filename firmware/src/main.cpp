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
#include <utils/attaConnector.h>
#include <utils/error.h>
#include <utils/log.h>

struct MyTest {
    ATTA_CONNECTOR_CMD(MY_TEST_CMD);
    float test0;
    int test1;
};

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


    MyTest cmd;
    cmd.test0 = 10.0f;
    cmd.test1 = 20;
    AttaConnector::transmit<MyTest>(cmd);

    bool b = true;
    while (true) {
        //AttaConnector::update();
        //if(AttaConnector::receive<MyTest>(&cmd))
        //    Log::success("Main", "Received MyTest -> $0 $1", cmd.test0, cmd.test1);

        // float angle = encoder.readAngle();
        // float volt = voltage.read();
        // float currW = current.readW();
        // float currUV = current.readUV();
        //// Uart::transmit("Angle: " + std::to_string(angle) + "\n");
        // Uart::transmit("Voltage: " + std::to_string(volt) + "V\n");
        // Uart::transmit("Currents: W(" + std::to_string(currW) + "A) UV(" + std::to_string(currUV) + "A)\n");

        Gpio::write(Gpio::LED_PIN, b);
        b = !b;
        motor.test();
    }
    return 0;
}
