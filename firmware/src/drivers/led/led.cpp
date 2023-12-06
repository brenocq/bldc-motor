//--------------------------------------------------
// BLDC Motor Controller
// led.cpp
// Date: 2023-11-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/gpio/gpio.h>
#include <drivers/led/led.h>
#include <drivers/timer/timer.h>

bool Led::init() {
    setColorAll(0, 0, 0);
    show();
    return true;
}

void Led::setColor(uint8_t i, uint8_t r, uint8_t g, uint8_t b) {
    if (i < NUM_LEDS) {
        _colors[i * 3 + 0] = g;
        _colors[i * 3 + 1] = r;
        _colors[i * 3 + 2] = b;
    }
}

void Led::setColorAll(uint8_t r, uint8_t g, uint8_t b) {
    for (uint32_t i = 0; i < NUM_LEDS; i++)
        setColor(i, r, g, b);
}

bool Led::show() {
    // Write color bits
    for (size_t i = 0; i < _colors.size(); i++) {
        for (size_t b = 0; b < 8; b++) {
            bool high = (_colors[i] & (1 << (7 - b))) > 0;
            _pwm[i * 8 + b] = Timer::PERIOD * (high ? DUTY1 : DUTY0);
        }
    }

    // Reset code
    for (size_t i = NUM_LEDS * 24; i < _pwm.size(); i++)
        _pwm[i] = 0;

    Timer::startPwmDma(Timer::LED_TIM, Timer::LED_CH, _pwm.data(), _pwm.size());
    return true;
}
