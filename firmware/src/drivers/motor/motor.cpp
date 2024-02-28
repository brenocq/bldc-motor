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

// clang-format off
const bool stage[6][6] = {
    {1,0,0,1,0,0},
    {1,0,0,0,0,1},
    {0,0,1,0,0,1},
    {0,1,1,0,0,0},
    {0,1,0,0,1,0},
    {0,0,0,1,1,0},
};
// clang-format on

void Motor::set(Controller::Output control) {
    Gpio::write(Gpio::MOTOR_UL_PIN, control.ul);
    Gpio::write(Gpio::MOTOR_UH_PIN, control.uh);
    Gpio::write(Gpio::MOTOR_VL_PIN, control.vl);
    Gpio::write(Gpio::MOTOR_VH_PIN, control.vh);
    Gpio::write(Gpio::MOTOR_WL_PIN, control.wl);
    Gpio::write(Gpio::MOTOR_WH_PIN, control.wh);
}
