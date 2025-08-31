//--------------------------------------------------
// BLDC Motor Controller
// current.cpp
// Date: 2025-08-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/adc/adc.h>
#include <drivers/current/current.h>
#include <utils/log.h>

bool Current::init(Gpio::Gpio gpio) {
    _gpio = gpio;
    Log::success("Current", "Initialized for GPIO $0", gpio.toString());
    return true;
}

float Current::read() {
    // Read the raw 12-bit value from the ADC pin
    uint16_t rawValue = Adc::read(_gpio);

    if (rawValue == 0xFFFF)
        return -1.0f; // Return error value

    // Convert raw ADC value to the voltage at the amplifier's output pin
    float vOut = (static_cast<float>(rawValue) / Adc::MAX_READ) * ADC_VREF;

    // For bidirectional mode, subtract the reference offset (Vref/2).
    // 0A of current corresponds to an output voltage of Vref/2.
    float vOutSigned = vOut - (ADC_VREF / 2.0f);

    // Convert the signed amplifier output voltage to the voltage across the shunt resistor
    float vShunt = vOutSigned / AMPLIFIER_GAIN;

    // Use Ohm's Law to calculate the current
    float current = vShunt / SHUNT_RESISTANCE;

    return current;
}
