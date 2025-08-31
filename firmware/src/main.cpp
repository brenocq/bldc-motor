//--------------------------------------------------
// BLDC Motor Controller
// main.cpp
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <drivers/hardware.h>
#include <utils/error.h>
#include <utils/log.h>

// FreeRTOS includes
#include <FreeRTOS.h>
#include <cmsis_os.h>
#include <tasks/tasks.h>

int main() {
    // Initialize hardware peripherals and drivers
    if (!Hardware::init())
        Error::hardFault("Failed to initialize hardware!");
    else
        Log::success("Main", "Hardware initialized");

    // Initialize FreeRTOS and tasks
    osKernelInitialize();
    if (!Tasks::init())
        Error::hardFault("Failed to initialize tasks!");
    osKernelStart();

    while (true) {
        // ...
    }
    return 0;
}
