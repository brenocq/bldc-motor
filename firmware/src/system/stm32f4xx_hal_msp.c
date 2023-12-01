//--------------------------------------------------
// BLDC Motor Controller
// stm32f4xx_hal_msp.c
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "stm32f4xx_hal.h"

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
}
