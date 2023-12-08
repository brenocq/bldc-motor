//--------------------------------------------------
// BLDC Motor Controller
// hardware.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/hardware.h>
#include <system/hal.h>

void Hardware::delay(float s) { HAL_Delay(s * 1000); }

void Hardware::delayMs(uint32_t ms) { HAL_Delay(ms); }

void Hardware::delayUs(uint32_t us) {
    uint32_t clkCycleStart = DWT->CYCCNT;

    // Convert to number of cycles
    us *= (HAL_RCC_GetHCLKFreq() / 1000000);

    // Delay until end
    while ((DWT->CYCCNT - clkCycleStart) < us) {
    }
}
