//--------------------------------------------------
// BLDC Simulation
// projectScript.cpp
// Date: 2023-09-27
//--------------------------------------------------
#include "projectScript.h"
#include "attaConnectorCmds.h"
#include "attaConnectorPlatform.h"
#include "imgui.h"
#include "implot.h"
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>

namespace cmp = atta::component;

cmp::Entity motorEntity(0);

void ProjectScript::onLoad() {
    if (!AttaConnector::init())
        LOG_WARN("ProjectScript", "Failed to initialize atta connector");
}

void ProjectScript::onStart() {
    _motorData = {};
    _time = {};
    _time3 = {};
    float R = 3.6f; // Phase resistance
    float L = 0.1f; // Phase inductance (TODO)
    float J = 0.1f; // Rotor inertia (TODO)
    float F = 1.0f; // Rotor friction (TODO)
    float P = 14;   // Number of poles
    float l = 1.0f; // Flux linkage (TODO)
    _motor = Motor(R, L, J, F, P, l);
    _tController = TrapezoidalController();
    _focController = FocController();

    _phyMotorTime = {};
    _phyMotorData = {};
}
void ProjectScript::onStop() {}

void ProjectScript::onUpdateBefore(float dt) {
    // Handle motor serial connection
    handleSerial();
    handleAttaConnector();

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
    //Controller::Output output = _tController.control(x, u, dt);
    Controller::Output output = _focController.control(x, u, dt);

    // Update motor
    float V = 7.8f;  // Voltage
    float Tl = 0.1f; // Load torque
    auto calcV = [V](bool h, bool l) {
        if (h && !l)
            return V;
        else if (!h && l)
            return 0.0f;
        else
            return -1.0f;
    };
    atta::vec3 Vs = {calcV(output.ah, output.al), calcV(output.bh, output.bl), calcV(output.ch, output.cl)};
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
    ImGui::Begin("Simulation");
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

    ImGui::Begin("Motor State");
    {
        if (ImPlot::BeginPlot("Battery Voltage")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Battery Voltage", _phyMotorTime.data(), _phyMotorData.batteryVoltage.data(), _phyMotorTime.size());
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Currents")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("UV", _phyMotorTime.data(), _phyMotorData.currentUV.data(), _phyMotorTime.size());
            ImPlot::PlotLine("W", _phyMotorTime.data(), _phyMotorData.currentW.data(), _phyMotorTime.size());
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Rotor Position")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Rotor Position", _phyMotorTime.data(), _phyMotorData.rotorPosition.data(), _phyMotorTime.size());
            ImPlot::EndPlot();
        }
    }
    ImGui::End();
}

std::weak_ptr<atta::io::Serial> _gSerial;

void ProjectScript::handleSerial() {
    std::vector<std::string> deviceNames = atta::io::Serial::getAvailableDeviceNames();
    // Handle disconnect
    if (_serial) {
        bool found = false;
        for (std::string deviceName : deviceNames)
            if (_serial->getDeviceName() == deviceName) {
                found = true;
                break;
            }
        if (!found) {
            LOG_DEBUG("ProjectScript", "Disconnect from [y]$0", _serial->getDeviceName());
            _gSerial = _serial = nullptr;
        }
    }

    // Try to connect to serial if not connected
    if (!_serial) {
        for (std::string deviceName : deviceNames) {
            if (deviceName.find("STMicroelectronics") != std::string::npos) {
                atta::io::Serial::CreateInfo info{};
                info.deviceName = deviceName;
                info.baudRate = 115200;
                info.timeout = 0.0f;
                _gSerial = _serial = atta::io::create<atta::io::Serial>(info);
                if (_serial)
                    _serial->start();
                LOG_DEBUG("ProjectScript", "Connected to [y]$0", deviceName);
                break;
            }
        }
    }
}

void ProjectScript::handleAttaConnector() {
    if (_serial)
        AttaConnector::update();
    MyTest0 t0;
    MyTest1 t1;
    MotorState state;
    while (AttaConnector::receive<MyTest0>(&t0))
        LOG_DEBUG("ProjectScript", "[w]Received MyTest0 -> $0 $1", (int)t0.u0, (int)t0.u1);
    while (AttaConnector::receive<MyTest1>(&t1))
        LOG_DEBUG("ProjectScript", "[w]Received MyTest1 -> $0 $1", t1.f, (int)t1.u);
    while (AttaConnector::receive<MotorState>(&state)) {
        _phyMotorTime.push_back(_phyMotorTime.empty() ? 0.0f : (_phyMotorTime.back() + 1.0f));
        _phyMotorData.batteryVoltage.push_back(state.batteryVoltage);
        _phyMotorData.currentUV.push_back(state.currentUV);
        _phyMotorData.currentW.push_back(state.currentW);
        _phyMotorData.rotorPosition.push_back(state.rotorPosition);
        LOG_DEBUG("ProjectScript", "[*y]Received Motor State");
        LOG_DEBUG("ProjectScript", "[w] - [*y]Battery voltage[y]: [w]$0", state.batteryVoltage);
        LOG_DEBUG("ProjectScript", "[w] - [*y]Current UV[y]: [w]$0", state.currentUV);
        LOG_DEBUG("ProjectScript", "[w] - [*y]Current W[y]: [w]$0", state.currentW);
        LOG_DEBUG("ProjectScript", "[w] - [*y]Rotor position[y]: [w]$0", state.rotorPosition);
    }
}

#include "attaConnectorPlatform.cpp"
