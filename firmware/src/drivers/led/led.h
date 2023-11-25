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

    static constexpr uint32_t NUM_LEDS = 2;

  private:
    static constexpr float SYMBOL_PERIOD = 1.2f; // 1.2uF (800kHz)
    static constexpr float T0H = 0.3f;           // Symbol 0 -> high period (uF)
    static constexpr float T0L = 0.9f;           // Symbol 0 -> low period (uF)
    static constexpr float T1H = 0.6f;           // Symbol 1 -> high period (uF)
    static constexpr float T1L = 0.6f;           // Symbol 1 -> low period (uF)
    static constexpr float RST = 85.0f;          // Reset period (at least 80uF)
    std::array<uint8_t, NUM_LEDS * 3> _colors;
};

inline Led led;

#endif // BLDC_DRIVERS_LED_LED_H
