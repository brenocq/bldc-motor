//--------------------------------------------------
// BLDC Motor Controller
// timer.cpp
// Date: 2023-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/gpio/gpio.h>
#include <drivers/timer/timer.h>
#include <utils/log.h>

namespace Timer {

/**
 * @brief Initialize PWM set to GPIO
 *
 * @warning For now the timer output can be set to only one GPIO
 *
 * @param timer Timer to configure
 *
 * @return True if success
 */
bool initPwm(Timer timer);

TIM_HandleTypeDef* getHandle(Timer timer);
TIM_TypeDef* getInstance(Timer timer);
void enableClock(Timer timer);
void disableClock(Timer timer);

Timer gpioModeToTimer(Gpio::Mode mode);
Channel gpioModeToChannel(Gpio::Mode mode);

TIM_HandleTypeDef hTIM1;
TIM_HandleTypeDef hTIM2;
TIM_HandleTypeDef hTIM3;
TIM_HandleTypeDef hTIM4;
TIM_HandleTypeDef hTIM5;
TIM_HandleTypeDef hTIM6;
TIM_HandleTypeDef hTIM7;
TIM_HandleTypeDef hTIM8;
TIM_HandleTypeDef hTIM9;
TIM_HandleTypeDef hTIM10;
TIM_HandleTypeDef hTIM11;
TIM_HandleTypeDef hTIM12;
TIM_HandleTypeDef hTIM13;
TIM_HandleTypeDef hTIM14;

} // namespace Timer

bool Timer::init() {
    // Check for conflicts
    bool conflict = false;
    for (uint32_t i = 0; i < timerList.size(); i++)
        for (uint32_t j = i + 1; j < timerList.size(); j++)
            if (timerList[i].timer == timerList[j].timer)
                conflict = true;
    if (conflict)
        return false;

    // Initialize timers
    for (TimerConfig cfg : timerList) {
        switch (cfg.mode) {
            case Mode::PWM:
                if (!initPwm(cfg.timer))
                    return false;
                break;
            default:
                // Mode not implemented yet
                return false;
        }
    }

    Log::success("Timer", "Initialized");
    return true;
}

bool Timer::deinit() {
    disableClock(Timer::TIM2);
    disableClock(Timer::TIM3);
    disableClock(Timer::TIM4);
    disableClock(Timer::TIM5);
    disableClock(Timer::TIM6);
    disableClock(Timer::TIM7);
    disableClock(Timer::TIM8);
    disableClock(Timer::TIM9);
    disableClock(Timer::TIM10);
    disableClock(Timer::TIM11);
    disableClock(Timer::TIM12);
    disableClock(Timer::TIM13);
    disableClock(Timer::TIM14);

    return true;
}

void Timer::setPwm(Timer timer, float pwm) { getInstance(timer)->CCR1 = pwm * 65535; }

bool Timer::initPwm(Timer timer) {
    // Check if GPIO was initialized correctly
    Channel channel;
    bool found = false;
    for (Gpio::GpioConfig config : Gpio::gpioList) {
        if (gpioModeToTimer(config.mode) == timer) {
            channel = gpioModeToChannel(config.mode);
            found = true;
            break;
        }
    }
    if (!found || channel == Channel::NONE)
        return false;

    // Enable clock
    enableClock(timer);

    // Initialize PWM
    TIM_HandleTypeDef* htim = getHandle(timer);
    htim->Instance = getInstance(timer);
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Prescaler = 0;
    htim->Init.Period = 65535;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(htim) != HAL_OK)
        return false;

    TIM_MasterConfigTypeDef sMasterConfig{};
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) != HAL_OK)
        return false;

    TIM_OC_InitTypeDef sConfigOC{};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, (uint32_t)channel) != HAL_OK)
        return false;

    // Set no pulse
    setPwm(timer, 0.0f);

    // Start PWM
    if (HAL_TIM_PWM_Start(htim, (uint32_t)channel) != HAL_OK)
        return false;

    return true;
}

// clang-format off
#define CASE_GET_HANDLE(x) \
    case Timer::x: \
        return (TIM_HandleTypeDef*)&h##x; \
        break;
#define CASE_GET_INSTANCE(x) \
    case Timer::x: \
        return (TIM_TypeDef*)x##_BASE; \
        break;
#define CASE_ENABLE_CLK(x) \
    case Timer::x: \
        __HAL_RCC_##x##_CLK_ENABLE(); \
        break;
#define CASE_DISABLE_CLK(x) \
    case Timer::x: \
        __HAL_RCC_##x##_CLK_DISABLE(); \
        break;
// clang-format on

TIM_HandleTypeDef* Timer::getHandle(Timer timer) {
    switch (timer) {
        CASE_GET_HANDLE(TIM1)
        CASE_GET_HANDLE(TIM2)
        CASE_GET_HANDLE(TIM3)
        CASE_GET_HANDLE(TIM4)
        CASE_GET_HANDLE(TIM5)
        CASE_GET_HANDLE(TIM6)
        CASE_GET_HANDLE(TIM7)
        CASE_GET_HANDLE(TIM8)
        CASE_GET_HANDLE(TIM9)
        CASE_GET_HANDLE(TIM10)
        CASE_GET_HANDLE(TIM11)
        CASE_GET_HANDLE(TIM12)
        CASE_GET_HANDLE(TIM13)
        CASE_GET_HANDLE(TIM14)
        default:
            break;
    }
    return nullptr;
}

TIM_TypeDef* Timer::getInstance(Timer timer) {
    switch (timer) {
        CASE_GET_INSTANCE(TIM1)
        CASE_GET_INSTANCE(TIM2)
        CASE_GET_INSTANCE(TIM3)
        CASE_GET_INSTANCE(TIM4)
        CASE_GET_INSTANCE(TIM5)
        CASE_GET_INSTANCE(TIM6)
        CASE_GET_INSTANCE(TIM7)
        CASE_GET_INSTANCE(TIM8)
        CASE_GET_INSTANCE(TIM9)
        CASE_GET_INSTANCE(TIM10)
        CASE_GET_INSTANCE(TIM11)
        CASE_GET_INSTANCE(TIM12)
        CASE_GET_INSTANCE(TIM13)
        CASE_GET_INSTANCE(TIM14)
        default:
            break;
    }
    return nullptr;
}

void Timer::enableClock(Timer timer) {
    switch (timer) {
        CASE_ENABLE_CLK(TIM1)
        CASE_ENABLE_CLK(TIM2)
        CASE_ENABLE_CLK(TIM3)
        CASE_ENABLE_CLK(TIM4)
        CASE_ENABLE_CLK(TIM5)
        CASE_ENABLE_CLK(TIM6)
        CASE_ENABLE_CLK(TIM7)
        CASE_ENABLE_CLK(TIM8)
        CASE_ENABLE_CLK(TIM9)
        CASE_ENABLE_CLK(TIM10)
        CASE_ENABLE_CLK(TIM11)
        CASE_ENABLE_CLK(TIM12)
        CASE_ENABLE_CLK(TIM13)
        CASE_ENABLE_CLK(TIM14)
        default:
            break;
    }
}

void Timer::disableClock(Timer timer) {
    switch (timer) {
        CASE_DISABLE_CLK(TIM1)
        CASE_DISABLE_CLK(TIM2)
        CASE_DISABLE_CLK(TIM3)
        CASE_DISABLE_CLK(TIM4)
        CASE_DISABLE_CLK(TIM5)
        CASE_DISABLE_CLK(TIM6)
        CASE_DISABLE_CLK(TIM7)
        CASE_DISABLE_CLK(TIM8)
        CASE_DISABLE_CLK(TIM9)
        CASE_DISABLE_CLK(TIM10)
        CASE_DISABLE_CLK(TIM11)
        CASE_DISABLE_CLK(TIM12)
        CASE_DISABLE_CLK(TIM13)
        CASE_DISABLE_CLK(TIM14)
        default:
            break;
    }
}

Timer::Timer Timer::gpioModeToTimer(Gpio::Mode mode) {
    if (mode >= Gpio::TIM1_CH1 && mode < Gpio::TIM2_CH1)
        return Timer::TIM1;
    else if (mode >= Gpio::TIM2_CH1 && mode < Gpio::TIM3_CH1)
        return Timer::TIM2;
    else if (mode >= Gpio::TIM3_CH1 && mode < Gpio::TIM4_CH1)
        return Timer::TIM3;
    else if (mode >= Gpio::TIM4_CH1 && mode < Gpio::TIM5_CH1)
        return Timer::TIM4;
    else if (mode >= Gpio::TIM5_CH1 && mode < Gpio::TIM8_CH1)
        return Timer::TIM5;
    else if (mode >= Gpio::TIM8_CH1 && mode < Gpio::TIM9_CH1)
        return Timer::TIM8;
    else if (mode >= Gpio::TIM9_CH1 && mode < Gpio::TIM10_CH1)
        return Timer::TIM9;
    else if (mode >= Gpio::TIM10_CH1 && mode < Gpio::TIM11_CH1)
        return Timer::TIM10;
    else if (mode >= Gpio::TIM11_CH1 && mode < Gpio::TIM12_CH1)
        return Timer::TIM11;
    else if (mode >= Gpio::TIM12_CH1 && mode < Gpio::TIM13_CH1)
        return Timer::TIM12;
    else if (mode >= Gpio::TIM13_CH1 && mode < Gpio::TIM14_CH1)
        return Timer::TIM13;
    else if (mode == Gpio::TIM14_CH1)
        return Timer::TIM14;
    return Timer::NONE;
}

Timer::Channel Timer::gpioModeToChannel(Gpio::Mode mode) {
    switch (mode) {
        case Gpio::TIM1_CH1:
        case Gpio::TIM1_CH1N:
        case Gpio::TIM2_CH1:
        case Gpio::TIM3_CH1:
        case Gpio::TIM4_CH1:
        case Gpio::TIM5_CH1:
        case Gpio::TIM8_CH1:
        case Gpio::TIM8_CH1N:
        case Gpio::TIM9_CH1:
        case Gpio::TIM10_CH1:
        case Gpio::TIM11_CH1:
        case Gpio::TIM12_CH1:
        case Gpio::TIM13_CH1:
        case Gpio::TIM14_CH1:
            return Channel::CH1;
        case Gpio::TIM1_CH2:
        case Gpio::TIM1_CH2N:
        case Gpio::TIM2_CH2:
        case Gpio::TIM3_CH2:
        case Gpio::TIM4_CH2:
        case Gpio::TIM5_CH2:
        case Gpio::TIM8_CH2:
        case Gpio::TIM8_CH2N:
        case Gpio::TIM9_CH2:
        case Gpio::TIM12_CH2:
            return Channel::CH2;
        case Gpio::TIM1_CH3:
        case Gpio::TIM1_CH3N:
        case Gpio::TIM2_CH3:
        case Gpio::TIM3_CH3:
        case Gpio::TIM4_CH3:
        case Gpio::TIM5_CH3:
        case Gpio::TIM8_CH3:
        case Gpio::TIM8_CH3N:
            return Channel::CH3;
        case Gpio::TIM1_CH4:
        case Gpio::TIM2_CH4:
        case Gpio::TIM3_CH4:
        case Gpio::TIM4_CH4:
        case Gpio::TIM5_CH4:
        case Gpio::TIM8_CH4:
            return Channel::CH4;
        default:
            return Channel::NONE;
    }

    return Channel::NONE;
}
