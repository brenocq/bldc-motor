//--------------------------------------------------
// BLDC Motor Controller
// interrupt.cpp
// Date: 2023-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/dma/dma.h>
#include <drivers/interrupt/interrupt.h>
#include <drivers/timer/timer.h>
#include <system/hal.h>
#include <utils/error.h>
#include <utils/log.h>

bool Interrupt::init() {
    // TIM2
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    // DMA
    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    Log::success("Interrupt", "Initialized");
    return true;
}

extern "C" {
//----- System interrupts -----//
void NMI_Handler() {
    while (1) {
    }
}
void HardFault_Handler() { Error::hardFault(); }
void MemManage_Handler() { Error::memFault(); }
void BusFault_Handler() { Error::busFault(); }
void UsageFault_Handler() { Error::usageFault(); }
void SVC_Handler() {}
void DebugMon_Handler() {}
void PendSV_Handler() {}
void SysTick_Handler() { HAL_IncTick(); }

//----- Peripheral interrupts -----//
void TIM2_IRQHandler() { HAL_TIM_IRQHandler(Timer::getHandle(Timer::TIM2)); }
void DMA1_Stream5_IRQHandler() { HAL_DMA_IRQHandler(Dma::getHandle(Dma::DMA1, Dma::STREAM5)); }
}
