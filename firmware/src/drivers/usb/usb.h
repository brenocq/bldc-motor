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
 * @note not tested yet
 */
namespace Usb {

bool init();

void update();

bool transmit(uint8_t* data, int32_t len, bool busyWait = true);

// Called by CDC code when one packet is received (up to 64 bytes)
void receiveCallbackCDC(uint8_t* data, uint32_t* len);

}; // namespace Usb

#endif // BLDC_DRIVERS_USB_USB_H
