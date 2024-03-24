//--------------------------------------------------
// BLDC Motor Controller
// motor.cpp
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/hardware.h>
#include <drivers/motor/motor.h>
#include <drivers/timer/timer.h>

bool Motor::init() {
    setPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_U, 100);
    setPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_V, 250);
    setPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_W, 500);
    startPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_U);
    startPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_V);
    startPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_W);
    return true;
}

//// clang-format off
// const bool stage[6][6] = {
//     {1,0,0,1,0,0},
//     {1,0,0,0,0,1},
//     {0,0,1,0,0,1},
//     {0,1,1,0,0,0},
//     {0,1,0,0,1,0},
//     {0,0,0,1,1,0},
// };
//// clang-format on
//
// void Motor::set(Controller::Output control) {
//    Gpio::write(Gpio::MOTOR_UL_PIN, control.ul);
//    Gpio::write(Gpio::MOTOR_UH_PIN, control.uh);
//    Gpio::write(Gpio::MOTOR_VL_PIN, control.vl);
//    Gpio::write(Gpio::MOTOR_VH_PIN, control.vh);
//    Gpio::write(Gpio::MOTOR_WL_PIN, control.wl);
//    Gpio::write(Gpio::MOTOR_WH_PIN, control.wh);
//}

void Motor::set(float angle, float magnitude) {
    // TODO Update timers
}
