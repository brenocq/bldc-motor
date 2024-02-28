//--------------------------------------------------
// BLDC Motor Controller
// motor.h
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_MOTOR_MOTOR_H
#define BLDC_DRIVERS_MOTOR_MOTOR_H
#include <controller/controller.h>

class Motor {
  public:
    bool init();

    enum PhaseState { LOW = 0, HIGH = 1, Z = 2 };

    void set(Controller::Output control);
};

inline Motor motor;

#endif // BLDC_DRIVERS_MOTOR_MOTOR_H
