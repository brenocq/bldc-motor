//--------------------------------------------------
// BLDC Motor Controller
// stm32f4xx_it.c
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <system/stm32f4xx_it.h>
#include "stm32f4xx_hal.h"

void NMI_Handler(void)
{
    while(1)
    {

    }
}

void HardFault_Handler(void)
{
    while(1)
    {
    }
}

void MemManage_Handler(void)
{
    while(1)
    {
    }
}

void BusFault_Handler(void)
{
    while(1)
    {
    }
}

void UsageFault_Handler(void)
{
    while(1)
    {
    }
}

void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{
    HAL_IncTick();
}
