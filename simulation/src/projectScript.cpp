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
    float R = 3.6f; // Phase resistance
    float L = 0.1f; // Phase inductance (TODO)
    float J = 0.1f; // Rotor inertia (TODO)
    float F = 1.0f; // Rotor friction (TODO)
    float P = 14;   // Number of poles
    float l = 1.0f; // Flux linkage (TODO)
    _motor = Motor(R, L, J, F, P, l);
    //_tController = TrapezoidalController();
    //_focController = FocController();

    _phyMotorData = {};
    _imuData = {};
}

void ProjectScript::onStop() {}

void ProjectScript::onUpdateBefore(float dt) {
    // Handle motor serial connection
    handleSerial();
    handleAttaConnector();

    // Update controller
    // Controller::Control u;
    // u.position = NAN;
    // u.velocity = NAN;
    // u.torque = NAN;
    // Controller::State x;
    // x.voltage = 7.8f;
    // x.currents = {_motor.getCurrent()[0], _motor.getCurrent()[1], _motor.getCurrent()[2]};
    // x.theta = _motor.getPosition();
    //// Controller::Output output = _tController.control(x, u, dt);
    // Controller::Output output = _focController.control(x, u, dt);

    // Update motor
    // float V = 7.8f;  // Voltage
    // float Tl = 0.1f; // Load torque
    // auto calcV = [V](bool h, bool l) {
    //    if (h && !l)
    //        return V;
    //    else if (!h && l)
    //        return 0.0f;
    //    else
    //        return -1.0f;
    //};
    // atta::vec3 Vs = {calcV(output.uh, output.ul), calcV(output.vh, output.vl), calcV(output.wh, output.wl)};
    //_motor.update(Vs, Tl, dt);
    //_motorData.position.push_back(_motor.getPosition());
    //_motorData.velocity.push_back(_motor.getVelocity());
    //_motorData.motorTorque.push_back(_motor.getMotorTorque());
    //_motorData.loadTorque.push_back(_motor.getLoadTorque());
    //_motorData.electromagneticTorque.push_back(_motor.getElectromagneticTorque());
    //_motorData.current.push_back(_motor.getCurrent());
    //_motorData.voltage.push_back(_motor.getVoltage());
    //_motorData.emf.push_back(_motor.getEMF());
    // motorEntity.get<cmp::Transform>()->orientation.set2DAngle(_motorData.position.back());
}

void ProjectScript::onUIRender() {
    // ImGui::Begin("Simulation");
    //{
    //     if (ImPlot::BeginPlot("Phase voltages")) {
    //         ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
    //         ImPlot::PlotLine("v_u", (float*)_motorData.voltage.data(), _motorData.voltage.size(), 0, 1, 0 * sizeof(float), sizeof(atta::vec3));
    //         ImPlot::PlotLine("v_v", (float*)_motorData.voltage.data(), _motorData.voltage.size(), 0, 1, 1 * sizeof(float), sizeof(atta::vec3));
    //         ImPlot::PlotLine("v_w", (float*)_motorData.voltage.data(), _motorData.voltage.size(), 0, 1, 2 * sizeof(float), sizeof(atta::vec3));
    //         ImPlot::EndPlot();
    //     }

    //    if (ImPlot::BeginPlot("Phase currents")) {
    //        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
    //        ImPlot::PlotLine("i_u", (float*)_motorData.current.data(), _motorData.current.size(), 0, 1, 0 * sizeof(float), sizeof(atta::vec3));
    //        ImPlot::PlotLine("i_v", (float*)_motorData.current.data(), _motorData.current.size(), 0, 1, 1 * sizeof(float), sizeof(atta::vec3));
    //        ImPlot::PlotLine("i_w", (float*)_motorData.current.data(), _motorData.current.size(), 0, 1, 2 * sizeof(float), sizeof(atta::vec3));
    //        ImPlot::EndPlot();
    //    }

    //    if (ImPlot::BeginPlot("Torque")) {
    //        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
    //        ImPlot::PlotLine("Load torque", _time.data(), _motorData.loadTorque.data(), _time.size());
    //        ImPlot::PlotLine("Electromagnetic torque", _time.data(), _motorData.electromagneticTorque.data(), _time.size());
    //        ImPlot::PlotLine("Motor torque", _time.data(), _motorData.motorTorque.data(), _time.size());
    //        ImPlot::EndPlot();
    //    }

    //    if (ImPlot::BeginPlot("Rotor velocity")) {
    //        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
    //        ImPlot::PlotLine("Angular velocity", _time.data(), _motorData.velocity.data(), _time.size());
    //        ImPlot::EndPlot();
    //    }

    //    if (ImPlot::BeginPlot("Rotor position")) {
    //        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
    //        ImPlot::PlotLine("Angular position", _time.data(), _motorData.position.data(), _time.size());
    //        ImPlot::EndPlot();
    //    }

    //    if (ImPlot::BeginPlot("Back EMF")) {
    //        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
    //        ImPlot::PlotLine("e_u", (float*)_motorData.emf.data(), _motorData.emf.size(), 0 * sizeof(float), sizeof(atta::vec3));
    //        ImPlot::PlotLine("e_v", (float*)_motorData.emf.data(), _motorData.emf.size(), 1 * sizeof(float), sizeof(atta::vec3));
    //        ImPlot::PlotLine("e_w", (float*)_motorData.emf.data(), _motorData.emf.size(), 2 * sizeof(float), sizeof(atta::vec3));
    //        ImPlot::EndPlot();
    //    }
    //}
    // ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once);
    ImGui::Begin("Motor State");
    {
        if (ImPlot::BeginPlot("Source Voltage")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Source Voltage", _phyMotorData.sourceVoltage.data(), _phyMotorData.sourceVoltage.size());
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Phase Voltages")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            size_t size = _phyMotorData.phaseVoltage[0].size();
            ImPlot::PlotLine("U", (float*)_phyMotorData.phaseVoltage[0].data(), size);
            ImPlot::PlotLine("V", (float*)_phyMotorData.phaseVoltage[1].data(), size);
            ImPlot::PlotLine("W", (float*)_phyMotorData.phaseVoltage[2].data(), size);
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Phase Currents")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            size_t size = _phyMotorData.phaseCurrent[0].size();
            ImPlot::PlotLine("U", (float*)_phyMotorData.phaseCurrent[0].data(), size);
            ImPlot::PlotLine("V", (float*)_phyMotorData.phaseCurrent[1].data(), size);
            ImPlot::PlotLine("W", (float*)_phyMotorData.phaseCurrent[2].data(), size);
            ImPlot::EndPlot();
        }

        std::vector<float> ia;
        std::vector<float> ib;
        for (size_t i = 0; i < _phyMotorData.phaseCurrent[0].size(); i++) {
            float u = _phyMotorData.phaseCurrent[0][i];
            float v = _phyMotorData.phaseCurrent[1][i];
            float w = _phyMotorData.phaseCurrent[2][i];
            ia.push_back(2 / 3.0f * (u - 0.5 * v - 0.5 * w));
            ib.push_back(2 / 3.0f * (std::sqrt(3) * 0.5 * v - std::sqrt(3) * 0.5 * w));
        }

        if (ImPlot::BeginPlot("Phase Currents (Clarke)")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Alpha", (float*)ia.data(), ia.size());
            ImPlot::PlotLine("Beta", (float*)ib.data(), ib.size());
            ImPlot::EndPlot();
        }

        std::vector<float> id;
        std::vector<float> iq;
        for (size_t i = 0; i < ia.size(); i++) {
            // Convert from rotor angle to electrical angle
            float angle = _phyMotorData.rotorPosition[i] - 0.222;
            if (angle < 0.0f)
                angle += 2 * M_PI;
            while (angle >= 2 * M_PI / 7)
                angle -= 2 * M_PI / 7;
            angle *= 7;
            LOG_DEBUG("ProjectScript", "Angle $0", angle / M_PI * 180);
            // Calculate d/q
            id.push_back(cos(angle) * ia[i] + sin(angle) * ib[i]);
            iq.push_back(-sin(angle) * ia[i] + cos(angle) * ib[i]);
        }

        if (ImPlot::BeginPlot("Phase Currents (Park)")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Direct", (float*)id.data(), id.size());
            ImPlot::PlotLine("Quadrature", (float*)iq.data(), iq.size());
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Rotor Position")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Rotor Position", _phyMotorData.rotorPosition.data(), _phyMotorData.rotorPosition.size());
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("IMU accelerometer")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("X", &_imuData.acc[0][0], _imuData.acc.size(), 1, 0, 0, sizeof(atta::vec3));
            ImPlot::PlotLine("Y", &_imuData.acc[0][1], _imuData.acc.size(), 1, 0, 0, sizeof(atta::vec3));
            ImPlot::PlotLine("Z", &_imuData.acc[0][2], _imuData.acc.size(), 1, 0, 0, sizeof(atta::vec3));
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("IMU gyroscope")) {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("X", &_imuData.gyr[0][0], _imuData.gyr.size(), 1, 0, 0, sizeof(atta::vec3));
            ImPlot::PlotLine("Y", &_imuData.gyr[0][1], _imuData.gyr.size(), 1, 0, 0, sizeof(atta::vec3));
            ImPlot::PlotLine("Z", &_imuData.gyr[0][2], _imuData.gyr.size(), 1, 0, 0, sizeof(atta::vec3));
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

    MotorState state;
    while (AttaConnector::receive<MotorState>(&state)) {
        _phyMotorData.sourceVoltage.push_back(state.sourceVoltage);
        for (size_t i = 0; i < 3; i++) {
            _phyMotorData.phaseCurrent[i].push_back(state.phaseCurrent[i]);
            _phyMotorData.phaseVoltage[i].push_back(state.phaseVoltage[i]);
        }
        _phyMotorData.rotorPosition.push_back(state.rotorPosition);
        // LOG_DEBUG("ProjectScript", "[*y]Received Motor State");
        // LOG_DEBUG("ProjectScript", "[w] - [*y]Battery voltage[y]: [w]$0", state.sourceVoltage);
        // LOG_DEBUG("ProjectScript", "[w] - [*y]Current[y]: [w]$0 $1 $2", state.phaseCurrent[0], state.phaseCurrent[1], state.phaseCurrent[2]);
        // LOG_DEBUG("ProjectScript", "[w] - [*y]Voltage[y]: [w]$0 $1 $2", state.phaseVoltage[0], state.phaseVoltage[1], state.phaseVoltage[2]);
        // LOG_DEBUG("ProjectScript", "[w] - [*y]Rotor position[y]: [w]$0", state.rotorPosition);
    }

    ImuState imu;
    while (AttaConnector::receive<ImuState>(&imu)) {
        _imuData.acc.push_back(atta::vec3(imu.acc[0], imu.acc[1], imu.acc[2]));
        _imuData.gyr.push_back(atta::vec3(imu.gyr[0], imu.gyr[1], imu.gyr[2]));
    }
}

#include "attaConnectorPlatform.cpp"
