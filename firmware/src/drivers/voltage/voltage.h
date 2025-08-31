//--------------------------------------------------
// BLDC Motor Controller
// voltage.h
// Date: 2023-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_VOLTAGE_VOLTAGE_H
#define BLDC_DRIVERS_VOLTAGE_VOLTAGE_H
#include <drivers/gpio/gpio.h>

class Voltage {
  public:
    bool init(Gpio::Gpio gpio);

    float read();

  private:
    // Vout = Vin*R2/(R1+R2)
    //
    // Vin---R1---v---R2---GND
    //            |
    //            |
    //           Vout
    static constexpr float R1 = 20.0f;
    static constexpr float R2 = 10.0f;

    Gpio::Gpio _gpio;
};

inline Voltage volt_src;
inline Voltage volt_u_phase;
inline Voltage volt_w_phase;
inline Voltage volt_v_phase;

#endif // BLDC_DRIVERS_VOLTAGE_VOLTAGE_H
