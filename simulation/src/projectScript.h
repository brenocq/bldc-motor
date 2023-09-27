//--------------------------------------------------
// BLDC Simulation
// projectScript.h
// Date: 2023-09-27
//--------------------------------------------------
#ifndef BLDC_PROJECT_SCRIPT_H
#define BLDC_PROJECT_SCRIPT_H
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
};

ATTA_REGISTER_PROJECT_SCRIPT(ProjectScript)

#endif // BLDC_PROJECT_SCRIPT_H
