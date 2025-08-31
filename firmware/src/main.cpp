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
#include <task.h>

/* Definitions for taskOne */
osThreadId_t taskOneHandle;
const osThreadAttr_t taskOne_attributes = {
    .name = "taskOne",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for taskTwo */
osThreadId_t taskTwoHandle;
const osThreadAttr_t taskTwo_attributes = {
    .name = "taskTwo",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

void StartTaskOne(void* argument);
void StartTaskTwo(void* argument);

int main() {
    if (!Hardware::init())
        Error::hardFault("Failed to initialize hardware!");
    Log::success("Main", "Initialized");

    osKernelInitialize();
    taskOneHandle = osThreadNew(StartTaskOne, NULL, &taskOne_attributes);
    taskTwoHandle = osThreadNew(StartTaskTwo, NULL, &taskTwo_attributes);
    osKernelStart();

    while (true) {
        // ...
    }
    return 0;
}

void StartTaskOne(void* argument) {
    for (;;) {
        Log::success("Main", "Task 1");
        // Uart::update();
        osDelay(500);
    }
}

void StartTaskTwo(void* argument) {
    for (;;) {
        Log::success("Main", "Task 2");
        // Uart::update();
        osDelay(1000);
    }
}
