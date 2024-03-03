//--------------------------------------------------
// BLDC Motor Controller
// imu.h
// Date: 2023-11-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_DRIVERS_IMU_IMU_H
#define BLDC_DRIVERS_IMU_IMU_H
#include <cstdint>
#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>

// clang-format off
// Used to cast register structs to uint8_t
#define REG_CAST() operator uint8_t() const { return *((uint8_t*)this); }
// clang-format on

class Imu {
  public:
    bool init(Spi::Peripheral peripheral, Gpio::Gpio chipSelect);

    void getGyrAcc(int16_t* gx, int16_t* gy, int16_t* gz, int16_t* ax, int16_t* ay, int16_t* az);
    std::array<int16_t, 3> getAcc();
    std::array<int16_t, 3> getGyr();
    float getTemperature();

  private:
    // clang-format off
    enum Reg : uint8_t {
        REG_FUNC_CFG_ACCESS             = 0x01,
        REG_PIN_CTRL                    = 0x02,
        REG_FIFO_CTRL1                  = 0x07,
        REG_FIFO_CTRL2                  = 0x08,
        REG_FIFO_CTRL3                  = 0x09,
        REG_FIFO_CTRL4                  = 0x0A,
        REG_COUNTER_BDR_REG1            = 0x0B,
        REG_COUNTER_BDR_REG2            = 0x0C,
        REG_INT1_CTRL                   = 0x0D,
        REG_INT2_CTRL                   = 0x0E,
        REG_WHO_AM_I                    = 0x0F,
        REG_CTRL1_XL                    = 0x10,
        REG_CTRL2_G                     = 0x11,
        REG_CTRL3_C                     = 0x12,
        REG_CTRL4_C                     = 0x13,
        REG_CTRL5_C                     = 0x14,
        REG_CTRL6_C                     = 0x15,
        REG_CTRL7_G                     = 0x16,
        REG_CTRL8_XL                    = 0x17,
        REG_CTRL9_XL                    = 0x18,
        REG_CTRL10_C                    = 0x19,
        REG_ALL_INT_SRC                 = 0x1A,
        REG_WAKE_UP_SRC                 = 0x1B,
        REG_TAP_SRC                     = 0x1C,
        REG_D6D_SRC                     = 0x1D,
        REG_STATUS_REG                  = 0x1E,
        REG_OUT_TEMP_L                  = 0x20,
        REG_OUT_TEMP_H                  = 0x21,
        REG_OUTX_L_G                    = 0x22,
        REG_OUTX_H_G                    = 0x23,
        REG_OUTY_L_G                    = 0x24,
        REG_OUTY_H_G                    = 0x25,
        REG_OUTZ_L_G                    = 0x26,
        REG_OUTZ_H_G                    = 0x27,
        REG_OUTX_L_A                    = 0x28,
        REG_OUTX_H_A                    = 0x29,
        REG_OUTY_L_A                    = 0x2A,
        REG_OUTY_H_A                    = 0x2B,
        REG_OUTZ_L_A                    = 0x2C,
        REG_OUTZ_H_A                    = 0x2D,
        REG_EMB_FUNC_STATUS_MAINPAGE    = 0x35,
        REG_FSM_STATUS_A_MAINPAGE       = 0x36,
        REG_FSM_STATUS_B_MAINPAGE       = 0x37,
        REG_STATUS_MASTER_MAINPAGE      = 0x39,
        REG_FIFO_STATUS1                = 0x3A,
        REG_FIFO_STATUS2                = 0x3B,
        REG_TIMESTAMP0                  = 0x40,
        REG_TIMESTAMP1                  = 0x41,
        REG_TIMESTAMP2                  = 0x42,
        REG_TIMESTAMP3                  = 0x43,
        REG_TAP_CFG0                    = 0x56,
        REG_TAP_CFG1                    = 0x57,
        REG_TAP_CFG2                    = 0x58,
        REG_TAP_THS_6D                  = 0x59,
        REG_INT_DUR2                    = 0x5A,
        REG_WAKE_UP_THS                 = 0x5B,
        REG_WAKE_UP_DUR                 = 0x5C,
        REG_FREE_FALL                   = 0x5D,
        REG_MD1_CFG                     = 0x5E,
        REG_MD2_CFG                     = 0x5F,
        REG_I3C_BUS_AVB                 = 0x62,
        REG_INTERNAL_FREQ_FINE          = 0x63,
        REG_INT_OIS                     = 0x6F,
        REG_CTRL1_OIS                   = 0x70,
        REG_CTRL2_OIS                   = 0x71,
        REG_CTRL3_OIS                   = 0x72,
        REG_X_OFS_USR                   = 0x73,
        REG_Y_OFS_USR                   = 0x74,
        REG_Z_OFS_USR                   = 0x75,
        REG_FIFO_DATA_OUT_TAG           = 0x78,
        REG_FIFO_DATA_OUT_X_L           = 0x79,
        REG_FIFO_DATA_OUT_X_H           = 0x7A,
        REG_FIFO_DATA_OUT_Y_L           = 0x7B,
        REG_FIFO_DATA_OUT_Y_H           = 0x7C,
        REG_FIFO_DATA_OUT_Z_L           = 0x7D,
        REG_FIFO_DATA_OUT_Z_H           = 0x7E
    };
    // clang-format on

    struct Ctrl1XL {
        uint8_t none : 1;
        uint8_t enableLPF2 : 1;
        uint8_t fullScale : 2;
        uint8_t odr : 4;
        REG_CAST();

        enum ODR : uint8_t {
            ODR_0Hz = 0b0000,
            ODR_12_5Hz = 0b0001,
            ODR_26Hz = 0b0010,
            ODR_52Hz = 0b0011,
            ODR_104Hz = 0b0100,
            ODR_208Hz = 0b0101,
            ODR_416Hz = 0b0110,
            ODR_833Hz = 0b0111,
            ODR_1666Hz = 0b1000,
            ODR_3333Hz = 0b1001,
            ODR_6666Hz = 0b1010,
        };

        enum FullScale : uint8_t {
            FS_2G = 0b00,
            FS_4G = 0b10,
            FS_8G = 0b11,
        };
    };

    struct Ctrl2G {
        uint8_t none : 1;
        uint8_t fullScale : 3;
        uint8_t odr : 4;
        REG_CAST();

        enum ODR : uint8_t {
            ODR_0Hz = 0b0000,
            ODR_12_5Hz = 0b0001,
            ODR_26Hz = 0b0010,
            ODR_52Hz = 0b0011,
            ODR_104Hz = 0b0100,
            ODR_208Hz = 0b0101,
            ODR_416Hz = 0b0110,
            ODR_833Hz = 0b0111,
            ODR_1666Hz = 0b1000,
            ODR_3333Hz = 0b1001,
            ODR_6666Hz = 0b1010,
        };

        enum FullScale : uint8_t {
            FS_125DPS = 0b001,
            FS_250DPS = 0b000,
            FS_500DPS = 0b010,
            FS_1000DPS = 0b100,
            FS_2000DPS = 0b110,
        };
    };

    uint8_t readReg(Reg reg);
    void writeReg(Reg reg, uint8_t data);

    static constexpr uint8_t WHO_AM_I_DEFAULT = 0x6B;

    Spi::Peripheral _peripheral; ///< SPI peripehral
    Gpio::Gpio _chipSelect;      ///< SPI chip select
};

inline Imu imu;

#endif // BLDC_DRIVERS_IMU_IMU_H
