//--------------------------------------------------
// BLDC Motor Controller
// hardware.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/adc/adc.h>
#include <drivers/clock/clock.h>
#include <drivers/current/current.h>
#include <drivers/dma/dma.h>
#include <drivers/gpio/gpio.h>
#include <drivers/hardware.h>
#include <drivers/interrupt/interrupt.h>
#include <drivers/spi/spi.h>
#include <drivers/timer/timer.h>
#include <drivers/uart/uart.h>
#include <drivers/voltage/voltage.h>
#include <system/hal.h>
#include <utils/error.h>
#include <utils/log.h>

bool Hardware::init() {
    HAL_Init();
    Clock::init();
    if (!Gpio::init())
        Error::hardFault("Failed to initialize GPIO driver");

    if (!Uart::init())
        Error::hardFault("Failed to initialize UART driver");
    if (!Spi::init())
        Error::hardFault("Failed to initialize SPI driver");
    // if (!Usb::init())
    //     Error::hardFault("Failed to initialize USB driver");
    if (!Adc::init())
        Error::hardFault("Failed to initialize ADC driver");
    if (!Timer::init())
        Error::hardFault("Failed to initialize TIMER driver");
    if (!Dma::init())
        Error::hardFault("Failed to initialize DMA driver");
    if (!Interrupt::init())
        Error::hardFault("Failed to initialize Interrupt driver");

    // Initialize debug pins as LOW
    Gpio::write(Gpio::DEBUG0_PIN, false);
    Gpio::write(Gpio::DEBUG1_PIN, false);
    Gpio::write(Gpio::DEBUG2_PIN, false);

    // Timer-DMA
    Timer::linkDma(Timer::LED_TIM, Timer::LED_CH, Dma::getHandle(Dma::LED_DMA, Dma::LED_STREAM));

    // Link DMA to UART
    Uart::linkDmaTx(Uart::Peripheral::UART6, Dma::getHandle(Dma::UART_DMA, Dma::UART_TX_STREAM));
    Uart::linkDmaRx(Uart::Peripheral::UART6, Dma::getHandle(Dma::UART_DMA, Dma::UART_RX_STREAM));
    Hardware::delayMs(1);

    // Initialize voltage sense
    if (!volt_src.init(Gpio::VOLT_SRC_PIN))
        Error::hardFault("Failed to initialize voltage SRC_PIN driver");
    if (!volt_u_phase.init(Gpio::VOLT_U_PIN))
        Error::hardFault("Failed to initialize voltage U_PIN driver");
    if (!volt_v_phase.init(Gpio::VOLT_V_PIN))
        Error::hardFault("Failed to initialize voltage V_PIN driver");
    if (!volt_w_phase.init(Gpio::VOLT_W_PIN))
        Error::hardFault("Failed to initialize voltage W_PIN driver");

    // Initialize current sense
    if (!curr_u_phase.init(Gpio::CURR_U_PIN))
        Error::hardFault("Failed to initialize current U_PIN driver");
    if (!curr_v_phase.init(Gpio::CURR_V_PIN))
        Error::hardFault("Failed to initialize current V_PIN driver");
    if (!curr_w_phase.init(Gpio::CURR_W_PIN))
        Error::hardFault("Failed to initialize current W_PIN driver");

    // if (!led.init())
    //    Error::hardFault("Failed to initialize led driver");
    // if (!encoder.init(Spi::Peripheral::SPI3, Gpio::ENC_CS_PIN))
    //     Error::hardFault("Failed to initialize encoder driver");

    // if (!phaseU.init(Phase::U, Spi::Peripheral::SPI1, Gpio::PHASE_CS_U_PIN))
    //     Error::hardFault("Failed to initialize phase driver for phase U");
    // if (!phaseV.init(Phase::V, Spi::Peripheral::SPI1, Gpio::PHASE_CS_V_PIN))
    //     Error::hardFault("Failed to initialize phase driver for phase V");
    // if (!phaseW.init(Phase::W, Spi::Peripheral::SPI1, Gpio::PHASE_CS_W_PIN))
    //     Error::hardFault("Failed to initialize phase driver for phase W");
    // if (!imu.init(Spi::Peripheral::SPI2, Gpio::IMU_CS_PIN))
    //     Error::hardFault("Failed to initialize IMU");
    // if (!motor.init())
    //     Error::hardFault("Failed to initialize motor driver");
    // if (!AttaConnector::init())
    //     Error::hardFault("Failed to initialize atta connector");

    return true;
}

void Hardware::delay(float s) { HAL_Delay(s * 1000); }

void Hardware::delayMs(uint32_t ms) { HAL_Delay(ms); }

void Hardware::delayUs(uint32_t us) {
    uint32_t clkCycleStart = DWT->CYCCNT;

    // Convert to number of cycles
    us *= (HAL_RCC_GetHCLKFreq() / 1000000);

    // Delay until end
    while ((DWT->CYCCNT - clkCycleStart) < us) {
    }
}
