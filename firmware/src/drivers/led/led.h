//--------------------------------------------------
// BLDC Motor Controller
// led.h
// Date: 2023-11-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_LED_LED_H
#define BLDC_DRIVERS_LED_LED_H
#include <drivers/gpio/gpio.h>

class Led {
  public:
    bool init();

    void setColor(uint8_t i, uint8_t r, uint8_t g, uint8_t b);
    void setColorAll(uint8_t r, uint8_t g, uint8_t b);
    bool show();

    static constexpr uint32_t NUM_LEDS = 16;

  private:
    static constexpr float SYMBOL_PERIOD = 1.2f;        // 1.2uF (800kHz)
    static constexpr float T0H = 0.3f;                  // Symbol 0 -> high period (uS)
    static constexpr float DUTY0 = T0H / SYMBOL_PERIOD; // Symbol 0 -> duty cycle
    static constexpr float T1H = 0.6f;                  // Symbol 1 -> high period (uS)
    static constexpr float DUTY1 = T1H / SYMBOL_PERIOD; // Symbol 1 -> duty cycle
    static constexpr float RST = 85.0f;                 // Reset period (at least 80uS)
    std::array<uint8_t, NUM_LEDS * 3> _colors;
    std::array<uint32_t, NUM_LEDS * 24 + size_t(RST / SYMBOL_PERIOD)> _pwm;
};

inline Led led;

#endif // BLDC_DRIVERS_LED_LED_H
