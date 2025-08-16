//--------------------------------------------------
// BLDC Motor Controller
// voltage.h
// Date: 2023-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/adc/adc.h>
#include <drivers/gpio/gpio.h>
#include <drivers/voltage/voltage.h>
#include <utils/log.h>

bool Voltage::init() {
    Log::success("Voltage", "Initialized");
    return true;
}

float Voltage::read() {
    return 0.0f;
    // float Vin = 3.33f * (Adc::read(Gpio::VOLT_PIN) / float(Adc::MAX_READ));
    // float Vout = Vin / R2 * (R1 + R2);
    // return Vout;
}
