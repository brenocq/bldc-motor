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

void Motor::test() {
    Gpio::write(Gpio::MOTOR_UH_PIN, stage[0][0]);
    Gpio::write(Gpio::MOTOR_UL_PIN, stage[0][1]);
    Gpio::write(Gpio::MOTOR_VH_PIN, stage[0][2]);
    Gpio::write(Gpio::MOTOR_VL_PIN, stage[0][3]);
    Gpio::write(Gpio::MOTOR_WH_PIN, stage[0][4]);
    Gpio::write(Gpio::MOTOR_WL_PIN, stage[0][5]);
}
