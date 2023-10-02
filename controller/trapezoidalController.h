//--------------------------------------------------
// BLDC Controller
// trapezoidalController.h
// Date: 2023-10-02
//--------------------------------------------------
#ifndef BLDC_TRAPEZOIDAL_CONTROLLER_H
#define BLDC_TRAPEZOIDAL_CONTROLLER_H
#include "controller.h"

// Trapezoidal Commutation Controller
class TrapezoidalController : public Controller {
  public:
    TrapezoidalController() = default;

    Output control(State s, Control c, float dt) override;
};

#endif // BLDC_TRAPEZOIDAL_CONTROLLER_H
