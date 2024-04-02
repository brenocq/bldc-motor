//--------------------------------------------------
// BLDC Motor Controller
// interrupt.cpp
// Date: 2023-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/dma/dma.h>
#include <drivers/interrupt/interrupt.h>
#include <drivers/timer/timer.h>
#include <drivers/uart/uart.h>
#include <drivers/usb/usb.h>
#include <system/hal.h>
#include <utils/error.h>
#include <utils/log.h>

bool Interrupt::init() {
    // TIM2
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    // DMA TIM2 CH1
    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    // USART6
    HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);

    // DMA USART6 TX
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

    // DMA USART6 RX
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

    // USB
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

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
void USART6_IRQHandler() { HAL_UART_IRQHandler(Uart::getHandle(Uart::Peripheral::UART6)); }
void DMA2_Stream1_IRQHandler() { HAL_DMA_IRQHandler(Dma::getHandle(Dma::DMA2, Dma::STREAM1)); }
void DMA2_Stream6_IRQHandler() { HAL_DMA_IRQHandler(Dma::getHandle(Dma::DMA2, Dma::STREAM6)); }
void OTG_FS_IRQHandler() { HAL_PCD_IRQHandler(Usb::getHandle()); }
}
