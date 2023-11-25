//--------------------------------------------------
// BLDC Motor Controller
// led.cpp
// Date: 2023-11-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/led/led.h>

bool Led::init() {
    setColorAll(0, 255, 0);
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
    // TODO using PWM+DMA
    return true;
}
