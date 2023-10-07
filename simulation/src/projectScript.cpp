//--------------------------------------------------
// BLDC Simulation
// projectScript.cpp
// Date: 2023-09-27
//--------------------------------------------------
#include "projectScript.h"
#include "imgui.h"
#include "implot.h"
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>

namespace cmp = atta::component;

cmp::Entity motorEntity(0);

void ProjectScript::onStart() {
    _motorData = {};
    _time = {};
    _time3 = {};
    float R = 3.6f; // Phase resistance
    float L = 0.1f; // Phase inductance (TODO)
    float J = 0.1f; // Rotor inertia (TODO)
    float F = 1.0f; // Rotor friction (TODO)
    float P = 8;    // Number of poles
    float l = 1.0f; // Flux linkage (TODO)
    _motor = Motor(R, L, J, F, P, l);
    _tController = TrapezoidalController();
}
void ProjectScript::onStop() {}

void ProjectScript::onUpdateBefore(float dt) {
    // Update time
    float lastTime = _time.empty() ? -dt : _time.back();
    float time = lastTime + dt;
    _time.push_back(time);
    _time3.push_back(time);
    _time3.push_back(time);
    _time3.push_back(time);

    // Update controller
    Controller::Control u;
    u.position = NAN;
    u.velocity = NAN;
    u.torque = NAN;
    Controller::State x;
    x.voltage = 7.8f;
    x.currents = {_motor.getCurrent()[0], _motor.getCurrent()[1], _motor.getCurrent()[2]};
    x.theta = _motor.getPosition();
    Controller::Output output = _tController.control(x, u, dt);

    // Update motor
    float V = 7.8f;  // Voltage
    float Tl = 0.1f; // Load torque
    atta::vec3 Vs = {output.ah * V, output.bh * V, output.ch * V};
    _motor.update(Vs, Tl, dt);
    _motorData.position.push_back(_motor.getPosition());
    _motorData.velocity.push_back(_motor.getVelocity());
    _motorData.motorTorque.push_back(_motor.getMotorTorque());
    _motorData.loadTorque.push_back(_motor.getLoadTorque());
    _motorData.electromagneticTorque.push_back(_motor.getElectromagneticTorque());
    _motorData.current.push_back(_motor.getCurrent());
    _motorData.voltage.push_back(_motor.getVoltage());
    _motorData.emf.push_back(_motor.getEMF());
    motorEntity.get<cmp::Transform>()->orientation.set2DAngle(_motorData.position.back());
}

void ProjectScript::onUIRender() {
    // Setup static data
    static float x_data[100];
    static float y_data[100];
    for (int i = 0; i < 100; i++) {
        x_data[i] = i;
        y_data[i] = sin(i * 0.1f);
    }

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once);
    ImGui::Begin("Plots");
    {
        if (ImPlot::BeginPlot("Phase voltages")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("v_u", _time3.data(), &_motorData.voltage[0].x, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("v_v", _time3.data(), &_motorData.voltage[0].y, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("v_w", _time3.data(), &_motorData.voltage[0].z, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Phase currents")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("i_u", _time3.data(), &_motorData.current[0].x, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("i_v", _time3.data(), &_motorData.current[0].y, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("i_w", _time3.data(), &_motorData.current[0].z, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Torque")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Load torque", _time.data(), _motorData.loadTorque.data(), _time.size());
            ImPlot::PlotLine("Electromagnetic torque", _time.data(), _motorData.electromagneticTorque.data(), _time.size());
            ImPlot::PlotLine("Motor torque", _time.data(), _motorData.motorTorque.data(), _time.size());
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Rotor velocity")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Angular velocity", _time.data(), _motorData.velocity.data(), _time.size());
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Rotor position")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Angular position", _time.data(), _motorData.position.data(), _time.size());
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Back EMF")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("e_u", _time3.data(), &_motorData.emf[0].x, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("e_v", _time3.data(), &_motorData.emf[0].y, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("e_w", _time3.data(), &_motorData.emf[0].z, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::EndPlot();
        }
    }
    ImGui::End();
}
