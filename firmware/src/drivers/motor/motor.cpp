//--------------------------------------------------
// BLDC Motor Controller
// motor.cpp
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/hardware.h>
#include <drivers/motor/motor.h>
#include <drivers/timer/timer.h>
#include <utils/log.h>

bool Motor::init() {
    setPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_U, 0);
    setPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_V, 0);
    setPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_W, 0);
    startPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_U);
    startPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_V);
    startPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_W);
    return true;
}

void Motor::set(float angle, float magnitude) {
    // Sector from the space vector diagram
    int sector = static_cast<int>(angle / (M_PI / 3));

    // Calculate ratios
    float sectorAngle = angle - sector * (M_PI / 3);
    float t1 = magnitude * std::sin(M_PI / 3 - sectorAngle); // Time ratio first vector from sector
    float t2 = magnitude * std::sin(sectorAngle);            // Time ratio second vector from sector
    float t0 = 1.0 - t1 - t2;                                // Time ratio null vector

    // Calculate channel CCR for each phase
    constexpr uint16_t maxPeriod = Timer::MOTOR_PERIOD;
    uint16_t uPeriod = maxPeriod;
    uint16_t vPeriod = maxPeriod;
    uint16_t wPeriod = maxPeriod;

    switch (sector) {
        case 0:
            uPeriod -= (t0 / 2) * maxPeriod;
            vPeriod -= ((t0 / 2) + t1) * maxPeriod;
            wPeriod -= ((t0 / 2) + t1 + t2) * maxPeriod;
            break;
        case 1:
            uPeriod -= ((t0 / 2) + t2) * maxPeriod;
            vPeriod -= (t0 / 2) * maxPeriod;
            wPeriod -= ((t0 / 2) + t1 + t2) * maxPeriod;
            break;
        case 2:
            uPeriod -= ((t0 / 2) + t1 + t2) * maxPeriod;
            vPeriod -= (t0 / 2) * maxPeriod;
            wPeriod -= ((t0 / 2) + t1) * maxPeriod;
            break;
        case 3:
            uPeriod -= ((t0 / 2) + t1 + t2) * maxPeriod;
            vPeriod -= ((t0 / 2) + t2) * maxPeriod;
            wPeriod -= (t0 / 2) * maxPeriod;
            break;
        case 4:
            uPeriod -= ((t0 / 2) + t1) * maxPeriod;
            vPeriod -= ((t0 / 2) + t1 + t2) * maxPeriod;
            wPeriod -= (t0 / 2) * maxPeriod;
            break;
        case 5:
            uPeriod -= (t0 / 2) * maxPeriod;
            vPeriod -= ((t0 / 2) + t1 + t2) * maxPeriod;
            wPeriod -= ((t0 / 2) + t2) * maxPeriod;
            break;
        default:
            Log::error("Motor", "Invalid section");
            return;
    }

    setPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_U, uPeriod);
    setPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_V, vPeriod);
    setPwm(Timer::MOTOR_TIM, Timer::MOTOR_CH_W, wPeriod);
}
