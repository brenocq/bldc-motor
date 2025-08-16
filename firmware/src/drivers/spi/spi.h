//--------------------------------------------------
// BLDC Motor Controller
// spi.h
// Date: 2023-11-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_SPI_SPI_H
#define BLDC_DRIVERS_SPI_SPI_H
#include <array>
#include <cstdint>

namespace Spi {

enum class Peripheral : uint8_t { SPI1 = 0, SPI2, SPI3, SPI4, NUM };

/**
 * @brief Initialize spi driver
 *
 * Initialize SPI peripherals
 */
bool init();

/**
 * @brief Deinitialize spi driver
 *
 * Deinitialize SPI peripherals
 */
bool deinit();

/**
 * @brief Transmit data
 *
 * @param peripheral Spi peripheral
 * @param data Pointer to data
 * @param len Data length in bytes
 *
 * @return True if transmitted successfully
 */
bool transmit(Peripheral peripheral, uint8_t* data, uint16_t len);

/**
 * @brief Receive data
 *
 * @param peripheral Spi peripheral
 * @param data Pointer to data
 * @param len Data length in bytes
 *
 * @return True if received successfully
 */
bool receive(Peripheral peripheral, uint8_t* data, uint16_t len);

/**
 * @brief Receive data
 *
 * @param peripheral Spi peripheral
 * @param txData Pointer to data to transmit
 * @param rxData Pointer to data to receive
 * @param len Data length in bytes
 *
 * @return True if transmitted/received successfully
 */
bool transmitReceive(Peripheral peripheral, uint8_t* txData, uint8_t* rxData, uint16_t len);

//---------- SPI configs ----------//
struct SpiConfig {
    enum Mode { MODE_0 = 0, MODE_1, MODE_2, MODE_3 };
    enum CS { CS_SOFT = 0, CS_HARD };

    Peripheral peripheral;
    Mode mode;
    CS cs;
};

inline const std::array spiList{
    SpiConfig{Peripheral::SPI2, SpiConfig::MODE_0, SpiConfig::CS_SOFT},
    SpiConfig{Peripheral::SPI3, SpiConfig::MODE_1, SpiConfig::CS_SOFT},
};

}; // namespace Spi

#endif // BLDC_DRIVERS_SPI_SPI_H
