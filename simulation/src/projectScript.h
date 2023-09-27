//--------------------------------------------------
// BLDC Simulation
// projectScript.h
// Date: 2023-09-27
//--------------------------------------------------
#ifndef BLDC_PROJECT_SCRIPT_H
#define BLDC_PROJECT_SCRIPT_H
#include "motor.h"
#include <atta/script/projectScript.h>

namespace scr = atta::script;

class ProjectScript : public scr::ProjectScript {
  public:
    //---------- Simulation ----------//
    void onStart() override;
    void onUpdateBefore(float dt) override;
    void onStop() override;

    //---------- UI ----------//
    void onUIRender() override;

  private:
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

    std::vector<float> _time;
    std::vector<float> _time3;
    Motor _motor;
    MotorData _motorData;
};

ATTA_REGISTER_PROJECT_SCRIPT(ProjectScript)

#endif // BLDC_PROJECT_SCRIPT_H
