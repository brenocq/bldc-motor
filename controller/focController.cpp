//--------------------------------------------------
// BLDC Controller
// focController.cpp
// Date: 2023-10-25
//--------------------------------------------------
#include "focController.h"

const std::array<Controller::Output, 8> FocController::_v = {
    Output(Output::HIGH, Output::LOW, Output::LOW),  Output(Output::HIGH, Output::HIGH, Output::LOW), Output(Output::LOW, Output::HIGH, Output::LOW),
    Output(Output::LOW, Output::HIGH, Output::HIGH), Output(Output::LOW, Output::LOW, Output::HIGH),  Output(Output::HIGH, Output::LOW, Output::HIGH),
    Output(Output::LOW, Output::LOW, Output::LOW),   Output(Output::HIGH, Output::HIGH, Output::HIGH)};

Controller::Output FocController::spaceVectorPWM(float angle, float magnitude) {
    // Null vectors
    static Output n0 = _v[7];
    static Output n1 = _v[8];

    // Angle within bounds
    while (angle < 0.0f)
        angle += 2 * M_PI;
    while (angle >= 2 * M_PI)
        angle -= 2 * M_PI;

    // Base vectors
    Output b0, b1;
    unsigned idx = angle / (2 * M_PI);
    b0 = _v[idx];
    b1 = _v[(idx + 1) % 6];

    // Percentage of time each base vector should be selected
    const float dist = 2 * M_PI / 6;
    float at = (angle - dist * idx) / dist;

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
    return spaceVectorPWM(a, 1.0f);
}
