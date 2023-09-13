//--------------------------------------------------
// BLDC Motor Controller
// encoder.cpp
// Date: 2023-09-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/encoder/encoder.h>
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/uart/uart.h>

bool Encoder::init() {
    Gpio::write(Gpio::ENC_CSN_PIN, true);
    Gpio::write(Gpio::ENC_CLK_PIN, true);
    Gpio::write(Gpio::ENC_DO_PIN, false);

    return true;
}

float Encoder::readAngle() {
    uint16_t data = 0;
    uint8_t status = 0;
    uint8_t crc = 0;

    Gpio::write(Gpio::ENC_CSN_PIN, false);
    Hardware::delayMs(1);
    Gpio::write(Gpio::ENC_CLK_PIN, false);
    Hardware::delayMs(1);
    // Read data
    for (int i = 13; i >= 0; i--) {
        Gpio::write(Gpio::ENC_CLK_PIN, true);
        Hardware::delayMs(1);
        Gpio::write(Gpio::ENC_CLK_PIN, false);
        data |= (Gpio::read(Gpio::ENC_DO_PIN) << i);
        Hardware::delayMs(1);
    }
    // Read status
    for (int i = 3; i >= 0; i--) {
        Gpio::write(Gpio::ENC_CLK_PIN, true);
        Hardware::delayMs(1);
        Gpio::write(Gpio::ENC_CLK_PIN, false);
        status |= (Gpio::read(Gpio::ENC_DO_PIN) << i);
        Hardware::delayMs(1);
    }
    // Read CRC
    for (int i = 5; i >= 0; i--) {
        Gpio::write(Gpio::ENC_CLK_PIN, true);
        Hardware::delayMs(1);
        Gpio::write(Gpio::ENC_CLK_PIN, false);
        crc |= (Gpio::read(Gpio::ENC_DO_PIN) << i);
        Hardware::delayMs(1);
    }
    Gpio::write(Gpio::ENC_CSN_PIN, true);

    float angle = data / 16384.0f;

    Uart::transmit("  - Data: " + std::to_string(data) + "\n");
    Uart::transmit("  - Status: " + std::to_string((int)status) + "\n");
    Uart::transmit("  - CRC: " + std::to_string((int)crc) + "\n");

    return angle;
}
