//--------------------------------------------------
// BLDC Simulation
// motor.h
// Date: 2023-09-27
//--------------------------------------------------
#ifndef BLDC_MOTOR_H
#define BLDC_MOTOR_H

class Motor {
  public:
    Motor() = default;
    Motor(float R, float L, float J, float F, float P, float l);

    void update(atta::vec3 v, float Tl, float dt);

    atta::vec3 getVoltage() const;
    atta::vec3 getCurrent() const;
    float getPosition() const;
    float getVelocity() const;
    float getLoadTorque() const;
    float getElectromagneticTorque() const;
    float getMotorTorque() const;
    atta::vec3 getEMF() const;

  private:
    // Back EMF function
    float f(float theta, uint8_t phase);

    // Parameters
    float _R; // Phase resistance
    float _L; // Phase inductance
    float _J; // Rotor inertia
    float _F; // Rotor friction
    float _P; // Number of poles
    float _l; // Flux linkage

    // State
    atta::vec3 _i; // Phase current
    float _w;      // Angular velocity
    float _theta;  // Angular position

    // Control
    atta::vec3 _v; // Phase voltage
    float _Tl;     // Load torque

    // Internal
    atta::vec3 _e; // Back EMF
    float _Te;     // Electromagnetic torque
    float _Tm;     // Motor torque
};

#endif // BLDC_MOTOR_H
