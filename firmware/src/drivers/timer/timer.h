//--------------------------------------------------
// BLDC Motor Controller
// timer.h
// Date: 2023-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_TIMER_TIMER_H
#define BLDC_DRIVERS_TIMER_TIMER_H
#include <cstdint>
#include <system/hal.h>

namespace Timer {

enum class Timer : uint8_t { NONE = 0, TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14 };
enum class Channel : uint32_t { NONE = 0xFFFFFFFF, CH1 = TIM_CHANNEL_1, CH2 = TIM_CHANNEL_2, CH3 = TIM_CHANNEL_3, CH4 = TIM_CHANNEL_4 };
enum class Mode {
    PWM = 0,       ///< Output pulse signal
    INTERRUPT,     ///< **Not supported** Generate period interrupts
    INPUT_CAPTURE, ///< **Not supported** Capture counter value when event happens
    INPUT_ENCODER, ///< **Not supported** Count number of interrupts
};

/**
 * @brief Initialize timer driver
 *
 * Initialize timer peripherals
 */
bool init();

/**
 * @brief Deinitialize timer driver
 *
 * Deinitialize timer peripherals
 */
bool deinit();

/**
 * @brief Set PWM duty cycle
 *
 * @param timer Timer
 * @param duty Duty cycle from 0.0f to 1.0f
 */
void setPwm(Timer timer, float duty);

//---------- Timers ----------//
static constexpr Timer LED = Timer::TIM2;

//---------- Timer configs ----------//
struct TimerConfig {
    Timer timer;
    Mode mode;
};

inline const std::array timerList{
    TimerConfig{LED, Mode::PWM},
};

}; // namespace Timer

#endif // BLDC_DRIVERS_TIMER_TIMER_H
