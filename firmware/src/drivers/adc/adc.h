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

constexpr uint16_t MAX_READ = 4095;

//---------- ADC configs ----------//
enum class Peripheral : uint8_t { ADC1 = 1, ADC2, ADC3 };

struct AdcConfig {
    Gpio::Gpio gpio;
    Peripheral peripheral;
};

inline const std::array adcList{
    AdcConfig{Gpio::VOLT_PIN, Peripheral::ADC1},
};

} // namespace Adc

#endif // BLDC_DRIVERS_ADC_ADC_H
