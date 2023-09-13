//--------------------------------------------------
// BLDC Motor Controller
// encoder.h
// Date: 2023-09-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_ENCODER_ENCODER_H
#define BLDC_DRIVERS_ENCODER_ENCODER_H

class Encoder {
  public:
    bool init();

    float readAngle();
};

inline Encoder encoder;

#endif // BLDC_DRIVERS_ENCODER_ENCODER_H
