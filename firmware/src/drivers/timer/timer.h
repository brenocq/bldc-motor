//--------------------------------------------------
// BLDC Motor Controller
// timer.h
// Date: 2023-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_TIMER_TIMER_H
#define BLDC_DRIVERS_TIMER_TIMER_H
#include <cstdint>
#include <drivers/dma/dma.h>
#include <system/hal.h>

namespace Timer {

enum Timer : uint8_t { TIM_NONE = 0, TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14 };
enum Channel : uint32_t { CH_NONE = 0xFFFFFFFF, CH1 = TIM_CHANNEL_1, CH2 = TIM_CHANNEL_2, CH3 = TIM_CHANNEL_3, CH4 = TIM_CHANNEL_4 };
enum class Mode {
    PWM = 0,       ///< Output pulse signal
    INTERRUPT,     ///< **Not supported** Generate period interrupts
    INPUT_CAPTURE, ///< **Not supported** Capture counter value when event happens
    INPUT_ENCODER, ///< **Not supported** Count number of interrupts
};
enum class CounterMode {
    UP = 0, ///< Count up
    DOWN,   ///< Count down
    CENTER, ///< Count up/down (center aligned 3)
};
using Handle = TIM_HandleTypeDef;
constexpr uint16_t PERIOD = 112;

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
 * @param channel Timer channel
 * @param ccr Capture/compare register value
 */
void setPwm(Timer timer, Channel channel, uint16_t ccr);

/**
 * @brief Get timer handle
 *
 * @param timer Timer
 *
 * @return Timer handle
 */
Handle* getHandle(Timer timer);

/**
 * @brief Link timer to DMA
 *
 * @param timer Timer
 * @param channel Timer channel
 * @param dmaHandle DMA Handle
 */
void linkDma(Timer timer, Channel channel, Dma::Handle* dmaHandle);

/**
 * @brief Start PWM
 *
 * @param timer Timer
 * @param channel Timer channel
 */
void startPwm(Timer timer, Channel channel);

/**
 * @brief Start PWM with DMA transaction
 *
 * @param timer Timer
 * @param channel Timer channel
 * @param data Data to be copied to TIM peripheral
 * @param size Number to transmit (byte/halfword/word depending on DMA configuration)
 */
void startPwmDma(Timer timer, Channel channel, uint32_t* data, uint16_t size);

//---------- Timers ----------//
static constexpr Timer LED_TIM = TIM2;
static constexpr uint16_t LED_PERIOD = 112;
static constexpr Channel LED_CH = CH1;

static constexpr Timer MOTOR_TIM = TIM1;
static constexpr uint16_t MOTOR_PERIOD = 3000 - 1;
static constexpr Channel MOTOR_CH_U = CH1;
static constexpr Channel MOTOR_CH_V = CH2;
static constexpr Channel MOTOR_CH_W = CH3;

//---------- Timer configs ----------//
struct TimerConfig {
    Timer timer;
    Mode mode;
    CounterMode counterMode;
    uint16_t period;
};

inline const std::array timerList{
    TimerConfig{LED_TIM, Mode::PWM, CounterMode::UP, LED_PERIOD},
    // TimerConfig{MOTOR_TIM, Mode::PWM, CounterMode::CENTER, MOTOR_PERIOD},
};

}; // namespace Timer

#endif // BLDC_DRIVERS_TIMER_TIMER_H
