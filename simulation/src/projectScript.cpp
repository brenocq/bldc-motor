//--------------------------------------------------
// BLDC Simulation
// projectScript.cpp
// Date: 2023-09-27
//--------------------------------------------------
#include "projectScript.h"
#include "imgui.h"
#include "implot.h"

void ProjectScript::onStart() {
    _motorData = {};
    float R = 3.6f; // Phase resistance
    float L = 0.1f; // Phase inductance
    float J = 1.0f; // Rotor inertia
    float F = 0.0f; // Rotor friction
    float P = 12;   // Number of poles
    float l = 1.0f; // Flux linkage
    _motor = Motor(R, L, J, F, P, l);
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

    // Update motor
    float V = 7.8f;  // Voltage
    float Tl = 0.0f; // Load torque
    _motor.update({V, 0.0f, 0.0f}, 0.0f, dt);
    _motorData.position.push_back(_motor.getPosition());
    _motorData.velocity.push_back(_motor.getVelocity());
    _motorData.motorTorque.push_back(_motor.getMotorTorque());
    _motorData.loadTorque.push_back(_motor.getLoadTorque());
    _motorData.electromagneticTorque.push_back(_motor.getElectromagneticTorque());
    _motorData.current.push_back(_motor.getCurrent());
    _motorData.voltage.push_back(_motor.getVoltage());
    _motorData.emf.push_back(_motor.getEMF());
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
        ImGui::Text("Input");
        if (ImPlot::BeginPlot("Phase voltages")) {
            ImPlot::PlotLine("v_u", _time3.data(), &_motorData.voltage[0].x, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("v_v", _time3.data(), &_motorData.voltage[0].y, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("v_w", _time3.data(), &_motorData.voltage[0].z, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Torque")) {
            ImPlot::PlotLine("Load torque", _time.data(), _motorData.loadTorque.data(), _time.size());
            ImPlot::PlotLine("Electromagnetic torque", _time.data(), _motorData.electromagneticTorque.data(), _time.size());
            ImPlot::PlotLine("Motor torque", _time.data(), _motorData.motorTorque.data(), _time.size());
            ImPlot::EndPlot();
        }

        ImGui::Text("State");
        if (ImPlot::BeginPlot("Phase currents")) {
            ImPlot::PlotLine("i_u", _time3.data(), &_motorData.current[0].x, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("i_v", _time3.data(), &_motorData.current[0].y, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("i_w", _time3.data(), &_motorData.current[0].z, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Rotor velocity")) {
            ImPlot::PlotLine("Angular velocity", _time.data(), _motorData.velocity.data(), _time.size());
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Rotor position")) {
            ImPlot::PlotLine("Angular position", _time.data(), _motorData.position.data(), _time.size());
            ImPlot::EndPlot();
        }

        ImGui::Text("Derived");
        if (ImPlot::BeginPlot("Back EMF")) {
            ImPlot::PlotLine("e_u", _time3.data(), &_motorData.emf[0].x, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("e_v", _time3.data(), &_motorData.emf[0].y, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::PlotLine("e_w", _time3.data(), &_motorData.emf[0].z, _time.size(), 0, sizeof(atta::vec3));
            ImPlot::EndPlot();
        }
    }
    ImGui::End();
}
