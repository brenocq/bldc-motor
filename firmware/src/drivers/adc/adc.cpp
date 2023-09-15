//--------------------------------------------------
// BLDC Motor Controller
// adc.cpp
// Date: 2023-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/adc/adc.h>
#include <drivers/gpio/gpio.h>
#include <system/hal.h>

namespace Adc {

constexpr uint32_t timeout = 100; ///< Timeout 100ms

Peripheral getPeripheral(Gpio::Gpio gpio);
ADC_HandleTypeDef* getHandle(Peripheral peripheral);
ADC_TypeDef* getInstance(Peripheral peripheral);
void enableClock(Peripheral peripheral);
void disableClock(Peripheral peripheral);

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;

enum Channel : uint32_t {
    CH0 = ADC_CHANNEL_0,
    CH1 = ADC_CHANNEL_1,
    CH2 = ADC_CHANNEL_2,
    CH3 = ADC_CHANNEL_3,
    CH4 = ADC_CHANNEL_4,
    CH5 = ADC_CHANNEL_5,
    CH6 = ADC_CHANNEL_6,
    CH7 = ADC_CHANNEL_7,
    CH8 = ADC_CHANNEL_8,
    CH9 = ADC_CHANNEL_9,
    CH10 = ADC_CHANNEL_10,
    CH11 = ADC_CHANNEL_11,
    CH12 = ADC_CHANNEL_12,
    CH13 = ADC_CHANNEL_13,
    CH14 = ADC_CHANNEL_14,
    CH15 = ADC_CHANNEL_15,
    CH16 = ADC_CHANNEL_16,
    CH17 = ADC_CHANNEL_17,
    CH18 = ADC_CHANNEL_18,
    CH_INVALID,
};
// ADC additional functions
struct AdcAF {
    Gpio::Gpio gpio;
    Peripheral peripheral;
    Channel channel;
};

// clang-format off
static const std::array _adcAF{
    AdcAF{Gpio::PA0, Peripheral::ADC1, Channel::CH0},
    AdcAF{Gpio::PA0, Peripheral::ADC2, Channel::CH0},
    AdcAF{Gpio::PA0, Peripheral::ADC3, Channel::CH0},
    AdcAF{Gpio::PA1, Peripheral::ADC1, Channel::CH1},
    AdcAF{Gpio::PA1, Peripheral::ADC2, Channel::CH1},
    AdcAF{Gpio::PA1, Peripheral::ADC3, Channel::CH1},
    AdcAF{Gpio::PA2, Peripheral::ADC1, Channel::CH2},
    AdcAF{Gpio::PA2, Peripheral::ADC2, Channel::CH2},
    AdcAF{Gpio::PA2, Peripheral::ADC3, Channel::CH2},
    AdcAF{Gpio::PA3, Peripheral::ADC1, Channel::CH3},
    AdcAF{Gpio::PA3, Peripheral::ADC2, Channel::CH3},
    AdcAF{Gpio::PA3, Peripheral::ADC3, Channel::CH3},
    AdcAF{Gpio::PA4, Peripheral::ADC1, Channel::CH4},
    AdcAF{Gpio::PA4, Peripheral::ADC2, Channel::CH4},
    AdcAF{Gpio::PA5, Peripheral::ADC1, Channel::CH5},
    AdcAF{Gpio::PA5, Peripheral::ADC2, Channel::CH5},
    AdcAF{Gpio::PA6, Peripheral::ADC1, Channel::CH6},
    AdcAF{Gpio::PA6, Peripheral::ADC2, Channel::CH6},
    AdcAF{Gpio::PA7, Peripheral::ADC1, Channel::CH7},
    AdcAF{Gpio::PA7, Peripheral::ADC2, Channel::CH7},
    AdcAF{Gpio::PB0, Peripheral::ADC1, Channel::CH8},
    AdcAF{Gpio::PB0, Peripheral::ADC2, Channel::CH8},
    AdcAF{Gpio::PB1, Peripheral::ADC1, Channel::CH9},
    AdcAF{Gpio::PB1, Peripheral::ADC2, Channel::CH9},
    AdcAF{Gpio::PC0, Peripheral::ADC1, Channel::CH10},
    AdcAF{Gpio::PC0, Peripheral::ADC2, Channel::CH10},
    AdcAF{Gpio::PC0, Peripheral::ADC3, Channel::CH10},
    AdcAF{Gpio::PC1, Peripheral::ADC1, Channel::CH11},
    AdcAF{Gpio::PC1, Peripheral::ADC2, Channel::CH11},
    AdcAF{Gpio::PC1, Peripheral::ADC3, Channel::CH11},
    AdcAF{Gpio::PC2, Peripheral::ADC1, Channel::CH12},
    AdcAF{Gpio::PC2, Peripheral::ADC2, Channel::CH12},
    AdcAF{Gpio::PC2, Peripheral::ADC3, Channel::CH12},
    AdcAF{Gpio::PC3, Peripheral::ADC1, Channel::CH13},
    AdcAF{Gpio::PC3, Peripheral::ADC2, Channel::CH13},
    AdcAF{Gpio::PC3, Peripheral::ADC3, Channel::CH13},
    AdcAF{Gpio::PC4, Peripheral::ADC1, Channel::CH14},
    AdcAF{Gpio::PC4, Peripheral::ADC2, Channel::CH14},
    AdcAF{Gpio::PC5, Peripheral::ADC1, Channel::CH15},
    AdcAF{Gpio::PC5, Peripheral::ADC2, Channel::CH15}
};
// clang-format on

} // namespace Adc

bool Adc::init() {
    // Check for conflicts
    bool conflict = false;
    for (uint32_t i = 0; i < adcList.size(); i++)
        for (uint32_t j = i + 1; j < adcList.size(); j++)
            if (adcList[i].gpio == adcList[j].gpio || adcList[i].peripheral == adcList[j].peripheral)
                conflict = true;
    if (conflict)
        return false;

    // Initialize ADCs
    std::array<bool, 4> usedAdc = {false, false, false, false};
    for (const AdcConfig& adcConfig : adcList)
        usedAdc[int(adcConfig.peripheral)] = true;
    for (size_t i = 1; i < usedAdc.size(); i++) {
        if (usedAdc[i]) {
            enableClock(Peripheral(i));

            hadc1.Instance = getInstance(Peripheral(i));
            hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
            hadc1.Init.Resolution = ADC_RESOLUTION_12B;
            hadc1.Init.ScanConvMode = DISABLE;
            hadc1.Init.ContinuousConvMode = DISABLE;
            hadc1.Init.DiscontinuousConvMode = DISABLE;
            hadc1.Init.NbrOfDiscConversion = 0;
            hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
            hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
            hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
            hadc1.Init.NbrOfConversion = 1;
            hadc1.Init.DMAContinuousRequests = DISABLE;
            hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
            if (HAL_ADC_Init(&hadc1) != HAL_OK)
                return false;
        }
    }

    // Link ADCs to GPIO
    for (const AdcConfig& adcConfig : adcList) {
        // Check which channel
        Channel ch = CH_INVALID;
        for (AdcAF af : _adcAF) {
            if (af.gpio == adcConfig.gpio && af.peripheral == adcConfig.peripheral) {
                ch = af.channel;
                break;
            }
        }
        if (ch == CH_INVALID)
            return false;

        // Configure ADC channel
        ADC_ChannelConfTypeDef sConfig = {0};
        sConfig.Channel = uint32_t(ch);
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
        if (HAL_ADC_ConfigChannel(getHandle(adcConfig.peripheral), &sConfig) != HAL_OK)
            return false;
    }
    return true;
}

bool Adc::deinit() {
    HAL_ADC_DeInit(getHandle(Peripheral::ADC1));
    HAL_ADC_DeInit(getHandle(Peripheral::ADC2));
    HAL_ADC_DeInit(getHandle(Peripheral::ADC3));
    disableClock(Peripheral::ADC1);
    disableClock(Peripheral::ADC2);
    disableClock(Peripheral::ADC3);
    return true;
}

uint16_t Adc::read(Gpio::Gpio gpio) {
    auto handle = getHandle(getPeripheral(gpio));

    // Start ADC conversion
    if (HAL_ADC_Start(handle) != HAL_OK)
        return 0xFFFF;

    // Wait for the ADC conversion to complete
    if (HAL_ADC_PollForConversion(handle, timeout) != HAL_OK)
        return 0xFFFF;

    uint16_t value = HAL_ADC_GetValue(handle);

    // Stop ADC
    if (HAL_ADC_Stop(handle) != HAL_OK)
        return 0xFFFF;

    // Get
    return value;
}

Adc::Peripheral Adc::getPeripheral(Gpio::Gpio gpio) {
    for (size_t i = 0; i < adcList.size(); i++)
        if (adcList[i].gpio == gpio)
            return adcList[i].peripheral;
    return Peripheral::ADC1;
}

ADC_HandleTypeDef* Adc::getHandle(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::ADC1:
            return &hadc1;
            break;
        case Peripheral::ADC2:
            return &hadc2;
            break;
        case Peripheral::ADC3:
            return &hadc3;
            break;
    }
    return nullptr;
}

ADC_TypeDef* Adc::getInstance(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::ADC1:
            return (ADC_TypeDef*)ADC1_BASE;
            break;
        case Peripheral::ADC2:
            return (ADC_TypeDef*)ADC2_BASE;
            break;
        case Peripheral::ADC3:
            return (ADC_TypeDef*)ADC3_BASE;
            break;
    }
    return nullptr;
}

void Adc::enableClock(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::ADC1:
            __HAL_RCC_ADC1_CLK_ENABLE();
            break;
        case Peripheral::ADC2:
            __HAL_RCC_ADC2_CLK_ENABLE();
            break;
        case Peripheral::ADC3:
            __HAL_RCC_ADC3_CLK_ENABLE();
            break;
    }
}

void Adc::disableClock(Peripheral peripheral) {
    switch (peripheral) {
        case Peripheral::ADC1:
            __HAL_RCC_ADC1_CLK_DISABLE();
            break;
        case Peripheral::ADC2:
            __HAL_RCC_ADC2_CLK_DISABLE();
            break;
        case Peripheral::ADC3:
            __HAL_RCC_ADC3_CLK_DISABLE();
            break;
    }
}
