//--------------------------------------------------
// BLDC Controller
// controller.cpp
// Date: 2023-09-28
//--------------------------------------------------
#include "controller.h"

Controller::Output Controller::control(State s, Control c, float dt) { return {Output::LOW, Output::LOW, Output::LOW}; }
