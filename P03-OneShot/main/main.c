/**
 * @file main.c
 * @author Ezequiel Combina
 * @brief 
 * @version 0.1
 * @date 2024-05-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

//-------------------- Inclusiones -------------
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"                    // Para driver de E/S digitales
#include "esp_log.h"

static const char *TAG = "MAIN";

//-------------------- Definiciones -------------
#define PULSADOR    GPIO_NUM_12
#define SALIDA1     GPIO_NUM_25
#define SALIDA2     GPIO_NUM_26 
#define SALIDA3     GPIO_NUM_27
#define DELAY_US    1
#define DELAY_MS    100 / portTICK_PERIOD_MS
#define DELAY_2_MS  1000 / portTICK_PERIOD_MS

//-------------------- Prototipos -------------
void TaskA(void *pvParameters);             // Prototipo de la tarea
void TaskB(void *pvParameters);             // Prototipo de la tarea
void TaskC(void *pvParameters);             // Prototipo de la tarea
void TaskMonitor(void *pvParameters);       // Prototipo de la tarea

//-------------------- Variables, constantes y punteros globales -------------
TaskHandle_t xHandleA = NULL;               // Puntero a la tarea
TaskHandle_t xHandleB = NULL;
TaskHandle_t xHandleC = NULL;
TaskHandle_t xHandleMonitor = NULL;

//-------------------- main -------------
void app_main() 
{
    //UBaseType_t prioridad = uxTaskPriorityGet (NULL);   //para obtener la prioridad de la tarea principal
    //printf("%d/n",prioridad);   //debería ser 1

    xTaskCreatePinnedToCore(TaskA,              // Función que implementa la tarea. La misma no debe retornar.
                 "Tarea A",                     // Nombre que reprenta la tarea, para facilitar la depuración.
                 configMINIMAL_STACK_SIZE*3,    // Tamaño del stack en bytes
                 NULL,                          // Puntero que se utilizará como parámetro para la tarea que se está creando. Como no lo usamos ponemos NULL
                 tskIDLE_PRIORITY+3,            // Prioridad de la tarea
                 &xHandleA,                     // Puntero a la tarea
                 0                              // Procesador donde se ejecuta
                );
    configASSERT( xHandleA );                   // Entra si no se pudieron crear las tareas, para debug 

    // PinnedToCore es para seleccionar el nucleo con el que se va a ejecutar la tarea. 

    xTaskCreatePinnedToCore(TaskB, "Tarea B", configMINIMAL_STACK_SIZE*3, NULL, tskIDLE_PRIORITY+2, &xHandleB, 0);
    configASSERT(xHandleB);                     // Entra si no se pudieron crear las tareas, para debug

    xTaskCreatePinnedToCore(TaskMonitor, "Tarea monitoreo", configMINIMAL_STACK_SIZE*3, NULL, tskIDLE_PRIORITY+1, &xHandleMonitor, 0);
    configASSERT(xHandleMonitor);               // Entra si no se pudieron crear las tareas, para debug
}

//-------------------- Tareas -------------
void TaskA(void *pvParameters)
{
    gpio_set_direction(PULSADOR, GPIO_MODE_INPUT);
	gpio_set_pull_mode(PULSADOR, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(SALIDA1, GPIO_MODE_OUTPUT);

    while (1)
    {
        if (gpio_get_level(PULSADOR))
        {
            xTaskCreatePinnedToCore(TaskC, "Tarea C", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, &xHandleC, 0);
            configASSERT(xHandleC); 
        }
        gpio_set_level(SALIDA1, 1);
        //ets_delay_us(DELAY_US);    //delay en microsegundos que bloquea al procesador
        vTaskDelay(DELAY_MS);
        gpio_set_level(SALIDA1, 0);
        vTaskDelay(DELAY_MS);
    }

    while (1)
    {
        gpio_set_level(SALIDA1, 1);
        //ESP_LOGI(TAG, "Led 1 encendido");
        vTaskDelay(DELAY_MS);
        gpio_set_level(SALIDA1, 0);
        //ESP_LOGI(TAG, "Led 1 apagado");
        vTaskDelay(DELAY_MS);
    }
}

void TaskB(void *pvParameters)
{
    gpio_set_direction(SALIDA2, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(SALIDA2, 1);
        //ESP_LOGI(TAG, "Led 2 encendido");
        vTaskDelay(DELAY_MS);
        gpio_set_level(SALIDA2, 0);
        //ESP_LOGI(TAG, "Led 2 apagado");
        vTaskDelay(DELAY_MS);
    }
}

void TaskC(void *pvParameters)
{
    gpio_set_direction(SALIDA3, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(SALIDA3, 1);
        //ESP_LOGI(TAG, "Led 3 encendido");
        vTaskDelay(DELAY_2_MS);
        gpio_set_level(SALIDA3, 0);
        //ESP_LOGI(TAG, "Led 3 apagado");
        //vTaskDelay(DELAY_2_MS);

        //Borra la tarea usando el handle
        if(xHandleC != NULL)
        {
            vTaskDelete(xHandleC);
        }
    }
}


void TaskMonitor(void *pvParameters)
{
    while(true)
    {
        //Muestra el espacio mínimo libre de stack que ha habido desde que comenzó la tarea.
        //Cuanto menor sea el número devuelto, más cerca está la tarea de desbordar su stack.
        ESP_LOGI(TAG, "Task %u min %u bytes", 1,  uxTaskGetStackHighWaterMark(xHandleA));
        ESP_LOGI(TAG, "Task %u min %u bytes", 2,  uxTaskGetStackHighWaterMark(xHandleB));
        ESP_LOGI(TAG, "Task %u min %u bytes", 3,  uxTaskGetStackHighWaterMark(xHandleC));
        vTaskDelay(DELAY_MS);
    }
}
