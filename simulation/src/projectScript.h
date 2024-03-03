//--------------------------------------------------
// BLDC Simulation
// projectScript.h
// Date: 2023-09-27
//--------------------------------------------------
#ifndef BLDC_PROJECT_SCRIPT_H
#define BLDC_PROJECT_SCRIPT_H
#include "motor.h"
// #include "trapezoidalController.h"
// #include "focController.h"
#include "attaConnector.h"
#include <atta/io/interface.h>
#include <atta/script/projectScript.h>

namespace scr = atta::script;

class ProjectScript : public scr::ProjectScript {
  public:
    void onLoad() override;

    //---------- Simulation ----------//
    void onStart() override;
    void onUpdateBefore(float dt) override;
    void onStop() override;

    //---------- UI ----------//
    void onUIRender() override;

    std::shared_ptr<atta::io::Serial> getSerial() const;

  private:
    void handleSerial();
    void handleAttaConnector();

    struct MotorData {
        std::vector<float> position;
        std::vector<float> velocity;
        std::vector<float> motorTorque;
        std::vector<float> loadTorque;
        std::vector<float> electromagneticTorque;
        std::vector<atta::vec3> current;
        std::vector<atta::vec3> voltage;
        std::vector<atta::vec3> emf;
    };

    struct PhysicalMotorData {
        std::vector<float> sourceVoltage;
        std::array<std::vector<float>, 3> phaseCurrent;
        std::array<std::vector<float>, 3> phaseVoltage;
        std::vector<float> rotorPosition;
    };

    struct ImuData {
        std::vector<atta::vec3> acc;
        std::vector<atta::vec3> gyr;
    };

    Motor _motor;
    MotorData _motorData;
    PhysicalMotorData _phyMotorData;
    ImuData _imuData;
    // TrapezoidalController _tController;
    // FocController _focController;
    std::shared_ptr<atta::io::Serial> _serial;
};

ATTA_REGISTER_PROJECT_SCRIPT(ProjectScript)

#endif // BLDC_PROJECT_SCRIPT_H
