//--------------------------------------------------
// BLDC Simulation
// motor.cpp
// Date: 2023-09-27
//--------------------------------------------------
#include "motor.h"

Motor::Motor(float R, float L, float J, float F, float P, float l) : _R(R), _L(L), _J(J), _F(F), _P(P), _l(l) {
    _i = {};
    _w = {};
    _theta = {};
}

void Motor::update(atta::vec3 v, float Tl, float dt) {
    _v = v;
    _Tl = Tl;

    for (int i = 0; i < SUBSTEPS; i++) {
        // Derivatives
        atta::vec3 di = {0.0f, 0.0f, 0.0f};
        float dw = 0.0f;
        float dtheta = 0.0f;

        // A
        const float rl = _R / _L;
        atta::vec3 lf = {_l * f(_theta, 0), _l * f(_theta, 1), _l * f(_theta, 2)};
        di[0] = -rl * _i[0] + _w * lf[0] / _J;
        di[1] = -rl * _i[1] + _w * lf[1] / _J;
        di[2] = -rl * _i[2] + _w * lf[2] / _J;
        dw = (_i[0] * lf[0] + _i[1] * lf[1] + _i[2] * lf[2] - _w * _F) / _J;
        dtheta = _w * _P / 2;

        // B
        di += _v / _L;
        dw -= _Tl / _J;

        // Update state
        _i += di * dt / SUBSTEPS;
        _w += dw * dt / SUBSTEPS;
        _theta += dtheta * dt / SUBSTEPS;

        // Calculate internal variables
        _e = atta::vec3(f(_theta, 0), f(_theta, 1), f(_theta, 2)) * _l * _w;
        _Te = (_e[0] * _i[0] + _e[1] * _i[1] + _e[2] * _i[2]) / _w;
        _Tm = _Te - _Tl;
    }
}

float Motor::f(float theta, uint8_t phase) { return sin(theta - phase * (2 * M_PI / 3)); }

atta::vec3 Motor::getVoltage() const { return _v; }
atta::vec3 Motor::getCurrent() const { return _i; }
float Motor::getPosition() const { return _theta; }
float Motor::getVelocity() const { return _w; }
float Motor::getLoadTorque() const { return _Tl; }
float Motor::getElectromagneticTorque() const { return _Te; }
float Motor::getMotorTorque() const { return _Tm; }
atta::vec3 Motor::getEMF() const { return _e; }
