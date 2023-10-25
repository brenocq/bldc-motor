//--------------------------------------------------
// BLDC Controller
// controller.h
// Date: 2023-09-28
//--------------------------------------------------
#ifndef BLDC_CONTROLLER_H
#define BLDC_CONTROLLER_H
#include <array>
#include <cmath>
#include <cstdint>

class Controller {
  public:
    Controller() = default;
    virtual ~Controller() = default;

    // Plant state
    struct State {
        float voltage;                 // Supply voltage
        std::array<float, 3> currents; // Phase currents
        float theta;                   // Rotor angular position
    };

    // Control signals
    struct Control {
        float position; // Desired position (NAN if no position control)
        float velocity; // Desired velocity (NAN if no velocity control)
        float torque;   // Desired torque (NAN if no torque control)
    };

    // Controller output
    struct Output {
        enum State { LOW = 0, HIGH = 1, Z = 2 };
        Output() : Output(Z, Z, Z) {}
        Output(State a, State b, State c) : al(a == LOW), ah(a == HIGH), bl(b == LOW), bh(b == HIGH), cl(c == LOW), ch(c == HIGH) {}

        bool al;
        bool ah;
        bool bl;
        bool bh;
        bool cl;
        bool ch;
    };

    virtual Output control(State s, Control c, float dt) = 0;

    static constexpr uint8_t NUM_POLES = 8;
};

#endif // BLDC_CONTROLLER_H
