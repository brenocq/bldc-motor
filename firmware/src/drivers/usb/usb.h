//--------------------------------------------------
// BLDC Motor Controller
// usb.h
// Date: 2023-09-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_USB_USB_H
#define BLDC_DRIVERS_USB_USB_H

/**
 * @brief USB Driver
 *
 * This driver uses the HAL PCD (peripheral-controller-driver) to implement USB FS 2.0
 */
namespace Usb {

using Handle = PCD_HandleTypeDef;

bool init();

void update();

Handle* getHandle();

bool transmit(uint8_t* data, int32_t len, bool busyWait = true);
bool receive(uint8_t* data, uint32_t* len);

}; // namespace Usb

#endif // BLDC_DRIVERS_USB_USB_H
