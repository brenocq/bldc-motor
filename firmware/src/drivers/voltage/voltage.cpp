//--------------------------------------------------
// BLDC Motor Controller
// voltage.cpp
// Date: 2023-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/adc/adc.h>
#include <drivers/voltage/voltage.h>
#include <utils/log.h>

bool Voltage::init(Gpio::Gpio gpio) {
    _gpio = gpio;
    Log::success("Voltage", "Initialized for GPIO $0", gpio.toString());
    return true;
}

float Voltage::read() {
    static constexpr float multiplier = 3.33f * (R1 + R2) / R2;
    return (Adc::read(_gpio) / float(Adc::MAX_READ)) * multiplier;
}
