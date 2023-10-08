//--------------------------------------------------
// BLDC Controller
// trapezoidalController.cpp
// Date: 2023-10-02
//--------------------------------------------------
#include "trapezoidalController.h"

Controller::Output TrapezoidalController::control(State s, Control c, float dt) {
    static std::array<Output, 6> commutations = {
        Output(Output::HIGH, Output::LOW, Output::Z), Output(Output::HIGH, Output::Z, Output::LOW), Output(Output::Z, Output::HIGH, Output::LOW),
        Output(Output::LOW, Output::HIGH, Output::Z), Output(Output::LOW, Output::Z, Output::HIGH), Output(Output::Z, Output::LOW, Output::HIGH),
    };

    float theta = s.theta * NUM_POLES; // Electrical angular position
    theta = std::fmod(theta, 2 * M_PI);
    if (theta < 0)
        theta += 2 * M_PI;

    const float sectionSize = 2 * M_PI / 6;
    float section = ((theta + M_PI) / sectionSize) + 0.5f; // Section where magnet north pole is
    if (section > 6.0f)
        section -= 6.0f;
    // LOG_DEBUG("TrapezoidalController", "Theta $0 section $1", theta, section);
    return commutations[(int)section];
}
