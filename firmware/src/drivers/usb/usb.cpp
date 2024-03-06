//--------------------------------------------------
// BLDC Motor Controller
// usb.cpp
// Date: 2023-09-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <drivers/usb/usb.h>
#include <system/hal.h>
#include <utils/log.h>

namespace Usb {
void waitTransmit();
bool enableClock();

PCD_HandleTypeDef hpcd;

} // namespace Usb

bool Usb::init() {
    if (!enableClock()) {
        Log::error("Usb", "Failed to enable clock");
        return false;
    }

    // Interrupts?
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

    // Initialize PCD
    hpcd.pData = nullptr;
    hpcd.Instance = ((USB_OTG_GlobalTypeDef*)USB_OTG_FS_PERIPH_BASE);
    hpcd.Init.dev_endpoints = 6;
    hpcd.Init.speed = PCD_SPEED_FULL;
    hpcd.Init.dma_enable = DISABLE;
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.Sof_enable = DISABLE;
    hpcd.Init.low_power_enable = DISABLE;
    hpcd.Init.lpm_enable = DISABLE;
    hpcd.Init.vbus_sensing_enable = DISABLE;
    hpcd.Init.use_dedicated_ep1 = DISABLE;
    if (HAL_PCD_Init(&hpcd) != HAL_OK) {
        Log::error("Usb", "Failed to initialize PCD");
        return false;
    }

    // Setup FIFOs
    HAL_PCDEx_SetRxFiFo(&hpcd, 0x80);
    HAL_PCDEx_SetTxFiFo(&hpcd, 0, 0x40);
    HAL_PCDEx_SetTxFiFo(&hpcd, 1, 0x80);

    Log::success("Usb", "Initialized");
    return true;
}

void Usb::waitTransmit() {
    // USBD_CDC_HandleTypeDef* hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
    // while (hUsbDeviceFS.pClassData == nullptr) {
    // }
    // while (hcdc->TxState != 0) {
    // }
}

bool Usb::transmit(uint8_t* data, int32_t len, bool busyWait) {
    // if (busyWait)
    //     waitTransmit();
    // return CDC_Transmit_FS(data, len) == USBD_OK;
    return false;
}

void Usb::receiveCallbackCDC(uint8_t* data, uint32_t* len) {
    // Called by base/cube/USB_DEVICE/App/usbd_cdc_if.c
}

bool Usb::enableClock() {
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct{};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        return false;
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    return true;
}
