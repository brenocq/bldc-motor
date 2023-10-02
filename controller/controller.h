//--------------------------------------------------
// BLDC Controller
// controller.h
// Date: 2023-09-28
//--------------------------------------------------
#ifndef BLDC_CONTROLLER_H
#define BLDC_CONTROLLER_H
#include <array>

class Controller {
  public:
    Controller() = default;

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
        Output(std::array<bool, 3> a) : al(!a[0]), ah(a[0]), bl(!a[1]), bh(a[1]), cl(!a[2]), ch(a[2]) {}
        Output(std::array<State, 3> a) : al(a[0] == LOW), ah(a[0] == HIGH), bl(a[1] == LOW), bh(a[1] == HIGH), cl(a[2] == LOW), ch(a[2] == HIGH) {}
        bool al;
        bool ah;
        bool bl;
        bool bh;
        bool cl;
        bool ch;
    }

    using Output = std::array<bool, 3>; // Voltage on each phase (HIGH or LOW)

    virtual Output control(State s, Control c, float dt) = 0;

    static constexpr uint8_t NUM_POLES = 8;
};

#endif // BLDC_CONTROLLER_H
