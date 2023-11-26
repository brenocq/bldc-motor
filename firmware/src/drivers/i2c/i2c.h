//--------------------------------------------------
// BLDC Motor Controller
// i2c.h
// Date: 2023-11-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_I2C_I2C_H
#define BLDC_DRIVERS_I2C_I2C_H
#include <array>
#include <cstdint>

namespace I2c {

using Address = uint16_t;
enum class Peripheral : uint8_t { I2C1 = 0, I2C2, I2C3, NUM };

/**
 * @brief Initialize i2c driver
 *
 * Initialize I2C peripherals and check if the devices from the i2c list can be found
 */
bool init();

/**
 * @brief Deinitialize i2c driver
 *
 * Deinitialize I2C peripherals
 */
bool deinit();

/**
 * @brief Check if i2c slave is ready
 *
 * @param peripheral I2c peripheral
 * @param address Slave address
 *
 * @return True if slave is ready
 */
bool checkReady(Peripheral peripheral, Address address);

/**
 * @brief Transmit data
 *
 * @param peripheral I2c peripheral
 * @param address Slave address
 * @param data Pointer to data
 * @param len Data length in bytes
 *
 * @return True if transmitted successfully
 */
bool transmit(Peripheral peripheral, Address address, uint8_t* data, uint16_t len);

/**
 * @brief Receive data
 *
 * @param peripheral I2c peripheral
 * @param address Slave address
 * @param data Pointer to data
 * @param len Data length in bytes
 *
 * @return True if received successfully
 */
bool receive(Peripheral peripheral, Address address, uint8_t* data, uint16_t len);

//---------- I2C configs ----------//
static constexpr Address PHASE_U_ADDR = 0x40 << 1;
static constexpr Address PHASE_V_ADDR = 0x41 << 1;
static constexpr Address PHASE_W_ADDR = 0x44 << 1;

struct I2cConfig {
    Peripheral peripheral;
    Address address;
};

inline const std::array i2cList{
    I2cConfig{Peripheral::I2C3, PHASE_U_ADDR},
    I2cConfig{Peripheral::I2C3, PHASE_V_ADDR},
    I2cConfig{Peripheral::I2C3, PHASE_W_ADDR},
};

}; // namespace I2c

#endif // BLDC_DRIVERS_I2C_I2C_H
