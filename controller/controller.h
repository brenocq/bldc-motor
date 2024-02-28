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
#include <ostream>

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
        enum Phase { LOW = 0, HIGH = 1, Z = 2 };
        Output() : Output(Z, Z, Z) {}
        Output(Phase u, Phase v, Phase w) : ul(u == LOW), uh(u == HIGH), vl(v == LOW), vh(v == HIGH), wl(w == LOW), wh(w == HIGH) {}

        bool ul;
        bool uh;
        bool vl;
        bool vh;
        bool wl;
        bool wh;
    };

    virtual Output control(State s, Control c, float dt) = 0;

    static constexpr uint8_t NUM_POLES = 8;
};
constexpr Controller::Output::Phase LOW = Controller::Output::LOW;
constexpr Controller::Output::Phase HIGH = Controller::Output::HIGH;
constexpr Controller::Output::Phase Z = Controller::Output::Z;

// Controller::Output overload
template <typename Tstream>
std::ostream& operator<<(Tstream& s, const Controller::Output& o) {
    char u = o.uh ? 'H' : (o.ul ? 'L' : 'Z');
    char v = o.vh ? 'H' : (o.vl ? 'L' : 'Z');
    char w = o.wh ? 'H' : (o.wl ? 'L' : 'Z');
    s << u << v << w;
    return s;
}

#endif // BLDC_CONTROLLER_H
