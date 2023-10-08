//--------------------------------------------------
// BLDC Simulation
// projectScript.h
// Date: 2023-09-27
//--------------------------------------------------
#ifndef BLDC_PROJECT_SCRIPT_H
#define BLDC_PROJECT_SCRIPT_H
#include "motor.h"
#include "trapezoidalController.h"
#include "attaConnector.h"
#include <atta/script/projectScript.h>
#include <atta/io/interface.h>

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
        std::vector<float> batteryVoltage;
        std::vector<float> currentUV;
        std::vector<float> currentW;
        std::vector<float> rotorPosition;
    };

    std::vector<float> _time;
    std::vector<float> _time3;
    std::vector<float> _phyMotorTime;
    Motor _motor;
    MotorData _motorData;
    PhysicalMotorData _phyMotorData;
    TrapezoidalController _tController;
    std::shared_ptr<atta::io::Serial> _serial;
};

ATTA_REGISTER_PROJECT_SCRIPT(ProjectScript)

#endif // BLDC_PROJECT_SCRIPT_H
