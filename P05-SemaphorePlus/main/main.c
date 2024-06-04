/**
 * @file main.c
 * @author Ezequiel Combina
 * @brief Modificacion del Duty Cycle de una señal mediante el tiempo de pulsado de un boton 
 * @version 0.1
 * @date 2024-05-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
/*==================[Inclusiones]======================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "pulsador.c"   

/*==================[Definiciones]======================*/
#define T_ESPERA_MS     40
#define T_ESPERA        pdMS_TO_TICKS(T_ESPERA_MS)
#define PERIODO_MS      1000
#define PERIODO         pdMS_TO_TICKS(PERIODO_MS)
#define PROCESADORA     0
#define PROCESADORB     1
#define N_LED           N_PULSADOR*2

/*==================[Variables globales]======================*/
gpio_int_type_t led[N_LED] = {GPIO_NUM_25, GPIO_NUM_26};

extern pulsadorInfo pulsador[N_PULSADOR];

/*==================[Prototipos de funciones]======================*/
void TaskDestello(void *taskParmPtr);       //Prototipo de la función de la tarea

/*==================[Main]======================*/
void app_main()
{
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    inicializarPulsador();

    for(int i = 0; i < N_PULSADOR; i++)
    {
        BaseType_t err = xTaskCreatePinnedToCore(
            TaskDestello,                     	// Funcion de la tarea a ejecutar
            "TaskDestello",   	                // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
            i,                          	    // Parametros de tarea
            tskIDLE_PRIORITY+i,         	    // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            NULL,                          		// Puntero a la tarea creada en el sistema
            PROCESADORA                         // Numero de procesador
        );

        // Gestion de errores
        if(err == pdFAIL)
        {
            ESP_LOGI(TAG, "Error al crear la tarea.");
            while(1);    // Si no pudo crear la tarea queda en un bucle infinito
        }
    }
}

/*==================[Implementacion de la tarea]======================*/
void TaskDestello(void *taskParmPtr)
{
    /*==================[Configuraciones]======================*/
    uint8_t indice = (uint8_t) taskParmPtr; // Recibimos el indice led correspondiente a la tarea
    
    for(int i = 0; i < N_LED; i++)
    {
        esp_rom_gpio_pad_select_gpio(led[i]);
        gpio_set_direction(led[i], GPIO_MODE_OUTPUT);
    }

    TickType_t xPeriodicity = PERIODO; 

    TickType_t xLastWakeTime = xTaskGetTickCount();

    uint8_t contador;

    /*==================[Bucle]======================*/
    while(1)
    {   
        contador = (uint8_t)uxSemaphoreGetCount(pulsador[indice].semaforo);
        ESP_LOGI(TAG, "Tarea %d. Quedan %d semaforos",indice, contador);
        if(xSemaphoreTake(pulsador[indice].semaforo, NULL) == pdTRUE)
        {
            xLastWakeTime = xTaskGetTickCount();
            gpio_set_level(led[indice], 1);
            vTaskDelay(xPeriodicity / 2);
            gpio_set_level(led[indice], 0);
            vTaskDelayUntil(&xLastWakeTime, xPeriodicity);
        }
        else
        {
            xLastWakeTime = xTaskGetTickCount();
            gpio_set_level(led[indice+1], 1);
            vTaskDelay(xPeriodicity / 2);
            gpio_set_level(led[indice+1], 0);
            vTaskDelayUntil(&xLastWakeTime, xPeriodicity);
        }
    } 
}