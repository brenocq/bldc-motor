//--------------------------------------------------
// BLDC Motor Controller
// encoder.h
// Date: 2023-09-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_ENCODER_ENCODER_H
#define BLDC_DRIVERS_ENCODER_ENCODER_H
#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>

class Encoder {
  public:
    bool init(Spi::Peripheral peripheral, Gpio::Gpio chipSelect);

    float readAngle();

  private:
    enum Status {
        STATUS_MAG_FIELD_TOO_STRONG = 0b0001,
        STATUS_MAG_FIELD_TOO_WEAK = 0b0010,
        STATUS_PUSH_BUTTON_DETECTED = 0b0100,
        STATUS_LOSS_OF_TRACK = 0b1000,
    };

    Spi::Peripheral _peripheral; ///< SPI peripheral
    Gpio::Gpio _chipSelect;      ///< SPI chip select
};

inline Encoder encoder;

#endif // BLDC_DRIVERS_ENCODER_ENCODER_H
