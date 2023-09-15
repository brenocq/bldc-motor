//--------------------------------------------------
// BLDC Motor Controller
// voltage.h
// Date: 2023-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_VOLTAGE_VOLTAGE_H
#define BLDC_DRIVERS_VOLTAGE_VOLTAGE_H

class Voltage {
  public:
    bool init();

    float read();

  private:
    // Vout = Vin*R2/(R1+R2)
    //
    // Vin---R1---v---R2---GND
    //            |
    //            |
    //           Vout
    static constexpr float R1 = 200.0f;
    static constexpr float R2 = 100.0f;
};

inline Voltage voltage;

#endif // BLDC_DRIVERS_VOLTAGE_VOLTAGE_H
