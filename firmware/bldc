#!/bin/bash
set -e

SCRIPT_PATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
BUILD_PATH="$SCRIPT_PATH/build"

printHelp()
{
   echo "BLDC Motor Controller Script"
   echo
   echo "Usage: ./bldc [option(s)]"
   echo
   echo "Options:"
   echo
   echo "-h or --help"
   echo "        This help menu"
   echo
   echo "-b or --build"
   echo "        Build firmware"
   echo
   echo "-c or --clean or --clear"
   echo "        Clean previous build"
   echo
   echo "-f or --flash"
   echo "        Flash firmware"
   echo
   echo "-t or --trace"
   echo "        Trace debugging. Print SWO output"
   echo
   echo "-g or --gdb"
   echo "        Open OpenOCD and GDB to debug the firmware"
   exit
}

build()
{
    mkdir -p $BUILD_PATH && cd $BUILD_PATH

    cmake $SCRIPT_PATH
    make -j

    exit
}

clean()
{
    rm -rf $BUILD_PATH
    exit
}

flash()
{
	st-flash write $BUILD_PATH/bin/firmware.bin 0x08000000
    exit
}

trace()
{
    st-trace --clock=144 --trace=2000000
    exit
}

gdb()
{
    cd $SCRIPT_PATH/base/openocd
    openocd -f stm32f4.cfg -c "gdb_port 3333" &
    arm-none-eabi-gdb $BUILD_PATH/bin/firmware.elf -ex "target extended-remote localhost:3333"
    exit
}

# Parse arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -h|--help)
      printHelp
      ;;
    -b|--build)
      build
      ;;
    -c|--clear|--clean)
      clean
      ;;
    -f|--flash)
      flash
      ;;
    -t|--trace)
      trace
      ;;
    -g|--gdb)
      gdb
      ;;
    -*|--*)
      echo "Unknown option $1"
      echo "Tip: ./bldc --help"
      exit
      ;;
  esac
done
