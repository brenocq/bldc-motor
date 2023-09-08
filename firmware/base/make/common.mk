# Check to make sure that the required variables are set
ifndef DEVICE
    $(error Please set the required DEVICE variable in your makefile.)
endif

ifndef FLASH
    $(error Please set the required FLASH variable in your makefile.)
endif

# Standard values for (linked) STM32-base folders
STM32_BASE_PATH   ?= ./base
STM32_CUBE_PATH   ?= $(STM32_BASE_PATH)/cube

# STM32-base sub-folders
BASE_LINKER   = $(STM32_BASE_PATH)/linker
BASE_MAKE     = $(STM32_BASE_PATH)/make
BASE_STARTUP  = $(STM32_BASE_PATH)/startup

# Standard values for project folders
BUILD_FOLDER ?= ./build
SRC_FOLDER ?= ./src

# Determine the series folder name
include $(BASE_MAKE)/series_folder_name.mk

# Include the series-specific makefile
include $(BASE_MAKE)/$(SERIES_FOLDER)/common.mk
MAPPED_DEVICE ?= $(DEVICE)

# The toolchain path, defaults to using the globally installed toolchain
CC      = arm-none-eabi-gcc
CXX     = arm-none-eabi-g++
LD      = arm-none-eabi-ld -v
AR      = arm-none-eabi-ar
AS      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size
GDB			= arm-none-eabi-gdb


# Flags - Overall Options
CPPFLAGS += -specs=nosys.specs
CPPFLAGS += -std=gnu++17

# Flags - C Language Options
CFLAGS += -ffreestanding

# Flags - C++ Language Options
CXXFLAGS += -fno-threadsafe-statics
CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fno-unwind-tables

# Flags - Warning Options
CPPFLAGS += -Wall
CPPFLAGS += -Wextra
CPPFLAGS += -Wno-unused-parameter -Wno-missing-field-initializers

# Flags - Debugging Options
CPPFLAGS += -g

# Flags - Optimization Options
CPPFLAGS += -ffunction-sections
CPPFLAGS += -fdata-sections

# Flags - Preprocessor options
CPPFLAGS += -D $(MAPPED_DEVICE)

# Flags - Assembler Options
ifneq (,$(or USE_ST_CMSIS, USE_ST_HAL))
    CPPFLAGS += -Wa,--defsym,CALL_ARM_SYSTEM_INIT=1
endif

# Flags - Linker Options
# CPPFLAGS += -nostdlib
CPPFLAGS += -Wl,-L$(BASE_LINKER),-T$(BASE_LINKER)/$(SERIES_FOLDER)/$(DEVICE).ld

# Flags - Directory Options
CPPFLAGS += -I$(SRC_FOLDER)
CPPFLAGS += -I$(SRC_FOLDER)/system
CPPFLAGS += -I$(BASE_STARTUP)

# Flags - Machine-dependant options
CPPFLAGS += -mcpu=$(SERIES_CPU)
#CPPFLAGS += -march=$(SERIES_ARCH)

CPPFLAGS += -mfloat-abi=hard
CPPFLAGS += -mfpu=$(SERIES_FPU)

CPPFLAGS += -mlittle-endian
CPPFLAGS += -mthumb
CPPFLAGS += -masm-syntax-unified

# Output files
ELF_FILE_NAME ?= stm32_executable.elf
BIN_FILE_NAME ?= stm32_bin_image.bin
OBJ_FILE_NAME ?= startup_$(MAPPED_DEVICE).o

ELF_FILE_PATH = $(BUILD_FOLDER)/$(ELF_FILE_NAME)
BIN_FILE_PATH = $(BUILD_FOLDER)/$(BIN_FILE_NAME)
OBJ_FILE_PATH = $(BUILD_FOLDER)/$(OBJ_FILE_NAME)

# Input files
SRC ?=
SRC += $(shell find $(SRC_FOLDER) -type f -name '*.c')
SRC += $(shell find $(SRC_FOLDER) -type f -name '*.cpp')

# Startup file
DEVICE_STARTUP = $(BASE_STARTUP)/$(SERIES_FOLDER)/$(MAPPED_DEVICE).s

# Include the CMSIS files, using the HAL implies using the CMSIS
ifneq (,$(or USE_ST_CMSIS, USE_ST_HAL))
    CPPFLAGS += -I$(STM32_CUBE_PATH)/CMSIS/ARM/inc
    CPPFLAGS += -I$(STM32_CUBE_PATH)/CMSIS/$(SERIES_FOLDER)/inc

    SRC += $(wildcard $(STM32_CUBE_PATH)/CMSIS/$(SERIES_FOLDER)/src/*.c)
endif

# Include the HAL files
ifdef USE_ST_HAL
    CPPFLAGS += -D USE_HAL_DRIVER
    CPPFLAGS += -I$(STM32_CUBE_PATH)
    CPPFLAGS += -I$(STM32_CUBE_PATH)/HAL/$(SERIES_FOLDER)/inc
	# USB
    CPPFLAGS += -I$(STM32_CUBE_PATH)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    CPPFLAGS += -I$(STM32_CUBE_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    CPPFLAGS += -I$(STM32_CUBE_PATH)/USB_DEVICE/App
    CPPFLAGS += -I$(STM32_CUBE_PATH)/USB_DEVICE/Target

    # A simply expanded variable is used here to perform the find command only once.
    HAL_SRC := $(shell find $(STM32_CUBE_PATH)/HAL/$(SERIES_FOLDER)/src/*.c ! -name '*_template.c')
	# USB
    HAL_SRC += $(shell find $(STM32_CUBE_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c)
    HAL_SRC += $(shell find $(STM32_CUBE_PATH)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/*.c)
    HAL_SRC += $(shell find $(STM32_CUBE_PATH)/USB_DEVICE/Target/*.c)
    HAL_SRC += $(shell find $(STM32_CUBE_PATH)/USB_DEVICE/App/*.c)

    SRC += $(HAL_SRC)
endif

# Make all
all:$(BIN_FILE_PATH)

$(BIN_FILE_PATH): $(ELF_FILE_PATH)
	$(OBJCOPY) -O binary $^ $@

$(ELF_FILE_PATH): $(SRC) $(OBJ_FILE_PATH) | $(BUILD_FOLDER)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(OBJ_FILE_PATH): $(DEVICE_STARTUP) | $(BUILD_FOLDER)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(BUILD_FOLDER):
	mkdir $(BUILD_FOLDER)

# Make clean
clean:
	@echo "---------- CLEAN ----------"
	rm -f $(ELF_FILE_PATH)
	rm -f $(BIN_FILE_PATH)
	rm -f $(OBJ_FILE_PATH)

# Flash code
flash:
	@echo "---------- FLASH ----------"
	st-flash write $(BUILD_FOLDER)/$(BIN_FILE_NAME) $(FLASH)

# Get SWO output
trace:
	@echo "---------- TRACE ----------"
	st-trace

.PHONY: all clean flash
