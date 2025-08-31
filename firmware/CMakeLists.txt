cmake_minimum_required(VERSION 3.22)
project(bldc_motor_controller LANGUAGES CXX C)

# Compiler
enable_language(ASM)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Folders
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/base/linker/STM32F4xx/STM32F446xE.ld)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/base/cube/CMSIS)
set(HAL_DIR ${CMAKE_SOURCE_DIR}/base/cube/HAL/STM32F4xx)
set(MIDDLEWARES_DIR ${CMAKE_SOURCE_DIR}/base/cube/Middlewares)
set(FREERTOS_DIR ${MIDDLEWARES_DIR}/FreeRTOS)
set(USB_DEVICE_LIB_DIR ${MIDDLEWARES_DIR}/ST/STM32_USB_Device_Library)
set(USB_DEVICE_DIR ${CMAKE_SOURCE_DIR}/base/cube/USB_DEVICE)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Include
include_directories(
    src/
    src/system/
)

#---------- Flags ----------#
# Flags for CPU, FPU, and ABI. Needed for both compiling and linking.
list(APPEND TARGET_ARCH_FLAGS
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Flags common to both C and C++
list(APPEND COMMON_FLAGS
    -masm-syntax-unified
    -specs=nosys.specs
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -ffunction-sections
    -fdata-sections
    # Assembler flags can be passed through the compiler driver
    -Wa,--defsym,CALL_ARM_SYSTEM_INIT=1
)

# Flags that are only used for linking
list(APPEND LINKER_FLAGS
    -Wl,-T${LINKER_SCRIPT}
    -Wl,--gc-sections
    #-Wl,--verbose
)

# Debug and Release flags
set(CMAKE_C_FLAGS_RELEASE   "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG   "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

# Target architecture flags apply to all steps (compile and link)
add_compile_options(${TARGET_ARCH_FLAGS})
add_link_options(${TARGET_ARCH_FLAGS})

# Apply the flags using modern CMake commands that handle lists correctly
add_compile_options(${COMMON_FLAGS})
add_link_options(${LINKER_FLAGS})

# Apply C++ specific flags individually to avoid list processing issues
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-unwind-tables>)

# Set language standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#---------- HAL ----------#
set(HAL_INCLUDES
    ${CMSIS_DIR}/ARM/inc
    ${CMSIS_DIR}/STM32F4xx/inc
    ${HAL_DIR}/inc
)
set(HAL_SOURCES
    ${HAL_DIR}/src/stm32f4xx_hal.c
    ${HAL_DIR}/src/stm32f4xx_hal_adc.c
    ${HAL_DIR}/src/stm32f4xx_hal_adc_ex.c
    ${HAL_DIR}/src/stm32f4xx_hal_cortex.c
    ${HAL_DIR}/src/stm32f4xx_hal_dma.c
    ${HAL_DIR}/src/stm32f4xx_hal_dma_ex.c
    ${HAL_DIR}/src/stm32f4xx_hal_exti.c
    ${HAL_DIR}/src/stm32f4xx_hal_flash.c
    ${HAL_DIR}/src/stm32f4xx_hal_flash_ex.c
    ${HAL_DIR}/src/stm32f4xx_hal_flash_ramfunc.c
    ${HAL_DIR}/src/stm32f4xx_hal_gpio.c
    ${HAL_DIR}/src/stm32f4xx_hal_i2c.c
    ${HAL_DIR}/src/stm32f4xx_hal_i2c_ex.c
    ${HAL_DIR}/src/stm32f4xx_hal_pcd.c
    ${HAL_DIR}/src/stm32f4xx_hal_pcd_ex.c
    ${HAL_DIR}/src/stm32f4xx_hal_pwr.c
    ${HAL_DIR}/src/stm32f4xx_hal_pwr_ex.c
    ${HAL_DIR}/src/stm32f4xx_hal_rcc.c
    ${HAL_DIR}/src/stm32f4xx_hal_rcc_ex.c
    ${HAL_DIR}/src/stm32f4xx_hal_spi.c
    ${HAL_DIR}/src/stm32f4xx_hal_tim.c
    ${HAL_DIR}/src/stm32f4xx_hal_tim_ex.c
    ${HAL_DIR}/src/stm32f4xx_hal_uart.c
    ${HAL_DIR}/src/stm32f4xx_ll_adc.c
    ${HAL_DIR}/src/stm32f4xx_ll_usb.c
)
add_library(stm32f4_hal STATIC ${HAL_SOURCES})
target_include_directories(stm32f4_hal PUBLIC ${HAL_INCLUDES})
target_compile_definitions(stm32f4_hal PUBLIC USE_HAL_DRIVER STM32F446xx)

#---------- FreeRTOS ----------#
set(FREERTOS_INCLUDES
    ${MIDDLEWARES_DIR}
    ${FREERTOS_DIR}/include
    ${FREERTOS_DIR}/CMSIS_RTOS_V2
    ${FREERTOS_DIR}/portable/GCC/ARM_CM4F
    ${CMAKE_SOURCE_DIR}/src/system # For FreeRTOSConfig.h
)
set(FREERTOS_SOURCES
    ${FREERTOS_DIR}/tasks.c
    ${FREERTOS_DIR}/queue.c
    ${FREERTOS_DIR}/list.c
    ${FREERTOS_DIR}/timers.c
    ${FREERTOS_DIR}/event_groups.c
    ${FREERTOS_DIR}/stream_buffer.c
    ${FREERTOS_DIR}/portable/GCC/ARM_CM4F/port.c
    ${FREERTOS_DIR}/portable/MemMang/heap_4.c
    ${FREERTOS_DIR}/CMSIS_RTOS_V2/cmsis_os2.c
)
add_library(freertos STATIC ${FREERTOS_SOURCES})
target_include_directories(freertos PUBLIC ${FREERTOS_INCLUDES})
target_link_libraries(freertos PRIVATE stm32f4_hal)

#---------- USB ----------#
set(USB_INCLUDES
    ${USB_DEVICE_LIB_DIR}/Class/CDC/Inc
    ${USB_DEVICE_LIB_DIR}/Core/Inc
    ${USB_DEVICE_DIR}/App
    ${USB_DEVICE_DIR}/Target
)
set(USB_SOURCES
    ${USB_DEVICE_LIB_DIR}/Core/Src/usbd_core.c
    ${USB_DEVICE_LIB_DIR}/Core/Src/usbd_ctlreq.c
    ${USB_DEVICE_LIB_DIR}/Core/Src/usbd_ioreq.c
    ${USB_DEVICE_LIB_DIR}/Class/CDC/Src/usbd_cdc.c
    ${USB_DEVICE_DIR}/App/usb_device.c
    ${USB_DEVICE_DIR}/App/usbd_cdc_if.c
    ${USB_DEVICE_DIR}/App/usbd_desc.c
    ${USB_DEVICE_DIR}/Target/usbd_conf.c
)
add_library(stm32_usb STATIC ${USB_SOURCES})
target_include_directories(stm32_usb PUBLIC ${USB_INCLUDES})
target_link_libraries(stm32_usb PRIVATE stm32f4_hal)

#---------- BLDC ----------#
set(BLDC_SOURCES
    src/drivers/adc/adc.cpp
    src/drivers/clock/clock.cpp
    src/drivers/current/current.cpp
    src/drivers/dma/dma.cpp
    src/drivers/encoder/encoder.cpp
    src/drivers/gpio/gpio.cpp
    src/drivers/i2c/i2c.cpp
    src/drivers/imu/imu.cpp
    src/drivers/interrupt/interrupt.cpp
    src/drivers/led/led.cpp
    src/drivers/motor/motor.cpp
    src/drivers/phase/phase.cpp
    src/drivers/spi/spi.cpp
    src/drivers/timer/timer.cpp
    src/drivers/uart/uart.cpp
    src/drivers/usb/usb.cpp
    src/drivers/voltage/voltage.cpp
    src/drivers/hardware.cpp

    src/system/stm32f4xx_hal_msp.c
    src/system/syscalls.c
    src/system/sysmem.c
    src/system/system_stm32f4xx.c

    src/tasks/tasks.cpp

    ../common/attaConnector.cpp
    src/utils/attaConnectorPlatform.cpp
    src/utils/circularBuffer.cpp
    src/utils/error.cpp
    src/utils/log.cpp

    ../controller/controller.cpp
    ../controller/focController.cpp
    ../controller/trapezoidalController.cpp

    src/main.cpp

    base/startup/STM32F4xx/STM32F446xx.s
)

# Main target
add_executable(bldc_motor_controller ${BLDC_SOURCES})
target_include_directories(bldc_motor_controller PRIVATE ..)# Include repository root
target_include_directories(bldc_motor_controller PRIVATE src/utils)
target_link_libraries(bldc_motor_controller PRIVATE stm32f4_hal stm32_usb freertos)
set_target_properties(bldc_motor_controller PROPERTIES OUTPUT_NAME "firmware.elf")
add_custom_command(TARGET bldc_motor_controller POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary $<TARGET_FILE:bldc_motor_controller> $<TARGET_FILE_DIR:bldc_motor_controller>/firmware.bin
)
