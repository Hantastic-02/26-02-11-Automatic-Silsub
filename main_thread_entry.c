#include "main_thread.h"
#include "g_hardware.h"
#include <stdio.h>

/** Declaration of Tasks **/
void Task_Value_Update(void *pvParameters);
void Task_FND_Control(void *pvParameters);

/** Task Handler **/
TaskHandle_t Handle_Value_Update;
TaskHandle_t Handle_FND_Control;

/** Structure for data sharing among tasks **/
typedef struct {
    uint8_t shared_value;
} Shared_t;

Shared_t shared;

/** Semaphore **/
SemaphoreHandle_t g_status_mutex;   // Mutex

/** Main Task: Create Tasks **/
void main_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    HW_Setting();

    g_status_mutex = xSemaphoreCreateMutex();

    if (g_status_mutex != NULL)
    {
        xTaskCreate(Task_Value_Update, "Task_Value_Update", 0x100, NULL, 2, &Handle_Value_Update);
        xTaskCreate(Task_FND_Control, "Task_FND_Control", 0x100, NULL, 1, &Handle_FND_Control);

        vTaskDelete(NULL);
    }
}

/** Task 1: Update shared value **/
void Task_Value_Update(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    for (;;)
    {
        if (xSemaphoreTake(g_status_mutex, (TickType_t)10) == pdTRUE) {
            shared.shared_value += 1;

            xSemaphoreGive(g_status_mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/** Task 2: Read shared value and display it **/
void Task_FND_Control(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    uint8_t display_data = 0;

    for (;;)
    {
        if (xSemaphoreTake(g_status_mutex, (TickType_t)10) == pdTRUE) {
            display_data = shared.shared_value;

            xSemaphoreGive(g_status_mutex);
        }

        R_FND_Print_Data(display_data);
    }
}

void R_IRQ_Interrupt(external_irq_callback_args_t *p_args)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    switch(p_args->channel)
    {
        case 11:
            break;

        case 12:
            break;

        case 13:
            break;

        case 14:
            break;
    }
}





