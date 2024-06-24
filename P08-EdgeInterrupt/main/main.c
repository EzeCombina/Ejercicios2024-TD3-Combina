/**
 * @file main.c
 * @author Ezequiel Combina
 * @brief 
 * @version 0.1
 * @date 2024-06-23  
 * @copyright Copyright (c) 2024
 * 
 */

/*
FALTA AGREGAR EL ANTIRREBOTE EN EL PULSADOR 
*/

/*==================[Inclusiones]======================*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_intr_alloc.h"

/*==================[Definiciones]======================*/
#define PERIODO_MS      5000
#define PERIODO         pdMS_TO_TICKS(PERIODO_MS)
#define PROCESADORA     0
#define PROCESADORB     1

#define PIN_SALIDA_1    GPIO_NUM_25
#define PIN_SALIDA_2    GPIO_NUM_26
#define PIN_ENTRADA_1   GPIO_NUM_12
#define PIN_ENTRADA_2   GPIO_NUM_13

/*==================[Variables]======================*/
static const char *TAG = "MAIN";

/*==================[Colas]======================*/
static QueueHandle_t cola_interrupciones;   // Declaración de la cola

/*==================[Prototipos de funciones]======================*/
void TaskA(void *taskParmPtr);       //Prototipo de la función de la tarea 
void TaskB(void *taskParmPtr);

/*==================[Main]======================*/
void app_main()
{
    // Crear cola para manejar interrupciones
    cola_interrupciones = xQueueCreate(1, sizeof(uint32_t));

    BaseType_t errA = xTaskCreatePinnedToCore(
        TaskA,                     	        // Funcion de la tarea a ejecutar
        "TaskA",   	                        // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
        NULL,                          	    // Parametros de tarea
        tskIDLE_PRIORITY+1,         	    // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        PROCESADORA                         // Numero de procesador
    );

    // Gestion de errores
    if(errA == pdFAIL)
    {
        ESP_LOGI(TAG, "Error al crear la tarea.");
        while(1);    // Si no pudo crear la tarea queda en un bucle infinito
    }else{ESP_LOGI(TAG, "Tarea creada correctamente");}

    BaseType_t errB = xTaskCreatePinnedToCore(
        TaskB,                     	        // Funcion de la tarea a ejecutar
        "TaskB",   	                        // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
        NULL,                          	    // Parametros de tarea
        tskIDLE_PRIORITY+1,         	    // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        PROCESADORA                         // Numero de procesador
    );

    // Gestion de errores
    if(errB == pdFAIL)
    {
        ESP_LOGI(TAG, "Error al crear la tarea.");
        while(1);    // Si no pudo crear la tarea queda en un bucle infinito
    }else{ESP_LOGI(TAG, "Tarea creada correctamente");}
}

/*==================[Funciones]======================*/
static void IRAM_ATTR isr_handler(void* arg)    // ISR de la interrupción
{
    //BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t pin = (uint32_t) arg;
    //uint32_t estado = gpio_get_level(pin);
    xQueueSendFromISR(cola_interrupciones, &pin, NULL);

    //if(xHigherPriorityTaskWoken != pdFALSE)
    //{
    //    portYIELD_FROM_ISR();
    //}
}

/*==================[Implementacion de la tarea]======================*/
void TaskA(void *taskParmPtr)
{
    /*==================[Configuraciones]======================*/
    uint8_t i = (uint8_t) taskParmPtr; // Recibimos el indice led correspondiente a la tarea
    
    // Configuración de pines de salida con interrupciones
    esp_rom_gpio_pad_select_gpio(PIN_SALIDA_1);
    gpio_set_direction(PIN_SALIDA_1, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(PIN_SALIDA_2);
    gpio_set_direction(PIN_SALIDA_2, GPIO_MODE_OUTPUT);
    
    TickType_t xPeriodicity =  PERIODO;
	TickType_t xLastWakeTime = xTaskGetTickCount();

    /*==================[Bucle]======================*/
    while(1)
    {
        gpio_set_level(PIN_SALIDA_1, 1);
        gpio_set_level(PIN_SALIDA_2, 0);
        vTaskDelay(xPeriodicity/2);
        gpio_set_level(PIN_SALIDA_1, 0);
        gpio_set_level(PIN_SALIDA_2, 1);
        vTaskDelayUntil(&xLastWakeTime, xPeriodicity);
    }
}

void TaskB(void *taskParmPtr)
{
    /*==================[Configuraciones]======================*/
    uint8_t i = (uint8_t) taskParmPtr; // Recibimos el indice led correspondiente a la tarea
    
    // Configuración de pines de entrada con interrupciones
    esp_rom_gpio_pad_select_gpio(PIN_ENTRADA_1);
    gpio_set_direction(PIN_ENTRADA_1, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_ENTRADA_1, GPIO_PULLDOWN_ONLY);
    gpio_set_intr_type(PIN_ENTRADA_1, GPIO_INTR_POSEDGE);

    esp_rom_gpio_pad_select_gpio(PIN_ENTRADA_2);
    gpio_set_direction(PIN_ENTRADA_2, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_ENTRADA_2, GPIO_PULLDOWN_ONLY);
    gpio_set_intr_type(PIN_ENTRADA_2, GPIO_INTR_ANYEDGE);

    // Instalar ISR
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    gpio_isr_handler_add(PIN_ENTRADA_1, isr_handler, (void*) PIN_ENTRADA_1);
    gpio_isr_handler_add(PIN_ENTRADA_2, isr_handler, (void*) PIN_ENTRADA_2);

    uint32_t pin_;
    uint32_t estado;

    /*==================[Bucle]======================*/
    while(1)
    {
        if(xQueueReceive(cola_interrupciones, &pin_, portMAX_DELAY) == pdTRUE) 
        {
            estado = gpio_get_level(pin_);
            ESP_LOGI(TAG, "Interrupción en pin %ld, estado: %ld", pin_, estado);
        } 
    }
}