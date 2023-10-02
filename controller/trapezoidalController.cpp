//--------------------------------------------------
// BLDC Controller
// trapezoidalController.cpp
// Date: 2023-10-02
//--------------------------------------------------
#include "trapezoidalController.h"

Controller::Output TrapezoidalController::control(State s, Control c, float dt) {
    float theta = s.theta / NUM_POLES; // Electrical angular position
    float s = 2 * M_PI / 6;
    float section = (theta / s) + 0.5f; // Section where magnet north pole is
    if (section > 6.0f)
        section -= 6.0f;

    static std::array<Output, 6> commutations = {
        {Output::HIGH, Output::LOW, Output::Z}, {Output::HIGH, Output::Z, Output::LOW}, {Output::Z, Output::HIGH, Output::LOW},
        {Output::LOW, Output::HIGH, Output::Z}, {Output::LOW, Output::Z, Output::HIGH}, {Output::Z, Output::LOW, Output::HIGH},
    };

    return commutations[(int)section];
}
