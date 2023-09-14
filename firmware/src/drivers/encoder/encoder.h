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

  private:
    // clang-format off
    enum Status {
        STATUS_MAG_FIELD_TOO_STRONG = 0b0001,
        STATUS_MAG_FIELD_TOO_WEAK   = 0b0010,
        STATUS_PUSH_BUTTON_DETECTED = 0b0100,
        STATUS_LOSS_OF_TRACK        = 0b1000
    };
    // clang-format on
};

inline Encoder encoder;

#endif // BLDC_DRIVERS_ENCODER_ENCODER_H
