//--------------------------------------------------
// BLDC Motor Controller
// adc.h
// Date: 2023-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_ADC_ADC_H
#define BLDC_DRIVERS_ADC_ADC_H
#include <drivers/gpio/gpio.h>

namespace Adc {

bool init();

bool deinit();

uint16_t read(Gpio::Gpio gpio);

constexpr float MAX_READ = 4095;

//---------- ADC configs ----------//
enum class Peripheral : uint8_t { ADC1 = 1, ADC2, ADC3 };

struct AdcConfig {
    Gpio::Gpio gpio;
    Peripheral peripheral;
};

// clang-format off
inline const std::array adcList{
    // Voltage sense
    AdcConfig{Gpio::VOLT_SRC_PIN, Peripheral::ADC1},
    AdcConfig{Gpio::VOLT_U_PIN, Peripheral::ADC1},
    AdcConfig{Gpio::VOLT_V_PIN, Peripheral::ADC1},
    AdcConfig{Gpio::VOLT_W_PIN, Peripheral::ADC1},
    // Current sense
    AdcConfig{Gpio::CURR_U_PIN, Peripheral::ADC2},
    AdcConfig{Gpio::CURR_V_PIN, Peripheral::ADC2},
    AdcConfig{Gpio::CURR_W_PIN, Peripheral::ADC2},
};
// clang-format on

} // namespace Adc

#endif // BLDC_DRIVERS_ADC_ADC_H
