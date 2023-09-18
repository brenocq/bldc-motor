//--------------------------------------------------
// BLDC Motor Controller
// motor.cpp
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/motor/motor.h>

bool Motor::init() {
    Gpio::write(Gpio::MOTOR_UH_PIN, Gpio::LOW);
    Gpio::write(Gpio::MOTOR_UL_PIN, Gpio::LOW);
    Gpio::write(Gpio::MOTOR_VH_PIN, Gpio::LOW);
    Gpio::write(Gpio::MOTOR_VL_PIN, Gpio::LOW);
    Gpio::write(Gpio::MOTOR_WH_PIN, Gpio::LOW);
    Gpio::write(Gpio::MOTOR_WL_PIN, Gpio::LOW);
    return true;
}

void Motor::test() {
    Gpio::write(Gpio::MOTOR_UH_PIN, Gpio::HIGH);
    Gpio::write(Gpio::MOTOR_VH_PIN, Gpio::HIGH);
    Gpio::write(Gpio::MOTOR_WH_PIN, Gpio::HIGH);
    Hardware::delay(0.1f);
    Gpio::write(Gpio::MOTOR_UH_PIN, Gpio::LOW);
    Gpio::write(Gpio::MOTOR_VH_PIN, Gpio::LOW);
    Gpio::write(Gpio::MOTOR_WH_PIN, Gpio::LOW);
    Hardware::delay(0.1f);
}
