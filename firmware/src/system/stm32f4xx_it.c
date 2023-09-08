//--------------------------------------------------
// BLDC Motor Controller
// stm32f4xx_it.c
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "stm32f4xx_hal.h"
#include <system/stm32f4xx_it.h>
#include <utils/error.h>

void NMI_Handler(void) {
    while (1) {
    }
}

void HardFault_Handler(void) { Error::hardFault(); }

void MemManage_Handler(void) { Error::memFault(); }

void BusFault_Handler(void) { Error::busFault(); }

void UsageFault_Handler(void) { Error::usageFault(); }

void SVC_Handler(void) {}

void DebugMon_Handler(void) {}

void PendSV_Handler(void) {}

void SysTick_Handler(void) { HAL_IncTick(); }
