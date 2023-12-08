//--------------------------------------------------
// BLDC Motor Controller
// hardware.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_HARDWARE_H
#define BLDC_DRIVERS_HARDWARE_H
#include <cstdint>

namespace Hardware {

void delay(float s);

void delayMs(uint32_t ms);

void delayUs(uint32_t us);

} // namespace Hardware

#endif // BLDC_DRIVERS_HARDWARE_H
