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

void setupStageCallback(PCD_HandleTypeDef* hpcd);
void dataOutStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum);
void dataInStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum);
void SOFCallback(PCD_HandleTypeDef* hpcd);
void resetCallback(PCD_HandleTypeDef* hpcd);
void suspendCallback(PCD_HandleTypeDef* hpcd);
void resumeCallback(PCD_HandleTypeDef* hpcd);
void ISOOUTIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum);
void ISOINIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum);
void connectCallback(PCD_HandleTypeDef* hpcd);
void disconnectCallback(PCD_HandleTypeDef* hpcd);

PCD_HandleTypeDef hpcd;
} // namespace Usb

bool Usb::init() {
    if (!enableClock()) {
        Log::error("Usb", "Failed to enable clock");
        return false;
    }

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

    // Register callBacks
    HAL_PCD_RegisterCallback(&hpcd, HAL_PCD_SOF_CB_ID, SOFCallback);
    HAL_PCD_RegisterCallback(&hpcd, HAL_PCD_SETUPSTAGE_CB_ID, setupStageCallback);
    HAL_PCD_RegisterCallback(&hpcd, HAL_PCD_RESET_CB_ID, resetCallback);
    HAL_PCD_RegisterCallback(&hpcd, HAL_PCD_SUSPEND_CB_ID, suspendCallback);
    HAL_PCD_RegisterCallback(&hpcd, HAL_PCD_RESUME_CB_ID, resumeCallback);
    HAL_PCD_RegisterCallback(&hpcd, HAL_PCD_CONNECT_CB_ID, connectCallback);
    HAL_PCD_RegisterCallback(&hpcd, HAL_PCD_DISCONNECT_CB_ID, disconnectCallback);
    HAL_PCD_RegisterDataOutStageCallback(&hpcd, dataOutStageCallback);
    HAL_PCD_RegisterDataInStageCallback(&hpcd, dataInStageCallback);
    HAL_PCD_RegisterIsoOutIncpltCallback(&hpcd, ISOOUTIncompleteCallback);
    HAL_PCD_RegisterIsoInIncpltCallback(&hpcd, ISOINIncompleteCallback);

    // Setup FIFOs
    HAL_PCDEx_SetRxFiFo(&hpcd, 0x80);
    HAL_PCDEx_SetTxFiFo(&hpcd, 0, 0x40);
    HAL_PCDEx_SetTxFiFo(&hpcd, 1, 0x80);

    // Start
    if (HAL_PCD_Start(&hpcd) != HAL_OK) {
        Log::error("Usb", "Failed to start PCD");
        return false;
    }

    Log::success("Usb", "Initialized");
    return true;
}

Usb::Handle* Usb::getHandle() { return &hpcd; }

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

void Usb::setupStageCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Setup"); }
void Usb::dataOutStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) { Log::debug("Usb", "OUT"); }
void Usb::dataInStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) { Log::debug("Usb", "IN"); }
void Usb::SOFCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "SOF"); }
void Usb::resetCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Reset"); }
void Usb::suspendCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Suspend"); }
void Usb::resumeCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Resume"); }
void Usb::ISOOUTIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) { Log::debug("Usb", "ISO OUT incomplete"); }
void Usb::ISOINIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) { Log::debug("Usb", "ISO IN incomplete"); }
void Usb::connectCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Connect"); }
void Usb::disconnectCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Disconnect"); }
