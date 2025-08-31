//--------------------------------------------------
// BLDC Motor Controller
// tasks.cpp
// Date: 2025-08-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <tasks/tasks.h>
#include <utils/log.h>

#include <drivers/current/current.h>
#include <drivers/voltage/voltage.h>

// FreeRTOS includes
#include <FreeRTOS.h>
#include <cmsis_os.h>
#include <task.h>

static void controllerTask(void* argument);
static void ledTask(void* argument);
static void attaConnectorTask(void* argument);

bool Tasks::init() {
    // Create tasks
    xTaskCreate(controllerTask, "ControllerTask", 512, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(ledTask, "LedTask", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(attaConnectorTask, "AttaConnectorTask", 512, NULL, tskIDLE_PRIORITY + 2, NULL);
    return true;
}

void controllerTask(void* argument) {
    for (;;) {
        Log::success("ControllerTask", "Running...");
        osDelay(1000);
    }
}

void ledTask(void* argument) {
    for (;;) {
        Log::success("LedTask", "Running...");
        osDelay(1000);
    }
}

void attaConnectorTask(void* argument) {
    for (;;) {
        float v_src = volt_src.read();
        float v_u = volt_u_phase.read();
        float v_v = volt_v_phase.read();
        float v_w = volt_w_phase.read();
        Log::success("AttaConnectorTask", "Voltages: SRC=$0V, U=$1V, V=$2V, W=$3V", v_src, v_u, v_v, v_w);

        float i_u = curr_u_phase.read();
        float i_v = curr_v_phase.read();
        float i_w = curr_w_phase.read();
        Log::success("AttaConnectorTask", "Currents: U=$0A, V=$1A, W=$2A", i_u, i_v, i_w);

        Log::success("AttaConnectorTask", "Running...");
        osDelay(500);
    }
}
