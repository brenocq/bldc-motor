//--------------------------------------------------
// BLDC Motor Controller
// motor.h
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_MOTOR_MOTOR_H
#define BLDC_DRIVERS_MOTOR_MOTOR_H

class Motor {
  public:
    bool init();

    enum PhaseState { LOW = 0, HIGH = 1, Z = 2 };

    // Motor control signal
    struct Control {
        Control() : Control(Z, Z, Z) {}
        Control(PhaseState u, PhaseState v, PhaseState w) : ul(u == LOW), uh(u == HIGH), vl(v == LOW), vh(v == HIGH), wl(w == LOW), wh(w == HIGH) {}

        bool ul;
        bool uh;
        bool vl;
        bool vh;
        bool wl;
        bool wh;
    };

    void set(Control control);
};

inline Motor motor;

#endif // BLDC_DRIVERS_MOTOR_MOTOR_H
