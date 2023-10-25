//--------------------------------------------------
// BLDC Controller
// focController.h
// Date: 2023-10-25
//--------------------------------------------------
#ifndef BLDC_FOC_CONTROLLER_H
#define BLDC_FOC_CONTROLLER_H
#include "controller.h"

// Field Orientation Control Controller
class FocController : public Controller {
  public:
    FocController() = default;

    Output control(State s, Control c, float dt) override;

  private:
    /**
     * Vectors from space vector diagram
     *
     *    V3-010 *-----* V2-110
     *          / v     \
     *  V4-011 *    o  u * V1-100
     *          \ w     /
     *    V5-001 *-----* V6-101
     *
     *  NULL -> V7-000 v8-111
     */
    static const std::array<Output, 8> _v;

    /**
     * Space vector PWM
     *
     * Given the angle and magnitude, switch between UVW vectors to generate desired magnetic flux vector
     *
     * @param angle Vector angle in radians from space vector diagram (0 is U, 120 is V, 240 is W)
     * @param magnitude Control the magnetic field vector magnitude (between 0 and 1)
     *
     * @return Chosen vector to generate desired magnetic flux vector
     */
    Output spaceVectorPWM(float angle, float magnitude);
};

#endif // BLDC_FOC_CONTROLLER_H
