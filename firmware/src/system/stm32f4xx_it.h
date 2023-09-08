//--------------------------------------------------
// BLDC Motor Controller
// stm32f4xx_it.h
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_SYSTEM_STM32F4XX_IT_H
#define BLDC_SYSTEM_STM32F4XX_IT_H

extern "C" {

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

}

#endif // BLDC_SYSTEM_STM32F4XX_IT
