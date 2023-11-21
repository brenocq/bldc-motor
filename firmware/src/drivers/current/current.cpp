//--------------------------------------------------
// BLDC Motor Controller
// current.cpp
// Date: 2023-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/adc/adc.h>
#include <drivers/current/current.h>

bool Current::init() {
    // Vdiff = 0.12Ohm * 0.1A * 100 = 1.2
    // Voltage is 1.65 when no current
    _gainW = _gainUV = GAIN_25;
    //setupGain(_gainW, Gpio::CURR_W_G1_PIN, Gpio::CURR_W_G0_PIN);
    //setupGain(_gainUV, Gpio::CURR_UV_G1_PIN, Gpio::CURR_UV_G0_PIN);
    return true;
}

float Current::readW() {
    return 0.0f;
    //float voltage = Adc::read(Gpio::CURR_W_PIN) / Adc::MAX_READ * 3.33f;
    //return calcCurrent(voltage, _gainW);
}

float Current::readUV() {
    return 0.0f;
    //float voltage = Adc::read(Gpio::CURR_UV_PIN) / Adc::MAX_READ * 3.33f;
    //return calcCurrent(voltage, _gainUV);
}

void Current::setupGain(Gain gain, Gpio::Gpio g1, Gpio::Gpio g0) {
    int g = (int)gain;
    Gpio::write(g1, (g & 0b10) != 0);
    Gpio::write(g0, (g & 0b01) != 0);
}

float Current::calcCurrent(float voltage, Gain gain) {
    static float convertGain[4] = {25.0f, 50.0f, 75.0f, 100.0f};
    float Vdiff = (voltage - Vref) / convertGain[(int)gain];
    return Vdiff / Rshunt;
}
