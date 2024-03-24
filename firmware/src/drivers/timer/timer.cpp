//--------------------------------------------------
// BLDC Motor Controller
// timer.cpp
// Date: 2023-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/timer/timer.h>
#include <utils/log.h>

namespace Timer {

/**
 * @brief Initialize PWM set to GPIO
 *
 * @warning For now the timer output can be set to only one GPIO
 *
 * @param timer Timer to configure
 * @param cfg Timer configuration
 *
 * @return True if success
 */
bool initPwm(Timer timer, TimerConfig cfg);

TIM_TypeDef* getInstance(Timer timer);
void enableClock(Timer timer);
void disableClock(Timer timer);

Timer gpioModeToTimer(Gpio::Mode mode);
Channel gpioModeToChannel(Gpio::Mode mode);

static constexpr uint16_t convert(CounterMode counterMode);

Handle hTIM1;
Handle hTIM2;
Handle hTIM3;
Handle hTIM4;
Handle hTIM5;
Handle hTIM6;
Handle hTIM7;
Handle hTIM8;
Handle hTIM9;
Handle hTIM10;
Handle hTIM11;
Handle hTIM12;
Handle hTIM13;
Handle hTIM14;

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
                if (!initPwm(cfg.timer, cfg))
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
    disableClock(TIM2);
    disableClock(TIM3);
    disableClock(TIM4);
    disableClock(TIM5);
    disableClock(TIM6);
    disableClock(TIM7);
    disableClock(TIM8);
    disableClock(TIM9);
    disableClock(TIM10);
    disableClock(TIM11);
    disableClock(TIM12);
    disableClock(TIM13);
    disableClock(TIM14);

    return true;
}

bool Timer::initPwm(Timer timer, TimerConfig cfg) {
    // Check if GPIO was initialized correctly
    std::vector<Channel> channels;
    for (Gpio::GpioConfig config : Gpio::gpioList) {
        if (gpioModeToTimer(config.mode) == timer) {
            Channel channel = gpioModeToChannel(config.mode);
            bool newChannel = true;
            for (size_t i = 0; i < channels.size(); i++)
                if (channels[i] == channel)
                    newChannel = false;
            if (newChannel)
                channels.push_back(channel);
        }
    }
    if (channels.empty()) {
        Log::error("Timer", "Trying to enable TIM$0 PWM, but no GPIO is using it", int(timer));
        return false;
    }

    // Enable clock
    enableClock(timer);

    // Initialize PWM
    TIM_HandleTypeDef* htim = getHandle(timer);
    htim->Instance = getInstance(timer);
    htim->Init.CounterMode = convert(cfg.counterMode);
    htim->Init.Prescaler = 0;
    htim->Init.Period = cfg.period;
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
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    if (timer == TIM1 || timer == TIM8) {
        sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
        sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
    }
    for (size_t i = 0; i < channels.size(); i++) {
        if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, (uint32_t)channels[i]) != HAL_OK)
            return false;
    }

    return true;
}

void Timer::setPwm(Timer timer, Channel channel, uint16_t ccr) {
    switch (channel) {
        case CH1:
            getInstance(timer)->CCR1 = ccr;
            return;
        case CH2:
            getInstance(timer)->CCR2 = ccr;
            return;
        case CH3:
            getInstance(timer)->CCR3 = ccr;
            return;
        case CH4:
            getInstance(timer)->CCR4 = ccr;
            return;
        default:
            break;
    }
    Log::error("Timer", "Could not set TIM$0 PWM", int(timer));
}

// clang-format off
#define LINK_DMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__) \
    do {                                                         \
        (__HANDLE__)->__PPP_DMA_FIELD__ = (__DMA_HANDLE__);     \
        (__DMA_HANDLE__)->Parent = (__HANDLE__);                  \
    } while (0U)
// clang-format on

void Timer::linkDma(Timer timer, Channel channel, Dma::Handle* dmaHandle) {
    switch (channel) {
        case CH1:
            LINK_DMA(getHandle(timer), hdma[TIM_DMA_ID_CC1], dmaHandle);
            return;
        case CH2:
            LINK_DMA(getHandle(timer), hdma[TIM_DMA_ID_CC2], dmaHandle);
            return;
        case CH3:
            LINK_DMA(getHandle(timer), hdma[TIM_DMA_ID_CC3], dmaHandle);
            return;
        case CH4:
            LINK_DMA(getHandle(timer), hdma[TIM_DMA_ID_CC4], dmaHandle);
            return;
        default:
            break;
    }
    Log::error("Timer", "Could not link DMA to TIM$0", int(timer));
}

void Timer::startPwm(Timer timer, Channel channel) {
    if (HAL_TIM_PWM_Start(getHandle(timer), channel) != HAL_OK)
        Log::error("Timer", "Failed to start TIM$0 PWM", int(timer));
    if (timer == TIM1 || timer == TIM8) {
        if (HAL_TIMEx_PWMN_Start(getHandle(timer), channel) != HAL_OK)
            Log::error("Timer", "Failed to start TIM$0 PWMN", int(timer));
    }
}

void Timer::startPwmDma(Timer timer, Channel channel, uint32_t* data, uint16_t size) {
    if (HAL_TIM_PWM_Start_DMA(getHandle(timer), channel, data, size) != HAL_OK)
        Log::error("Timer", "Failed to start TIM$0 PWM DMA", int(timer));
}

// clang-format off
#define CASE_GET_HANDLE(x) \
    case x: \
        return (TIM_HandleTypeDef*)&h##x; \
        break;
#define CASE_GET_INSTANCE(x) \
    case x: \
        return (TIM_TypeDef*)x##_BASE; \
        break;
#define CASE_ENABLE_CLK(x) \
    case x: \
        __HAL_RCC_##x##_CLK_ENABLE(); \
        break;
#define CASE_DISABLE_CLK(x) \
    case x: \
        __HAL_RCC_##x##_CLK_DISABLE(); \
        break;
// clang-format on

Timer::Handle* Timer::getHandle(Timer timer) {
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
        return TIM1;
    else if (mode >= Gpio::TIM2_CH1 && mode < Gpio::TIM3_CH1)
        return TIM2;
    else if (mode >= Gpio::TIM3_CH1 && mode < Gpio::TIM4_CH1)
        return TIM3;
    else if (mode >= Gpio::TIM4_CH1 && mode < Gpio::TIM5_CH1)
        return TIM4;
    else if (mode >= Gpio::TIM5_CH1 && mode < Gpio::TIM8_CH1)
        return TIM5;
    else if (mode >= Gpio::TIM8_CH1 && mode < Gpio::TIM9_CH1)
        return TIM8;
    else if (mode >= Gpio::TIM9_CH1 && mode < Gpio::TIM10_CH1)
        return TIM9;
    else if (mode >= Gpio::TIM10_CH1 && mode < Gpio::TIM11_CH1)
        return TIM10;
    else if (mode >= Gpio::TIM11_CH1 && mode < Gpio::TIM12_CH1)
        return TIM11;
    else if (mode >= Gpio::TIM12_CH1 && mode < Gpio::TIM13_CH1)
        return TIM12;
    else if (mode >= Gpio::TIM13_CH1 && mode < Gpio::TIM14_CH1)
        return TIM13;
    else if (mode == Gpio::TIM14_CH1)
        return TIM14;
    return TIM_NONE;
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
            return CH1;
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
            return CH2;
        case Gpio::TIM1_CH3:
        case Gpio::TIM1_CH3N:
        case Gpio::TIM2_CH3:
        case Gpio::TIM3_CH3:
        case Gpio::TIM4_CH3:
        case Gpio::TIM5_CH3:
        case Gpio::TIM8_CH3:
        case Gpio::TIM8_CH3N:
            return CH3;
        case Gpio::TIM1_CH4:
        case Gpio::TIM2_CH4:
        case Gpio::TIM3_CH4:
        case Gpio::TIM4_CH4:
        case Gpio::TIM5_CH4:
        case Gpio::TIM8_CH4:
            return CH4;
        default:
            return CH_NONE;
    }

    return CH_NONE;
}

constexpr uint16_t Timer::convert(CounterMode counterMode) {
    switch (counterMode) {
        case CounterMode::UP:
            return TIM_COUNTERMODE_UP;
        case CounterMode::DOWN:
            return TIM_COUNTERMODE_DOWN;
        case CounterMode::CENTER:
            return TIM_COUNTERMODE_CENTERALIGNED3;
    }
    return TIM_COUNTERMODE_UP;
}
