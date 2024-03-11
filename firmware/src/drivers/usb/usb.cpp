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

// clang-format off
#define FIELD_CAST_16(T) \
        T() = default; \
        T(uint16_t d) { *((uint16_t*)this) = d; } \
        operator uint16_t() const { return *((uint16_t*)this); }
#define FIELD_CAST_8(T) \
        T() = default; \
        T(uint8_t d) { *((uint8_t*)this) = d; } \
        operator uint8_t() const { return *((uint8_t*)this); }
// clang-format on

namespace Usb {

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

enum class EPType { CONTROL = 0b00, ISOCHRONOUS, BULK, INTERRUPT };
constexpr uint16_t EP0_MAX_PACKET_SIZE = 64;

struct SetupData {
    struct BmRequestType {
        enum class Recipient : uint8_t { DEVICE = 0b00, INTERFACE, ENDPOINT, OTHER };
        enum class Type : uint8_t { STANDARD = 0b00, CLASS, VENDOR };
        enum class Direction : uint8_t { HOST_TO_DEVICE = 0b0, DEVICE_TO_HOST };

        Recipient recipient : 5;
        Type type : 2;
        Direction direction : 1;

        FIELD_CAST_8(BmRequestType);
    };

    enum class BRequest {
        GET_STATUS = 0x00,
        CLEAR_FEATURE = 0x01,
        SET_FEATURE = 0x03,
        SET_ADDRESS = 0x05,
        GET_DESCRIPTOR = 0x06,
        SET_DESCRIPTOR = 0x07,
        GET_CONFIGURATION = 0x08,
        SET_CONFIGURATION = 0x09,
        GET_INTERFACE = 0x0A,
        SET_INTERFACE = 0x0B,
        SYNCH_FRAME = 0x0C,
    };

    BmRequestType bmRequestType;
    BRequest bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
};

enum class DescriptorType : uint8_t {
    DEVICE = 0x01,
    CONFIGURATION = 0x02,
    STRING = 0x03,
    INTERFACE = 0x04,
    ENDPOINT = 0x05,
    DEVICE_QUALIFIER = 0x06,
    OTHER_SPEED_CONFIGURATION = 0x07,
    INTERFACE_POWER = 0x08
};

struct DeviceDescriptor {
    enum class BcdUSB : uint16_t {
        USB_1_0 = 0x0100,
        USB_1_1 = 0x0110,
        USB_2_0 = 0x0200,
        USB_3_0 = 0x0300,
    };

    enum class DeviceClass : uint8_t {
        CLASS_PER_INTERFACE = 0x00,
        AUDIO_DEVICE = 0x01,
        CDC = 0x02,
    };

    enum class DeviceSubClass : uint8_t {
        NONE = 0x00,
    };

    enum class DeviceProtocol : uint8_t {
        NONE = 0x00,
    };

    uint8_t bLength;
    DescriptorType bDescriptorType;
    BcdUSB bcdUSB;
    DeviceClass bDeviceClass;
    DeviceSubClass bDeviceSubClass;
    DeviceProtocol bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
};

struct ConfigurationDescriptor {
    uint8_t bLength;
    DescriptorType bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
};

const DeviceDescriptor _deviceDescriptor = {
    .bLength = sizeof(DeviceDescriptor),
    .bDescriptorType = DescriptorType::DEVICE,
    .bcdUSB = DeviceDescriptor::BcdUSB::USB_2_0,
    .bDeviceClass = DeviceDescriptor::DeviceClass::CLASS_PER_INTERFACE,
    .bDeviceSubClass = DeviceDescriptor::DeviceSubClass::NONE,
    .bDeviceProtocol = DeviceDescriptor::DeviceProtocol::NONE,
    .bMaxPacketSize0 = 64,
    .idVendor = 0xBBBB,
    .idProduct = 0xBABA,
    .bcdDevice = 0xCAFE,
    .iManufacturer = 0,
    .iProduct = 0,
    .iSerialNumber = 0,
    .bNumConfigurations = 1,
};

const ConfigurationDescriptor _configurationDescriptor = {
    .bLength = sizeof(ConfigurationDescriptor),
    .bDescriptorType = DescriptorType::CONFIGURATION,
    .wTotalLength = sizeof(ConfigurationDescriptor), // Size for the whole descriptor set
    .bNumInterfaces = 0,
    .bConfigurationValue = 0,
    .iConfiguration = 0,
    .bmAttributes = 0,
    .bMaxPower = 0,
};

void deviceGetDescriptor(const SetupData& setupData);
void deviceRequest(const SetupData& setupData);

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

    // Register callbacks
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

    // Setup FIFOs (1.25KB)
    HAL_PCDEx_SetRxFiFo(&hpcd, 0x80);    // 512B
    HAL_PCDEx_SetTxFiFo(&hpcd, 0, 0x40); // 256B
    HAL_PCDEx_SetTxFiFo(&hpcd, 1, 0x80); // 512B

    // Start
    if (HAL_PCD_Start(&hpcd) != HAL_OK) {
        Log::error("Usb", "Failed to start PCD");
        return false;
    }

    Log::success("Usb", "Initialized");
    return true;
}

Usb::Handle* Usb::getHandle() { return &hpcd; }

bool Usb::transmit(uint8_t* data, int32_t len, bool busyWait) { return false; }

bool Usb::receive(uint8_t* data, uint32_t* len) { return false; }

bool Usb::enableClock() {
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct{};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        return false;
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    return true;
}

void Usb::resetCallback(PCD_HandleTypeDef* hpcd) {
    Log::debug("Usb", "Reset");

    // Configure EN0
    HAL_PCD_EP_Open(hpcd, 0x00U, EP0_MAX_PACKET_SIZE, (uint8_t)EPType::CONTROL); // EP0 OUT
    HAL_PCD_EP_Open(hpcd, 0x80U, EP0_MAX_PACKET_SIZE, (uint8_t)EPType::CONTROL); // EP0 IN

    // Configure device address
    HAL_PCD_SetAddress(hpcd, 0);
}

void Usb::deviceGetDescriptor(const SetupData& setupData) {
    uint8_t descriptorType = setupData.wValue >> 8;
    uint8_t descriptorIndex = setupData.wValue & 0xFF;
    switch (descriptorType) {
        case (uint8_t)DescriptorType::DEVICE:
            if (HAL_PCD_EP_Transmit(&hpcd, 0x00U, (uint8_t*)&_deviceDescriptor, sizeof(_deviceDescriptor)) != HAL_OK)
                Log::error("Usb", "Failed to transmit device descriptor");
            break;
        case (uint8_t)DescriptorType::CONFIGURATION:
            if (HAL_PCD_EP_Transmit(&hpcd, 0x00U, (uint8_t*)&_configurationDescriptor, sizeof(_configurationDescriptor)) != HAL_OK)
                Log::error("Usb", "Failed to transmit device descriptor");
            break;
        case (uint8_t)DescriptorType::DEVICE_QUALIFIER:

            break;
        default:
            Log::warning("Usb", "Could not handle deviceGetDescriptor with type $0", (int)descriptorType);
            break;
    }
}

void Usb::deviceRequest(const SetupData& setupData) {
    switch (setupData.bmRequestType.type) {
        case SetupData::BmRequestType::Type::STANDARD: {
            switch (setupData.bRequest) {
                case SetupData::BRequest::SET_ADDRESS: {
                    uint16_t address = setupData.wValue;
                    if (address > 127 || setupData.wIndex > 0 || setupData.wLength > 0) {
                        Log::error("Usb", "Wrong setupData for SET_ADDRESS");
                        break;
                    }
                    HAL_PCD_SetAddress(&hpcd, address);
                    HAL_PCD_EP_Transmit(&hpcd, 0, nullptr, 0);
                    Log::debug("Usb", "Address set to $0", address);
                } break;
                case SetupData::BRequest::GET_DESCRIPTOR:
                    deviceGetDescriptor(setupData);
                    break;
                case SetupData::BRequest::SET_CONFIGURATION:
                    Log::debug("Usb", "Set configuration to $0", (int)setupData.wValue);
                    break;
                default:
                    break;
            }
        }
        case SetupData::BmRequestType::Type::VENDOR:
        case SetupData::BmRequestType::Type::CLASS:
            break;
    }
}

void Usb::setupStageCallback(PCD_HandleTypeDef* hpcd) {
    uint8_t* setup = (uint8_t*)hpcd->Setup;
    SetupData setupData;
    setupData.bmRequestType = setup[0];
    setupData.bRequest = (SetupData::BRequest)setup[1];
    setupData.wValue = (setup[3] << 8) | setup[2];
    setupData.wIndex = (setup[5] << 8) | setup[4];
    setupData.wLength = (setup[7] << 8) | setup[6];
    switch (setupData.bmRequestType.recipient) {
        case SetupData::BmRequestType::Recipient::DEVICE:
            deviceRequest(setupData);
            break;
        default:
            break;
    }
    Log::debug("Usb", "Setup bmRequest $0 bRequest $1 wValue $2 wIndex $3 wLength $4", (int)setupData.bmRequestType, (int)setupData.bRequest,
               setupData.wValue, setupData.wIndex, setupData.wLength);
}

void Usb::dataOutStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) {
    // Log::debug("Usb", "EP$0 OUT -> $1 $2", (int)epnum, (int)hpcd->OUT_ep[epnum].xfer_len, (int)hpcd->OUT_ep[epnum].xfer_count);
    Log::debug("Usb", "EP$0 OUT", (int)epnum);
    uint8_t* data = hpcd->OUT_ep[epnum].xfer_buff;
}

void Usb::dataInStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) {
    Log::debug("Usb", "EP$0 IN", (int)epnum);

    // TODO if more packets to transmit, transmit

    // TODO if transmitted all packets, send ZLP (zero-length packet) to indicate end to transmission
    // HAL_PCD_EP_Transmit(hpcd, epnum, nullptr, 0);
    if (epnum == 0x00) {
        // Prepare to receive status OUT packet if control endpoint
        HAL_PCD_EP_Receive(hpcd, epnum, nullptr, 0);
    }
}

void Usb::SOFCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "SOF"); }

void Usb::suspendCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Suspend"); }
void Usb::resumeCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Resume"); }
void Usb::ISOOUTIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) { Log::debug("Usb", "ISO OUT incomplete"); }
void Usb::ISOINIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) { Log::debug("Usb", "ISO IN incomplete"); }
void Usb::connectCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Connect"); }
void Usb::disconnectCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Disconnect"); }
