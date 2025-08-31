//--------------------------------------------------
// BLDC Motor Controller
// current.h
// Date: 2025-08-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_CURRENT_CURRENT_H
#define BLDC_DRIVERS_CURRENT_CURRENT_H
#include <drivers/gpio/gpio.h>

class Current {
  public:
    bool init(Gpio::Gpio gpio);

    float read();

  private:
    // Physical Constants
    static constexpr float AMPLIFIER_GAIN = 20.0f;    // From INA240A1 datasheet
    static constexpr float SHUNT_RESISTANCE = 0.050f; // Shunt resistor value in ohms
    static constexpr float ADC_VREF = 3.3f;           // ADC reference voltage

    Gpio::Gpio _gpio;
};

inline Current curr_u_phase;
inline Current curr_w_phase;
inline Current curr_v_phase;

#endif // BLDC_DRIVERS_CURRENT_CURRENT_H
