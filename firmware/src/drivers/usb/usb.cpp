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

extern USBD_HandleTypeDef hUsbDeviceFS;

namespace Usb {
void waitTransmit();
}

bool Usb::init() {
    MX_USB_DEVICE_Init();
    return true;
}

void Usb::waitTransmit() {
    USBD_CDC_HandleTypeDef* hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
    while (hUsbDeviceFS.pClassData == nullptr) {
    }
    while (hcdc->TxState != 0) {
    }
}

bool Usb::transmit(uint8_t* data, int32_t len, bool busyWait) {
    if (busyWait)
        waitTransmit();
    return CDC_Transmit_FS(data, len) == USBD_OK;
}

void Usb::receiveCallbackCDC(uint8_t* data, uint32_t* len) {
    // Called by base/cube/USB_DEVICE/App/usbd_cdc_if.c
}
