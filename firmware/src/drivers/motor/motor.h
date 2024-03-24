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

    // void set(Controller::Output control);
    void set(float angle, float magnitude);
};

inline Motor motor;

#endif // BLDC_DRIVERS_MOTOR_MOTOR_H
