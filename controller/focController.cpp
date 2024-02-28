//--------------------------------------------------
// BLDC Controller
// focController.cpp
// Date: 2023-10-25
//--------------------------------------------------
#include "focController.h"
#include <utils/log.h>

const std::array<Controller::Output, 8> FocController::_v = {Output(HIGH, LOW, LOW),  Output(HIGH, HIGH, LOW), Output(LOW, HIGH, LOW),
                                                             Output(LOW, HIGH, HIGH), Output(LOW, LOW, HIGH),  Output(HIGH, LOW, HIGH),
                                                             Output(LOW, LOW, LOW),   Output(HIGH, HIGH, HIGH)};

Controller::Output FocController::spaceVectorPWM(float angle, float magnitude) {
    // Null vectors
    static Output n0 = _v[7];
    static Output n1 = _v[8]; // TODO use

    // Angle within bounds
    while (angle < 0.0f)
        angle += 2 * M_PI;
    while (angle >= 2 * M_PI)
        angle -= 2 * M_PI;

    // Base vectors
    const float sector = 2 * M_PI / 6; // Sector angle
    Output b0, b1;
    unsigned idx = angle / sector;
    b0 = _v[idx];
    b1 = _v[(idx + 1) % 6];

    // Percentage of time each base vector should be selected
    float at = (angle - sector * idx) / sector;

    // Percentage of time base vectors should be selected instead of null vector
    float mt = magnitude;

    // Select vector
    if (rand() / float(RAND_MAX) < mt)
        return (rand() / float(RAND_MAX) < at) ? b1 : b0;
    else
        return n0;
}

Controller::Output FocController::control(State s, Control c, float dt) {
    static float a = 0.0f;
    a += 2 * M_PI * dt;
    if (a >= 2 * M_PI)
        a = 0;
    return spaceVectorPWM(a, 0.2f);
}
