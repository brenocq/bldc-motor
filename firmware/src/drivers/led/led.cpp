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
    setColorAll(0, 255, 0);
    show();
    return true;
}

uint16_t pwmData[10];
void Led::setColor(uint8_t i, uint8_t r, uint8_t g, uint8_t b) {
    if (i < NUM_LEDS) {
        _colors[i * 3 + 0] = g;
        _colors[i * 3 + 1] = r;
        _colors[i * 3 + 2] = b;
    }

    for (int i = 0; i < 10; i++)
        pwmData[i] = 65535 * i / 9.0f;
    Timer::startPwmDma(Timer::LED_TIM, Timer::LED_CH, (uint32_t*)pwmData, 10);
    // static bool v = true;
    // Gpio::write(Gpio::LED_DI_PIN, v);
    // v = !v;
}

void Led::setColorAll(uint8_t r, uint8_t g, uint8_t b) {
    for (uint32_t i = 0; i < NUM_LEDS; i++)
        setColor(i, r, g, b);
}

bool Led::show() {
    // TODO using PWM+DMA
    return true;
}
