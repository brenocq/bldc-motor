//--------------------------------------------------
// BLDC Motor Controller
// current.h
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_CURRENT_CURRENT_H
#define BLDC_DRIVERS_CURRENT_CURRENT_H
#include <drivers/gpio/gpio.h>

class Current {
  public:
    bool init();

    float readW();
    float readUV();

  private:
    enum Gain { GAIN_25 = 0b00, GAIN_50 = 0b01, GAIN_75 = 0b10, GAIN_100 = 0b11 };

    void setupGain(Gain gain, Gpio::Gpio g1, Gpio::Gpio g0);
    float calcCurrent(float voltage, Gain gain);

    Gain _gainW;
    Gain _gainUV;
    static constexpr float Rshunt = 0.12f;      // Shunt resistor resistance
    static constexpr float Vref = 3.33f * 0.5f; // Voltage reference (voltage when current is zero)
};

inline Current current;

#endif // BLDC_DRIVERS_CURRENT_CURRENT_H
