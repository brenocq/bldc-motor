#ifndef USB_HID_H
#define USB_HID_H
/**\addtogroup USB_HID USB HID class
 * \brief This module contains USB Human Interface Devices class definitions
 * \details This module based on
 * + [Device Class Definition for Human Interface Devices (HID)
 *Version 1.11](https://www.usb.org/sites/default/files/documents/hid1_11.pdf)
 * + [LUFA - the Lightweight USB Framework for
 *AVRs.](https://github.com/abcminiuser/lufa)
 *
 * \par Control usage types
 *                Type     | Logical min | Logical max | Flags | Signal |
 *Operation
 *-------------------------|-------------|-------------|---------------------------|--------|----------------
 *[LC] Linear control      | -1          | 1           | Relative, Preferred
 *state |   Edge | Inc / Dec value
 *                        || -Min        | Max         | Relative, Preferred
 *state |  Level | Inc / Dec value by n
 *                        ||  Min        | Max         | Absolute, Preferred
 *state |   N/A  | Absolute value [OOC] ON/OFF control     | -1          | 1 |
 *Relative, No preferred    |   Edge | 1 asserts ON; -1 assertsOFF
 *                        || 0           | 1           | Relative, Preferred
 *state |   Edge | 0->1 toggles ON/OFF
 *                        || 0           | 1           | Absolute, No preferred
 *|  Level | 0 asserts ON; 1 asserts OFF [MC] Momentary control   | 0 | 1 |
 *Absolute, Preferred state |  Level | 0 asserts, 1 deasserts condition [OSC]
 *One shot control   | 0           | 1           | Relative, Preferred state |
 *Edge | 0->1 triggers an event [RTC] Re-trigger control | 0           | 1 |
 *Absolute, Preferred state |  Level | 1 trigger an event
 *
 *\par Data usage types
 *Type               | Flgs                         | Description
 *-------------------|------------------------------|------------------------
 *[SEL] Selector     | Array                        | Contained a Named Array
 *[SV] Static value  | Constant, Variable, Absolute | Read-only multiple-bit
 *value [SF] Static flag   | Constant, Variable, Absolute | Read-only single-bit
 *value [DV] Dynamic value | Data, Variable, Absolute     | Read-write
 *multiple-bit value [DF] Dynamic flag  | Data, Variable, Absolute     |
 *Read-write single-bit value
 *
 * @{ */

/**\name USB HID class code */
#define USB_CLASS_HID 0x03 /**<\brief Interface belongs to the Human Interface device class.*/

/**\name USB HID subclass codes
 * @{ */
#define USB_HID_SUBCLASS_NONBOOT 0x00 /**<\brief Nonboot subclass.*/
#define USB_HID_SUBCLASS_BOOT 0x01    /**<\brief Boot subclass.*/
/** @} */

/**\name USB HID protocol codes
 * @{ */
#define USB_HID_PROTO_NONBOOT 0x00  /**<\brief Nonboot protocol.*/
#define USB_HID_PROTO_KEYBOARD 0x01 /**<\brief Keyboard boot protocol.*/
#define USB_HID_PROTO_MOUSE 0x02    /**<\brief Mouse boot protocol.*/
/** @} */

#define USB_HID_REPORT_IN 0x00      /**<\brief Indicates that the item is an IN report type.*/
#define USB_HID_REPORT_OUT 0x01     /**<\brief Indicates that the item is an OUT report type.*/
#define USB_HID_REPORT_FEATURE 0x02 /**<\brief Indicates that the item is a FEATURE report type.*/

/**\name USB HID class-specified requests
 * @{ */
#define USB_HID_GETREPORT 0x01   /**<\brief Request to get the current HID report from the device.*/
#define USB_HID_GETIDLE 0x02     /**<\brief Request to get the current device idle count.*/
#define USB_HID_GETPROTOCOL 0x03 /**<\brief Request to get the current HID report protocol mode.*/
#define USB_HID_SETREPORT 0x09   /**<\brief Request to set the current HID report to the device.*/
#define USB_HID_SETIDLE 0x0A     /**<\brief Request to set the device's idle count.*/
#define USB_HID_SETPROTOCOL 0x0B /**<\brief Request to set the current HID report protocol mode.*/
/** @} */

/**\name USB HID class-specified descriptor types
 * @{ */
#define USB_DTYPE_HID 0x21        /**<\brief HID class HID descriptor type.*/
#define USB_DTYPE_HID_REPORT 0x22 /**<\brief HID class HID report descriptor type.*/
/** @} */

/**\name USB HID country codes
 * @{ */
#define USB_HID_COUNTRY_NONE 0   /**<\brief Not supported */
#define USB_HID_COUNTRY_AR 1     /**<\brief Arabic */
#define USB_HID_COUNTRY_BE 2     /**<\brief Belgian */
#define USB_HID_COUNTRY_CA_BI 3  /**<\brief Canadian-Bilingual */
#define USB_HID_COUNTRY_CA_FR 4  /**<\brief Canadian-French */
#define USB_HID_COUNTRY_CZ 5     /**<\brief Czech Republic */
#define USB_HID_COUNTRY_DK 6     /**<\brief Danish */
#define USB_HID_COUNTRY_FI 7     /**<\brief Finnish */
#define USB_HID_COUNTRY_FR 8     /**<\brief French */
#define USB_HID_COUNTRY_DE 9     /**<\brief German */
#define USB_HID_COUNTRY_GR 10    /**<\brief Greek */
#define USB_HID_COUNTRY_HEB 11   /**<\brief Hebrew */
#define USB_HID_COUNTRY_HU 12    /**<\brief Hungary */
#define USB_HID_COUNTRY_ISO 13   /**<\brief International (ISO) */
#define USB_HID_COUNTRY_IT 14    /**<\brief Italian */
#define USB_HID_COUNTRY_JP 15    /**<\brief Japan (Katakana) */
#define USB_HID_COUNTRY_KR 16    /**<\brief Korean */
#define USB_HID_COUNTRY_LAT 17   /**<\brief Latin American */
#define USB_HID_COUNTRY_NL 18    /**<\brief Netherlands/Dutch */
#define USB_HID_COUNTRY_NO 19    /**<\brief Norwegian */
#define USB_HID_COUNTRY_PER 20   /**<\brief Persian (Farsi) */
#define USB_HID_COUNTRY_PL 21    /**<\brief Poland */
#define USB_HID_COUNTRY_PO 22    /**<\brief Portuguese */
#define USB_HID_COUNTRY_RU 23    /**<\brief Russia */
#define USB_HID_COUNTRY_SK 24    /**<\brief Slovakia */
#define USB_HID_COUNTRY_ES 25    /**<\brief Spanish */
#define USB_HID_COUNTRY_SE 26    /**<\brief Swedish */
#define USB_HID_COUNTRY_CH_FR 26 /**<\brief Swiss-French */
#define USB_HID_COUNTRY_CH_DE 27 /**<\brief Swiss-German */
#define USB_HID_COUNTRY_CH 29    /**<\brief Switzerland */
#define USB_HID_COUNTRY_TW 30    /**<\brief Taiwan */
#define USB_HID_COUNTRY_TR_Q 31  /**<\brief Turkish-Q */
#define USB_HID_COUNTRY_UK 32    /**<\brief United Kingdom */
#define USB_HID_COUNTRY_US 33    /**<\brief United States */
#define USB_HID_COUNTRY_YU 34    /**<\brief Yugoslavia */
#define USB_HID_COUNTRY_TR_F 35  /**<\brief Turkish-F */
/** @} */

/**\name HID Collections types
 * @{ */
#define HID_PHYSICAL_COLLECTION 0x00 /**<\brief A physical collection of items.*/
#define HID_APPLICATION_COLLECTION                                                                                                                   \
    0x01 /**<\brief Applies a name to a top level collection which the operating                                                                     \
          * system uses to identify a device and possibly remap to a legacy                                                                          \
          * API.*/
#define HID_LOGICAL_COLLECTION                                                                                                                       \
    0x02 /**<\brief A logical collection of                                                                                                          \
            items.*/
#define HID_REPORT_COLLECTION 0x03
#define HID_NARY_COLLECTION                                                                                                                          \
    0x04 /**<\brief A collection that encompasses an array definition, naming                                                                        \
          * the array set or the field created by the array.*/
#define HID_USAGE_SWITCH_COLLECTION                                                                                                                  \
    0x05 /**<\brief Modifies the purpose or function of the usages (controls)                                                                        \
            that it contains. */
#define HID_USAGE_MODIFIER_COLLECTION                                                                                                                \
    0x06 /**<\brief Modifies the purpose or function of the usages (controls)                                                                        \
            that contains it.*/
/** @} */

/**\brief USB HID functional descriptor */
struct usb_hid_descriptor {
    uint8_t bLength;             /**<\brief Size of the descriptor, in bytes. */
    uint8_t bDescriptorType;     /**<\brief Type of the descriptor, set to \ref
                                    USB_DTYPE_HID */
    uint16_t bcdHID;             /**<\brief BCD encoded version that the HID descriptor and
                                    device complies to. */
    uint8_t bCountryCode;        /**<\brief Country code of the localized device, or zero
                                    if universal. */
    uint8_t bNumDescriptors;     /**<\brief Total number of HID report descriptors for
                                    the interface. */
    uint8_t bDescriptorType0;    /**<\brief 1'st HID report descriptor type, set to
                                    \ref USB_DTYPE_HID_REPORT */
    uint16_t wDescriptorLength0; /**<\brief 1'sr HID report descriptor length in
                                    bytes. */
} __attribute__((packed));

/**\brief USB HID functional descriptor header */
struct usb_hid_descriptor_header {
    uint8_t bLength;         /**<\brief Size of the descriptor, in bytes. */
    uint8_t bDescriptorType; /**<\brief Type of the descriptor, set to \ref
                                USB_DTYPE_HID */
    uint16_t bcdHID;         /**<\brief BCD encoded version that the HID descriptor and
                                device complies to. */
    uint8_t bCountryCode;    /**<\brief Country code of the localized device, or zero
                                if universal. */
    uint8_t bNumDescriptors; /**<\brief Total number of HID report descriptors for
                                the interface. */
} __attribute__((packed));

/**\brief USB HID report descriptor */
struct usb_hid_report_descriptor {
    uint8_t bDescriptorType;    /**<\brief Type of HID report, set to \ref
                                   USB_DTYPE_HID_REPORT */
    uint16_t wDescriptorLength; /**<\brief Length of the associated HID report
                                   descriptor, in bytes. */
} __attribute__((packed));

/**\brief Helper macro for the multireport hid descriptor */
#define DECLARE_USB_HID_DESCRIPTOR(p)                                                                                                                \
    struct usb_hid_descriptor_##p {                                                                                                                  \
        uint8_t bLength;                                                                                                                             \
        uint8_t bDescriptorType;                                                                                                                     \
        uint16_t bcdHID;                                                                                                                             \
        uint8_t bCountryCode;                                                                                                                        \
        uint8_t bNumDescriptors;                                                                                                                     \
        struct usb_hid_report_descriptor report[p];                                                                                                  \
    } __attribute__((packed));

/* Private Interface - For use in library only: */
#if !defined(__DOXYGEN__)
/* Macros: */
#define CONCAT(x, y) x##y
#define CONCAT_EXPANDED(x, y) CONCAT(x, y)
#define HID_RI_DATA_SIZE_MASK 0x03
#define HID_RI_TYPE_MASK 0x0C
#define HID_RI_TAG_MASK 0xF0
#define HID_RI_TYPE_MAIN 0x00
#define HID_RI_TYPE_GLOBAL 0x04
#define HID_RI_TYPE_LOCAL 0x08
#define HID_RI_DATA_BITS_0 0x00
#define HID_RI_DATA_BITS_8 0x01
#define HID_RI_DATA_BITS_16 0x02
#define HID_RI_DATA_BITS_32 0x03
#define HID_RI_DATA_BITS(DataBits) CONCAT_EXPANDED(HID_RI_DATA_BITS_, DataBits)
#define _HID_RI_ENCODE_0(Data)
#define _HID_RI_ENCODE_8(Data) , (Data & 0xFF)
#define _HID_RI_ENCODE_16(Data) _HID_RI_ENCODE_8(Data) _HID_RI_ENCODE_8(Data >> 8)
#define _HID_RI_ENCODE_32(Data) _HID_RI_ENCODE_16(Data) _HID_RI_ENCODE_16(Data >> 16)
#define _HID_RI_ENCODE(DataBits, ...) CONCAT_EXPANDED(_HID_RI_ENCODE_, DataBits(__VA_ARGS__))
#define _HID_RI_ENTRY(Type, Tag, DataBits, ...) (Type | Tag | HID_RI_DATA_BITS(DataBits)) _HID_RI_ENCODE(DataBits, (__VA_ARGS__))
#endif
/* Public Interface - May be used in end-application: */
/* Macros: */
/** \name HID Input, Output and Feature Report Descriptor Item Flags */
//@{
#define HID_IOF_CONSTANT                                                                                                                             \
    (1 << 0)                      /**<Item is a static read-only field in a report and cannot be                                                     \
                                     modified.*/
#define HID_IOF_DATA (0 << 0)     /**<Item contains a modifiable device data.*/
#define HID_IOF_VARIABLE (1 << 1) /**<Each field represents data from a physical control.*/
#define HID_IOF_ARRAY                                                                                                                                \
    (0 << 1) /**<Array returns an index in each field that corresponds to the                                                                        \
                pressed button.*/
#define HID_IOF_RELATIVE                                                                                                                             \
    (1 << 2)                      /**<Data is relative (indicating the change in value from the last                                                 \
                                     report).*/
#define HID_IOF_ABSOLUTE (0 << 2) /**<Data is absolute (based on a fixed origin).*/
#define HID_IOF_WRAP                                                                                                                                 \
    (1 << 3) /**<Data “rolls over” when reaching either the extreme high or low                                                                  \
                value.*/
#define HID_IOF_NO_WRAP                                                                                                                              \
    (0 << 3)                                /**<Data not “rolls over” when reaching either the extreme high or                                   \
                                               low value. */
#define HID_IOF_NON_LINEAR (1 << 4)         /**<Noninear relationship between logical and physical units.*/
#define HID_IOF_LINEAR (0 << 4)             /**<Linear relationship between logical and physical units.*/
#define HID_IOF_NO_PREFERRED_STATE (1 << 5) /**<Control has no preferred state.*/
#define HID_IOF_PREFERRED_STATE                                                                                                                      \
    (0 << 5) /**<Control has a preferred state to which it will return when the                                                                      \
              * user is not physically interacting with the control. */
#define HID_IOF_NULLSTATE                                                                                                                            \
    (1 << 6)                              /**<Control has a state in which it is not sending meaningful                                              \
                                             data.*/
#define HID_IOF_NO_NULL_POSITION (0 << 6) /**<Control has no nullstate.*/
#define HID_IOF_VOLATILE (1 << 7)         /**<Data may be changed by device itself. (Output and Feature) */
#define HID_IOF_NON_VOLATILE (0 << 7)     /**<Data can be changed by host only. (Output and Feature) */
#define HID_IOF_BUFFERED_BYTES                                                                                                                       \
    (1 << 8)                      /**<Indicates that the control emits a fixed-size stream of                                                        \
                                     bytes.*/
#define HID_IOF_BITFIELD (0 << 8) /**<Data is a fixed bitfield.*/
//@}

/** \name HID Report Descriptor Main items */
//@{
/** Describes information about the data provided by one or more physical
 * controls. */
#define HID_RI_INPUT(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_MAIN, 0x80, DataBits, __VA_ARGS__)
/** similar to an Input item except it describes data sent to the device—for
 * example, LED states. */
#define HID_RI_OUTPUT(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_MAIN, 0x90, DataBits, __VA_ARGS__)
/** identifies a relationship between two or more data (Input, Output, or
 * Feature.) */
#define HID_RI_COLLECTION(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_MAIN, 0xA0, DataBits, __VA_ARGS__)
/** describe device configuration information that can be sent to the device. */
#define HID_RI_FEATURE(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_MAIN, 0xB0, DataBits, __VA_ARGS__)
/** closes a collection */
#define HID_RI_END_COLLECTION(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_MAIN, 0xC0, DataBits, __VA_ARGS__)
//@}
/** \name HID Report Descriptor Global items */
//@{
/** Unsigned integer specifying the current Usage Page. */
#define HID_RI_USAGE_PAGE(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x00, DataBits, __VA_ARGS__)
/** Defines a minimum value that a variable or array item will report. */
#define HID_RI_LOGICAL_MINIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x10, DataBits, __VA_ARGS__)
/** Defines a maximum value that a variable or array item will report. */
#define HID_RI_LOGICAL_MAXIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x20, DataBits, __VA_ARGS__)
/** Defines a minimum value for the physical extent of a variable item */
#define HID_RI_PHYSICAL_MINIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x30, DataBits, __VA_ARGS__)
/** Defines a maximum value for the physical extent of a variable item */
#define HID_RI_PHYSICAL_MAXIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x40, DataBits, __VA_ARGS__)
/** Value of the unit exponent in base 10.
 * \note The USB-HID specification is unclear about Unit exponent usage.
 * Practically it's limited by 1 nibble.
 */
#define HID_RI_UNIT_EXPONENT(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x50, DataBits, __VA_ARGS__)
/** Encoded unit value \see \ref HID_UNITS_ENCODE */
#define HID_RI_UNIT(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x60, DataBits, __VA_ARGS__)
/** Unsigned integer specifying the size of the report fields in bits. */
#define HID_RI_REPORT_SIZE(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x70, DataBits, __VA_ARGS__)
/** Unsigned value that specifies the Report ID. */
#define HID_RI_REPORT_ID(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x80, DataBits, __VA_ARGS__)
/** Unsigned integer specifying the number of data fields for the item */
#define HID_RI_REPORT_COUNT(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x90, DataBits, __VA_ARGS__)
/** Places a copy of the global item state table on the stack. */
#define HID_RI_PUSH(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0xA0, DataBits, __VA_ARGS__)
/** Replaces the item state table with the top structure from the stack. */
#define HID_RI_POP(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0xB0, DataBits, __VA_ARGS__)
//@}
/** \name HID Report Descriptor Local items */
//@{
/** Represents a suggested usage for the item or collection. */
#define HID_RI_USAGE(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0x00, DataBits, __VA_ARGS__)
/** Defines the starting usage associated with an array or bitmap */
#define HID_RI_USAGE_MINIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0x10, DataBits, __VA_ARGS__)
/** Defines the ending usage associated with an array or bitmap. */
#define HID_RI_USAGE_MAXIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0x20, DataBits, __VA_ARGS__)
/** Determines the body part used for a control. */
#define HID_RI_DESIGNATOR_INDEX(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0x30, DataBits, __VA_ARGS__)
/** Defines the index of the starting designator associated with an array or
 * bitmap. */
#define HID_RI_DESIGNATOR_MIN(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0x40, DataBits, __VA_ARGS__)
/** Defines the index of the ending designator associated with an array or
 * bitmap. */
#define HID_RI_DESIGNATOR_MAX(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0x50, DataBits, __VA_ARGS__)
/** String index for a String descriptor; allows a string to be associated with
 * a particular item or control. */
#define HID_RI_STRING_INDEX(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0x70, DataBits, __VA_ARGS__)
/** Specifies the first string index when assigning a group of sequential
 * strings to controls in an array or bitmap. */
#define HID_RI_STRING_MINIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0x80, DataBits, __VA_ARGS__)
/** Specifies the last string index when assigning a group of sequential strings
 * to controls in an array or bitmap. */
#define HID_RI_STRING_MAXIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0x90, DataBits, __VA_ARGS__)
/** Defines the beginning or end of a set of local items (1 = open set, 0 =
 * close set). */
#define HID_RI_DELIMITER(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_LOCAL, 0xA0, DataBits, __VA_ARGS__)
//@}
/** \name HID Report Descriptor for most common cases */
//@{
#define HID_INPUT(data) HID_RI_INPUT(8, data)           /**<\copydoc HID_RI_INPUT */
#define HID_OUTPUT(data) HID_RI_OUTPUT(8, data)         /**<\copydoc HID_RI_OUTPUT */
#define HID_COLLECTION(data) HID_RI_COLLECTION(8, data) /**<\copydoc HID_RI_COLLECTION */
#define HID_FEATURE(data)                                                                                                                            \
    HID_RI_FEATURE(8, data)                         /**<\copydoc HID_RI_FEATURE                                                                      \
                                                     */
#define HID_END_COLLECTION HID_RI_END_COLLECTION(0) /**<\copydoc HID_RI_END_COLLECTION */
#define HID_USAGE_PAGE(data)                                                                                                                         \
    HID_RI_USAGE_PAGE(8, data)                                       /**<Range [0; 0xFF]; \copydoc HID_RI_USAGE_PAGE                                 \
                                                                      */
#define HID_LOGICAL_MINIMUM(data) HID_RI_LOGICAL_MINIMUM(8, data)    /**<Range [-127; 127]; \copydoc HID_RI_LOGICAL_MINIMUM */
#define HID_LOGICAL_MAXIMUM(data) HID_RI_LOGICAL_MAXIMUM(8, data)    /**<Range [-127; 127]; \copydoc HID_RI_LOGICAL_MAXIMUM */
#define HID_PHYSICAL_MINIMUM(data) HID_RI_PHYSICAL_MINIMUM(16, data) /**<Range [-32767; 32767]; \copydoc HID_RI_PHYSICAL_MINIMUM */
#define HID_PHYSICAL_MAXIMUM(data) HID_RI_PHYSICAL_MAXIMUM(16, data) /**<Range [-32767; 32767]; \copydoc HID_RI_PHYSICAL_MAXIMUM */
#define HID_UNIT_EXPONENT(data) HID_RI_UNIT_EXPONENT(8, data)        /**<Range [-127; 127]; \copydoc HID_RI_UNIT_EXPONENT */
#define HID_UNIT(bits, data)                                                                                                                         \
    HID_RI_UNIT(bits, data)                                   /**<\copydoc HID_RI_UNIT                                                               \
                                                               */
#define HID_REPORT_SIZE(data) HID_RI_REPORT_SIZE(8, data)     /**<Range [0; 0xFF]; \copydoc HID_RI_REPORT_SIZE */
#define HID_REPORT_ID(data) HID_RI_REPORT_ID(8, data)         /**<Range [0; 0xFF]; \copydoc HID_RI_REPORT_ID */
#define HID_REPORT_COUNT(data) HID_RI_REPORT_COUNT(8, data)   /**<Range [0; 0xFF]; \copydoc HID_RI_REPORT_COUNT */
#define HID_PUSH HID_RI_PUSH(0)                               /**<\copydoc HID_RI_PUSH */
#define HID_POP HID_RI_POP(0)                                 /**<\copydoc HID_RI_POP */
#define HID_USAGE(data) HID_RI_USAGE(8, data)                 /**<Range [0; 0xFF]; \copydoc HID_RI_USAGE */
#define HID_USAGE_MINIMUM(data) HID_RI_USAGE_MINIMUM(8, data) /**<Range [0; 0xFF]; \copydoc HID_RI_USAGE_MINIMUM */
#define HID_USAGE_MAXIMUM(data) HID_RI_USAGE_MAXIMUM(8, data) /**<Range [0; 0xFF]; \copydoc HID_RI_USAGE_MAXIMUM */
//@}

/**\name Macros for the units encoding
 * \anchor HID_UNITS_ENCODE */
//@{
#define HID_UNIT_NONE 0x00 /**<No system. */
#define HID_UNIT_CGS_LINEAR                                                                                                                          \
    0x01 /**<Centimeter-Gram-Second metric linear system. \note USB-HID defines                                                                      \
            this as SI linear.*/
#define HID_UNIT_CGS_ROTATION                                                                                                                        \
    0x02                                               /**<Centimeter-Gram-Second metric rotation system. \note USB-HID                              \
                                                          defines this as SI rotation.*/
#define HID_UNIT_IMPERIAL_LINEAR 0x03                  /**<Imperial linear system.*/
#define HID_UNIT_IMPERIAL_ROTATION 0x04                /**<Imperial rotation system.*/
#define HID_UNIT_LENGTH(exp) ((exp & 0x0F) << 4)       /**<Length, position, distance unit. cm (CGS), inch (Imperial) */
#define HID_UNIT_ANGLE(exp) ((exp & 0x0F) << 4)        /**<Angle unit. radians(CGS), degree (Imperial) */
#define HID_UNIT_MASS(exp) ((exp & 0x0F) << 8)         /**<Mass unit. gram (CGS), slug (Imperial) */
#define HID_UNIT_TIME(exp) ((exp & 0x0F) << 12)        /**<Time unit. secound. */
#define HID_UNIT_TEMPERATURE(exp) ((exp & 0x0F) << 16) /**<Temperature unit. Kelvin (CGS), Fahrenheit (Imperial) */
#define HID_UNIT_CURRENT(exp) ((exp & 0x0F) << 20)     /**<Current unit. Ampere */
#define HID_UNIT_LUMINOUS(exp) ((exp & 0x0F) << 24)    /**<Luminous intensity unit. Candela */
//@}

/**\name Macros for the some SI named units */
//@{
/** SI length. m = 1E2 * cm */
#define HID_UNIT_METER(exp) HID_UNIT(8, HID_UNIT_CGS_LINEAR | HID_UNIT_LENGTH(1)), HID_UNIT_EXPONENT(exp + 2)
/** SI mass. kg = 1E3 * g */
#define HID_UNIT_KG(exp)                                                                                                                             \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1), \
    HID_UNIT_EXPONENT(exp + 3)
/** SI time. 1s */
#define HID_UNIT_SECOND(exp)                                                                                                                         \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_TIME(1), \
    HID_UNIT_EXPONENT(exp + 0)
/** SI velocity. m/s = 1E2 * cm/s */
#define HID_UNIT_MPS(exp) HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_LENGTH(1) | HID_UNIT_TIME(-1)), HID_UNIT_EXPONENT(exp + 2)
/** SI acceleration. m/s^2 = 1E2 * cm/s^2*/
#define HID_UNIT_MPS2(exp) HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_LENGTH(1) | HID_UNIT_TIME(-2)), HID_UNIT_EXPONENT(exp + 2)
/** SI force. N = 1E5 * g * cm / s^2 */
#define HID_UNIT_NEWTON(exp) HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(1) | HID_UNIT_TIME(-2)), HID_UNIT_EXPONENT(exp + 5)
/** SI energy. J = 1E7 * g * cm^2 / s^2 */
#define HID_UNIT_JOULE(exp) HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) | HID_UNIT_TIME(-2)), HID_UNIT_EXPONENT(exp + 7)
/** SI power. W = 1E7 * g * cm^2 / s^3 */
#define HID_UNIT_WATT(exp) HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) | HID_UNIT_TIME(-3)), HID_UNIT_EXPONENT(exp + 7)
/** SI pressure. Pa = 1E1 * g / (cm * s^2) */
#define HID_UNIT_PASCAL(exp)                                                                                                                         \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(-1) | HID_UNIT_TIME(-2)), HID_UNIT_EXPONENT(exp + 1)
/** SI frequency. Hz = 1 / s */
#define HID_UNIT_HERTZ(exp) HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_TIME(-1)), HID_UNIT_EXPONENT(exp + 0)
/** SI current. A */
#define HID_UNIT_AMPERE(exp) HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_CURRENT(1)), HID_UNIT_EXPONENT(exp)
/** SI voltage. V = W / A  = 1E7 * g * cm^2 / (s^3 * A) */
#define HID_UNIT_VOLT(exp)                                                                                                                           \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) | HID_UNIT_TIME(-3) | HID_UNIT_CURRENT(-1)), HID_UNIT_EXPONENT(exp + 7)
/** SI resistance. Ohm = 1E7 * g * cm^2 / (s^3 * A^2) */
#define HID_UNIT_OHM(exp)                                                                                                                            \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) | HID_UNIT_TIME(-3) | HID_UNIT_CURRENT(-2)), HID_UNIT_EXPONENT(exp + 7)
/** SI inductance. H = 1E7 * g * cm^2 / (s^2 * A^2) */
#define HID_UNIT_HENRY(exp)                                                                                                                          \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) | HID_UNIT_TIME(-2) | HID_UNIT_CURRENT(-2)), HID_UNIT_EXPONENT(exp + 7)
/** SI  capacitance. F = 1E-7 * s^4 * A^2 / (cm^2 * g) */
#define HID_UNIT_FARAD(exp)                                                                                                                          \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(-1) | HID_UNIT_LENGTH(-2) | HID_UNIT_TIME(4) | HID_UNIT_CURRENT(2)), HID_UNIT_EXPONENT(exp - 7)
/** SI  electric charge. C = s * A */
#define HID_UNIT_COULOMB(exp) HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_TIME(1) | HID_UNIT_CURRENT(1)), HID_UNIT_EXPONENT(exp)
/** SI  magnetic flux density. T = 1E3 * g / (s^2 * A) */
#define HID_UNIT_TESLA(exp) HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_TIME(-2) | HID_UNIT_CURRENT(-1)), HID_UNIT_EXPONENT(exp)
/** SI  illuminance. lx = 1E4 * cd / cm^2 */
#define HID_UNIT_LUX(exp) HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_LENGTH(-1) | HID_UNIT_LUMINOUS(1)), HID_UNIT_EXPONENT(exp)

//@}

/** @}  */

/**\ingroup USB_HID
 * \addtogroup USB_HID_USAGES_DESKTOP HID Usage Tables for Desktop
 * \brief Contains USB HID Usages definitions for Generic Desktop Page
 * \details This module based on
 * + [HID Usage Tables
 * Version 1.12](https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf)
 * @{ */

#define HID_PAGE_DESKTOP 0x01 /**<\brief Desktop usage page.*/

/**\name Application Usages
 * @{ */
#define HID_DESKTOP_POINTER 0x01   /**<\brief CP Pointer control.*/
#define HID_DESKTOP_MOUSE 0x02     /**<\brief CA Mouse.*/
#define HID_DESKTOP_JOYSTICK 0x04  /**<\brief CA Joystick.*/
#define HID_DESKTOP_GAMEPAD 0x05   /**<\brief CA Gamepad.*/
#define HID_DESKTOP_KEYBOARD 0x06  /**<\brief CA Keybiard.*/
#define HID_DESKTOP_KEYPAD 0x07    /**<\brief CA Keypad.*/
#define HID_DESKTOP_MULTIAXIS 0x08 /**<\brief CA 3D input device.*/
#define HID_DESKTOP_TABLET 0x09 /**<\brief CA System controls on Tablet PCs.*/
/** @} */

/**\name Axis Usages
 * @{ */
#define HID_DESKTOP_X \
  0x30 /**<\brief DV Linear translation in the X direction.*/
#define HID_DESKTOP_Y \
  0x31 /**<\brief DV Linear translation in the Y direction.*/
#define HID_DESKTOP_Z \
  0x32 /**<\brief DV Linear translation in the Z direction.*/
#define HID_DESKTOP_RX 0x33 /**<\brief DV Rotation about X axis.*/
#define HID_DESKTOP_RY 0x34 /**<\brief DV Rotation about Y axis.*/
#define HID_DESKTOP_RZ 0x35 /**<\brief DV Rotation about Z axis.*/
/** @} */

/**\name Miscellaneous Controls
 * @{ */
#define HID_DESKTOP_SLIDER \
  0x36 /**<\brief DV Linear control for a variable value.*/
#define HID_DESKTOP_DIAL \
  0x37 /**<\brief DV Rotary control for a variable value.*/
#define HID_DESKTOP_WHEEL \
  0x38 /**<\brief DV Rotary control for a variable value.*/
#define HID_DESKTOP_HAT_SWITCH                                 \
  0x39 /**<\brief DV A specialized mechanical configuration of \
        * switches generating a variable value with a null state.*/
#define HID_DESKTOP_MOTION_WAKEUP                           \
  0x3C /**<\brief DF Enables the generation of a USB remote \
        * wakeup when the device detects motion.*/
#define HID_DESKTOP_START 0x3D /**<\brief OOC Session start button.*/
#define HID_DESKTOP_SELECT \
  0x3E /**<\brief OOC Application option select button.*/
#define HID_DESKTOP_RESOLUTION_MULT \
  0x48 /**<\brief DV Resolution Multiplier for a Control.*/
/** @} */

/**\name Vector Usages
 * @{ */
#define HID_DESKTOP_VX 0x40 /**<\brief DV Vector in the X direction.*/
#define HID_DESKTOP_VY 0x41 /**<\brief DV Vector in the Y direction.*/
#define HID_DESKTOP_VZ 0x42 /**<\brief DV Vector in the Z direction.*/
#define HID_DESKTOP_VBRX \
  0x43 /**<\brief DV Relative Vector in the X direction.*/
#define HID_DESKTOP_VBRY \
  0x44 /**<\brief DV Relative vector in the Y direction.*/
#define HID_DESKTOP_VBRZ \
  0x45                       /**<\brief DV Relative vector in the Z direction.*/
#define HID_DESKTOP_VNO 0x46 /**<\brief DV A non oriented vector or value.*/
/** @} */

/**\name System Controls
 * @{ */
#define HID_DESKTOP_SYS_CONTROL \
  0x80 /**<\brief CA Application-level collection.*/
#define HID_DESKTOP_SYS_CONTEXT_MNU \
  0x84 /**<\brief OSC Evokes a context-sensitive menu.*/
#define HID_DESKTOP_SYS_MAIN_MNU \
  0x85 /**<\brief OSC Evokes the OS main-level selection menu.*/
#define HID_DESKTOP_SYS_APP_MNU \
  0x86 /**<\brief OSC Displays an application-specific menu.*/
#define HID_DESKTOP_SYS_MENU_HELP 0x87   /**<\brief OSC Displays the help \
                                            menu.*/
#define HID_DESKTOP_SYS_MENU_EXIT 0x88   /**<\brief OSC Exits a menu.*/
#define HID_DESKTOP_SYS_MENU_SELECT 0x89 /**<\brief OSC Selects a menu item.*/
#define HID_DESKTOP_SYS_MENU_RIGHT 0x8A  /**<\brief RTC Menu select right.*/
#define HID_DESKTOP_SYS_MENU_LEFT 0x8B   /**<\brief RTC Menu select left.*/
#define HID_DESKTOP_SYS_MENU_UP 0x8C     /**<\brief RTC Menu select up.*/
#define HID_DESKTOP_SYS_MENU_DOWN 0x8D   /**<\brief RTC Menu select down.*/
/** @} */

/**\name Power Controls
 * @{ */
#define HID_DESKTOP_SYS_PWR_DOWN 0x81 /**<\brief OSC Power down control.*/
#define HID_DESKTOP_SYS_SLEEP 0x82    /**<\brief OSC Sleep control.*/
#define HID_DESKTOP_SYS_WAKEUP 0x83   /**<\brief OSC Wakeup control.*/
#define HID_DESKTOP_SYS_RST_COLD 0x8E /**<\brief OSC Cold restart control.*/
#define HID_DESKTOP_SYS_RST_WARM 0x8F /**<\brief OSC Warm restart control.*/
#define HID_DESKTOP_SYS_DOCK 0xA0     /**<\brief OSC Prepare for docking.*/
#define HID_DESKTOP_SYS_UNDOCK 0xA1   /**<\brief OSC Prepare for undocking. */
#define HID_DESKTOP_SYS_SETUP 0xA2 /**<\brief OSC Enter to BIOS-level setup.*/
#define HID_DESKTOP_SYS_SPKR_MUTE 0xA7 /**<\brief OSC Mute system speakers.*/
#define HID_DESKTOP_SYS_HIBERNATE \
  0xA8 /**<\brief OSC System hibernate control.*/
/** @} */

/**\name Buffered Bytes
 * @{ */
#define HID_DESKTOP_COUNTEDBUF                                \
  0x3A /**<\brief CL Used with buffered byte data to indicate \
        * the number of valid bytes in the buffered-byte field.*/
#define HID_DESKTOP_BYTECOUNT                                  \
  0x3B /**<\brief DV Defines a report field that indicates the \
        * number of meaningful data bytes in an associated     \
        * buffered-byte field.*/
/** @} */

/**\name Direction Pads
 * @{ */
#define HID_DESKTOP_DPAD_UP \
  0x90 /**<\brief OOC Top of a Direction Pad is pressed.*/
#define HID_DESKTOP_DPAD_DOWN \
  0x91 /**<\brief OOC Bottom of a Direction Pad is pressed.*/
#define HID_DESKTOP_DPAD_RIGHT \
  0x92 /**<\brief OOC Right side of a Direction Pad is pressed.*/
#define HID_DESKTOP_DPAD_LEFT \
  0x93 /**<\brief OOC Left side of a Direction Pad is pressed.*/
/** @} */

/**\name Feature Notifications
 * @{ */
#define HID_DESKTOP_FEATURE_NOTIFY                             \
  0x47 /**<\brief DV This usage is declared in an Input report \
        * and is used as a notification to the host that the   \
        * contents of a specific Feature report has changed.*/
/** @} */

/**\name Software Flow Control
 * @{ */
#define HID_DESKTOP_SYS_BREAK 0xA3 /**<\brief OSC System break control.*/
#define HID_DESKTOP_SYS_DBG_BREAK \
  0xA4 /**<\brief OSC System debugger break control.*/
#define HID_DESKTOP_APP_BREAK 0xA5 /**<\brief OSC Application break control.*/
#define HID_DESKTOP_APP_DBG_BREAK \
  0xA6 /**<\brief OSC Application debugger break control.*/
/** @} */

/**\name System Display Control
 * @{ */
#define HID_DESKTOP_SYS_DISP_INVERT \
  0xB0 /**<\brief OSC Set display to render in inverted colors.*/
#define HID_DESKTOP_SYS_DISP_INT \
  0xB1 /**<\brief OSC Set the captive display as the primary display.*/
#define HID_DESKTOP_SYS_DISP_EXT \
  0xB2 /**<\brief OSC Set the external display as the primary display.*/
#define HID_DESKTOP_SYS_DISP_BOTH                             \
  0xB3 /**<\brief OSC Use both internal and external displays \
        * as primary diaplay.*/
#define HID_DESKTOP_SYS_DISP_DUAL                             \
  0xB4 /**<\brief OSC Use both internal and external displays \
        * as primary and secondary diaplays.*/
#define HID_DESKTOP_SYS_DISP_TGL \
  0xB5 /**<\brief OSC Toggles internal/external/both displays.*/
#define HID_DESKTOP_SYS_DISP_SWAP \
  0xB6 /**<\brief OSC Swap primary/secondary displays.*/
#define HID_DESKTOP_SYS_DISP_AUTO 0xB7 /**<\brief OCS Toggles LCD autoscale.*/

/** @}  */
/** @}  */

/**\ingroup USB_HID
 * \addtogroup USB_HID_USAGES_BUTTON HID Usage Tables for Button
 * \brief Contains USB HID Usages definitions for Button Page
 * \details This module based on
 * + [HID Usage Tables
 * Version 1.12](https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf)
 * @{ */

#define HID_PAGE_BUTTON 0x09 /**<\brief HID usage page for Buttons */

#define HID_BUTTON_NO_PRESSED 0x00 /**<\brief No button pressed */
#define HID_BUTTON_1 0x01          /**<\brief Button 1 pressed */
#define HID_BUTTON_2 0x02          /**<\brief Button 2 pressed */
#define HID_BUTTON_3 0x03          /**<\brief Button 3 pressed */
#define HID_BUTTON_4 0x04          /**<\brief Button 4 pressed */
#define HID_BUTTON_5 0x05          /**<\brief Button 5 pressed */

/** @}  */

#endif // USB_HID_H
