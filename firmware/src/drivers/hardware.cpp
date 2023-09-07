//--------------------------------------------------
// BLDC Motor Controller
// hardware.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/hardware.h>
#include <system/hal.h>

void Hardware::delay(float s) { HAL_Delay(s * 1000); }

void Hardware::delayMs(float ms) { HAL_Delay(ms); }
