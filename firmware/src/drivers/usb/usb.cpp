//--------------------------------------------------
// BLDC Motor Controller
// usb.cpp
// Date: 2023-09-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <drivers/usb/usb.h>
#include <drivers/usb/usbHid.h>
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

enum class Endpoint : uint8_t {
    OUT0 = 0x00,
    OUT1 = 0x01,
    OUT2 = 0x02,
    OUT3 = 0x03,
    OUT4 = 0x04,
    OUT5 = 0x05,
    OUT6 = 0x06,
    OUT7 = 0x07,
    IN0 = 0x80,
    IN1 = 0x81,
    IN2 = 0x82,
    IN3 = 0x83,
    IN4 = 0x84,
    IN5 = 0x85,
    IN6 = 0x86,
    IN7 = 0x87,
};

enum class EPType : uint8_t { CONTROL = 0b00, ISOCHRONOUS, BULK, INTERRUPT };
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
    INTERFACE_POWER = 0x08,
    HID = 0x21,
};

enum class BaseClass : uint8_t {
    CLASS_PER_INTERFACE = 0x00,
    AUDIO_DEVICE = 0x01,
    CDC = 0x02,
    HID = 0x03,
};

enum class SubClass : uint8_t {
    NONE = 0x00,
};

enum class Protocol : uint8_t {
    NONE = 0x00,
};

struct __attribute__((__packed__)) DeviceDescriptor {
    enum class BcdUSB : uint16_t {
        USB_1_0 = 0x0100,
        USB_1_1 = 0x0110,
        USB_2_0 = 0x0200,
        USB_3_0 = 0x0300,
    };

    uint8_t bLength;
    DescriptorType bDescriptorType;
    BcdUSB bcdUSB;
    BaseClass bDeviceClass;
    SubClass bDeviceSubClass;
    Protocol bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
};

struct __attribute__((__packed__)) ConfigurationDescriptor {
    using Attribute = uint8_t;
    static constexpr uint8_t ATTRIB_DEFAULT = (1 << 7);
    static constexpr uint8_t ATTRIB_REMOVE_WAKEUP = (1 << 5);
    static constexpr uint8_t ATTRIB_SELF_POWERED = (1 << 6);

    uint8_t bLength;
    DescriptorType bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
};

struct __attribute__((__packed__)) InterfaceDescriptor {
    uint8_t bLength;
    DescriptorType bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    BaseClass bInterfaceClass;
    SubClass bInterfaceSubClass;
    Protocol bInterfaceProtocol;
    uint8_t iInterface;
};

struct __attribute__((__packed__)) EndpointDescriptor {
    using Attribute = uint8_t;
    static constexpr uint8_t ATTRIB_TYPE_CONTROL = 0b00;
    static constexpr uint8_t ATTRIB_TYPE_ISOCHRONOUS = 0b01;
    static constexpr uint8_t ATTRIB_TYPE_BULK = 0b10;
    static constexpr uint8_t ATTRIB_TYPE_INTERRUPT = 0b11;

    // Isochronous type configuration
    static constexpr uint8_t ATTRIB_NO_SYNC = (0b00 << 2);
    static constexpr uint8_t ATTRIB_ASYNC = (0b01 << 2);
    static constexpr uint8_t ATTRIB_ADAPTIVE = (0b10 << 2);
    static constexpr uint8_t ATTRIB_SYNC = (0b11 << 2);

    static constexpr uint8_t ATTRIB_DATA_ENDPOINT = (0b00 << 4);
    static constexpr uint8_t ATTRIB_FEEDBACK_ENDPOINT = (0b01 << 4);
    static constexpr uint8_t ATTRIB_IMPLICIT_FEEDBACK_DATA_ENDPOINT = (0b10 << 4);

    uint8_t bLength;
    DescriptorType bDescriptorType;
    Endpoint bEndpointAddress;
    Attribute bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
};

struct __attribute__((__packed__)) HidDescriptor {
    uint8_t bLength;
    DescriptorType bDescriptorType;
    uint16_t bcdHID;
    uint8_t bCountryCode;
    uint8_t bNumDescriptors;
    uint8_t bDescriptorType0;
    uint16_t wDescriptorLength0;
};

struct HidReport {
    uint8_t x;
    uint8_t y;
    uint8_t buttons;
};

// clang-format off
const uint8_t hidReportDescriptor[] = {
    HID_USAGE_PAGE(HID_PAGE_DESKTOP),
    HID_USAGE(HID_DESKTOP_MOUSE),
    HID_COLLECTION(HID_APPLICATION_COLLECTION),
        HID_USAGE(HID_DESKTOP_POINTER),
        HID_COLLECTION(HID_PHYSICAL_COLLECTION),
            HID_USAGE(HID_DESKTOP_X),
            HID_USAGE(HID_DESKTOP_Y),
            HID_LOGICAL_MINIMUM(-127),
            HID_LOGICAL_MAXIMUM(127),
            HID_REPORT_SIZE(8),
            HID_REPORT_COUNT(2),
            HID_INPUT(HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE),

            HID_USAGE_PAGE(HID_PAGE_BUTTON),
            HID_USAGE_MINIMUM(1),
            HID_USAGE_MAXIMUM(3),
            HID_LOGICAL_MINIMUM(0),
            HID_LOGICAL_MAXIMUM(1),
            HID_REPORT_SIZE(1),
            HID_REPORT_COUNT(3),
            HID_INPUT(HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_REPORT_SIZE(1),
            HID_REPORT_COUNT(5),
            HID_INPUT(HID_IOF_CONSTANT),
        HID_END_COLLECTION,
    HID_END_COLLECTION
};
// clang-format on

const DeviceDescriptor _deviceDescriptor = {
    .bLength = sizeof(DeviceDescriptor),
    .bDescriptorType = DescriptorType::DEVICE,
    .bcdUSB = DeviceDescriptor::BcdUSB::USB_2_0,
    .bDeviceClass = BaseClass::CLASS_PER_INTERFACE,
    .bDeviceSubClass = SubClass::NONE,
    .bDeviceProtocol = Protocol::NONE,
    .bMaxPacketSize0 = 64,
    .idVendor = 0xBBBB,
    .idProduct = 0xBABA,
    .bcdDevice = 0xCAFE,
    .iManufacturer = 0,
    .iProduct = 0,
    .iSerialNumber = 0,
    .bNumConfigurations = 1,
};

struct DescriptorSet {
    ConfigurationDescriptor configurationDescriptor;
    InterfaceDescriptor interfaceDescriptor;
    HidDescriptor hidDescriptor;
    EndpointDescriptor endpointDescriptor;
};
const DescriptorSet _descriptorSet = {
    .configurationDescriptor =
        {
            .bLength = sizeof(ConfigurationDescriptor),
            .bDescriptorType = DescriptorType::CONFIGURATION,
            .wTotalLength = sizeof(DescriptorSet),
            .bNumInterfaces = 1,
            .bConfigurationValue = 1,
            .iConfiguration = 0,
            .bmAttributes = ConfigurationDescriptor::ATTRIB_DEFAULT | ConfigurationDescriptor::ATTRIB_SELF_POWERED,
            .bMaxPower = 25, // 50mA
        },
    .interfaceDescriptor =
        {
            .bLength = sizeof(InterfaceDescriptor),
            .bDescriptorType = DescriptorType::INTERFACE,
            .bInterfaceNumber = 0,
            .bAlternateSetting = 0,
            .bNumEndpoints = 1,
            .bInterfaceClass = BaseClass::HID,
            .bInterfaceSubClass = SubClass::NONE,
            .bInterfaceProtocol = Protocol::NONE,
            .iInterface = 0,
        },
    .hidDescriptor =
        {
            .bLength = sizeof(HidDescriptor),
            .bDescriptorType = DescriptorType::HID,
            .bcdHID = 0x0100,
            .bCountryCode = 0,
            .bNumDescriptors = 1,
            .bDescriptorType0 = 0x22,
            .wDescriptorLength0 = sizeof(hidReportDescriptor),
        },
    .endpointDescriptor =
        {
            .bLength = sizeof(EndpointDescriptor),
            .bDescriptorType = DescriptorType::ENDPOINT,
            .bEndpointAddress = Endpoint::IN3,
            .bmAttributes = EndpointDescriptor::ATTRIB_TYPE_INTERRUPT,
            .wMaxPacketSize = 64,
            .bInterval = 50,

        },
};

void deviceGetDescriptor(const SetupData& setupData);
void deviceRequest(const SetupData& setupData);
void interfaceRequest(const SetupData& setupData);

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

void Usb::update() {}

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
    HAL_PCD_EP_Open(hpcd, (uint8_t)Endpoint::OUT0, EP0_MAX_PACKET_SIZE, (uint8_t)EPType::CONTROL); // EP0 OUT
    HAL_PCD_EP_Open(hpcd, (uint8_t)Endpoint::IN0, EP0_MAX_PACKET_SIZE, (uint8_t)EPType::CONTROL);  // EP0 IN

    // Configure device address
    HAL_PCD_SetAddress(hpcd, 0);
}

void Usb::deviceGetDescriptor(const SetupData& setupData) {
    uint8_t descriptorType = setupData.wValue >> 8;
    // uint8_t descriptorIndex = setupData.wValue & 0xFF;
    switch (descriptorType) {
        case (uint8_t)DescriptorType::DEVICE:
            if (HAL_PCD_EP_Transmit(&hpcd, (uint8_t)Endpoint::IN0, (uint8_t*)&_deviceDescriptor,
                                    std::min((uint16_t)sizeof(_deviceDescriptor), setupData.wLength)) != HAL_OK)
                Log::error("Usb", "Failed to transmit device descriptor");
            Log::debug("Usb", "DEVICE");
            break;
        case (uint8_t)DescriptorType::CONFIGURATION:
            if (HAL_PCD_EP_Transmit(&hpcd, (uint8_t)Endpoint::IN0, (uint8_t*)&_descriptorSet,
                                    std::min((uint16_t)sizeof(DescriptorSet), setupData.wLength)) != HAL_OK)
                Log::error("Usb", "Failed to transmit descriptor set");
            Log::debug("Usb", "CONFIGURATION");
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
                    HAL_PCD_EP_Transmit(&hpcd, (uint8_t)Endpoint::IN0, nullptr, 0);
                    Log::debug("Usb", "Address set to $0", address);
                } break;
                case SetupData::BRequest::GET_DESCRIPTOR:
                    deviceGetDescriptor(setupData);
                    break;
                case SetupData::BRequest::SET_CONFIGURATION:
                    HAL_PCD_EP_Open(&hpcd, (uint8_t)Endpoint::IN3, 64, (uint8_t)EPType::INTERRUPT);
                    HAL_PCD_EP_Transmit(&hpcd, (uint8_t)Endpoint::IN3, nullptr, 0);
                    HAL_PCD_EP_Transmit(&hpcd, (uint8_t)Endpoint::IN0, nullptr, 0);
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

void Usb::interfaceRequest(const SetupData& setupData) {
    switch (setupData.bmRequestType.type) {
        case SetupData::BmRequestType::Type::STANDARD: {
            uint16_t request = setupData.wValue >> 8;
            if (request == 0x22) { // HID REPORT
                HAL_PCD_EP_Transmit(&hpcd, (uint8_t)Endpoint::IN0, (uint8_t*)hidReportDescriptor, sizeof(hidReportDescriptor));
            }
            break;
        }
        case SetupData::BmRequestType::Type::CLASS: {
            if ((int)setupData.bRequest == 0x0A) { // HID SET IDLE
                HAL_PCD_EP_Transmit(&hpcd, (uint8_t)Endpoint::IN0, nullptr, 0);
            }
            break;
        }
        default:
            break;
    }
}

void Usb::setupStageCallback(PCD_HandleTypeDef* hpcd) {
    Log::debug("Usb", "SETUP");
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
        case SetupData::BmRequestType::Recipient::INTERFACE:
            interfaceRequest(setupData);
            break;
        default:
            break;
    }
}

void Usb::dataOutStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) {
    // Log::debug("Usb", "EP$0 OUT -> $1 $2", (int)epnum, (int)hpcd->OUT_ep[epnum].xfer_len, (int)hpcd->OUT_ep[epnum].xfer_count);
    Log::debug("Usb", "EP$0 OUT", (int)epnum);
    // uint8_t* data = hpcd->OUT_ep[epnum].xfer_buff;
}

void Usb::dataInStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) {
    Log::debug("Usb", "EP$0 IN", (int)epnum);

    // TODO if more packets to transmit, transmit

    // TODO if transmitted all packets, send ZLP (zero-length packet) to indicate end to transmission
    // HAL_PCD_EP_Transmit(hpcd, epnum, nullptr, 0);
    // if (epnum == 0x00) {
    //    // Prepare to receive status OUT packet if control endpoint
    //}

    if (epnum == 0x03) {
        // HidReport hidReport = {.x = 5};
        // HAL_PCD_EP_Transmit(hpcd, epnum, (uint8_t*)&hidReport, sizeof(HidReport));
        HAL_PCD_EP_Transmit(hpcd, epnum, nullptr, 0);
        Log::debug("Usb", "Transmit $0", (int)sizeof(HidReport));
    }
    if (epnum == 0x00)
        HAL_PCD_EP_Receive(hpcd, epnum, nullptr, 0);
}

void Usb::SOFCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "SOF"); }

void Usb::suspendCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Suspend"); }
void Usb::resumeCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Resume"); }
void Usb::ISOOUTIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) { Log::debug("Usb", "ISO OUT incomplete"); }
void Usb::ISOINIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum) { Log::debug("Usb", "ISO IN incomplete"); }
void Usb::connectCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Connect"); }
void Usb::disconnectCallback(PCD_HandleTypeDef* hpcd) { Log::debug("Usb", "Disconnect"); }
