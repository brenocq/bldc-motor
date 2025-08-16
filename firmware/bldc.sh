#!/bin/bash
set -e

# --- Configuration ---
# Get the script's absolute directory
SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
SOURCE_PATH="$SCRIPT_DIR"
BUILD_PATH="$SCRIPT_DIR/build"

# Output files from the build
FIRMWARE_ELF="$BUILD_PATH/bin/firmware.elf"
FIRMWARE_BIN="$BUILD_PATH/bin/firmware.bin"

# --- Functions ---

help()
{
    echo "BLDC Motor Controller Script"
    echo
    echo "Usage: ./bldc [actions] [options]"
    echo
    echo "Actions (can be combined):"
    echo "  -b, --build       Build firmware"
    echo "  -c, --clean       Clean previous build"
    echo "  -f, --flash       Flash firmware (implies --build if not already built)"
    echo "  -t, --trace       Trace debugging. Print SWO output"
    echo "  -g, --gdb         Open OpenOCD and GDB session"
    echo
    echo "Other Options:"
    echo "  -h, --help        This help menu"
    echo
    echo "Example: ./bldc --clean --build --flash"
    echo "Example: ./bldc --build"
    exit 0
}

clean()
{
    echo "--- Cleaning build directory ---"
    rm -rf "$BUILD_PATH"
}

build()
{
    echo "--- Configuring Firmware ---"
    cmake -S "$SOURCE_PATH" -B "$BUILD_PATH"

    echo "--- Building Firmware ---"
    cmake --build "$BUILD_PATH" --parallel

    echo "--- Build finished successfully ---"
}

flash()
{
    echo "--- Flashing firmware ---"
    st-flash write "$FIRMWARE_BIN" 0x08000000
}

trace()
{
    echo "--- Starting SWO trace ---"
    st-trace --clock=144m --trace=1m
}

gdb()
{
    # Ensure OpenOCD is killed when the script exits (e.g., Ctrl+C)
    trap 'echo -e "\n--- Shutting down OpenOCD ---"; kill $OPENOCD_PID' EXIT

    echo "--- Starting OpenOCD ---"
    openocd -f "$SCRIPT_DIR/base/openocd/stm32f4.cfg" -c "gdb_port 3333" &
    OPENOCD_PID=$!

    echo "--- Starting GDB (PID: $OPENOCD_PID) ---"
    # Wait a moment for OpenOCD to start up before connecting GDB
    sleep 1
    arm-none-eabi-gdb "$FIRMWARE_ELF" -ex "target extended-remote localhost:3333"

    # The trap will handle cleanup
}

# --- Argument Parsing ---

# Action flags
DO_CLEAN=false
DO_BUILD=false
DO_FLASH=false
DO_TRACE=false
DO_GDB=false

# If no arguments are provided, show help
if [ $# -eq 0 ]; then
    help
fi

while [[ $# -gt 0 ]]; do
  case $1 in
    -h|--help)
      help
      ;;
    -c|--clear|--clean)
      DO_CLEAN=true
      shift
      ;;
    -b|--build)
      DO_BUILD=true
      shift
      ;;
    -f|--flash)
      DO_FLASH=true
      shift
      ;;
    -t|--trace)
      DO_TRACE=true
      shift
      ;;
    -g|--gdb)
      DO_GDB=true
      shift
      ;;
    -*) # Handles unknown options
      echo "Unknown option $1"
      printHelp
      ;;
  esac
done

# --- Execute Actions in Logical Order ---

if [ "$DO_CLEAN" = true ]; then
    clean
fi

# A flash or GDB session implies a build is needed if not explicitly requested
if [ "$DO_FLASH" = true ] || [ "$DO_GDB" = true ]; then
    DO_BUILD=true
fi

if [ "$DO_BUILD" = true ]; then
    build
fi

if [ "$DO_FLASH" = true ]; then
    flash
fi

# GDB and Trace are mutually exclusive; they both try to talk to the chip
if [ "$DO_GDB" = true ]; then
    gdb
elif [ "$DO_TRACE" = true ]; then
    trace
fi
