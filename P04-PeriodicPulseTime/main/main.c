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

/*==================[Variables globales]======================*/
gpio_int_type_t led[N_PULSADOR] = {GPIO_NUM_25};

//static const char *TAG = "MAIN";

/*==================[Prototipos de funciones]======================*/
void TaskLed(void *taskParmPtr);       //Prototipo de la función de la tarea
//void tarea_tecla(void* taskParmPtr);

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
            TaskLed,                     	    // Funcion de la tarea a ejecutar
            "TaskLed",   	                    // Nombre de la tarea como String amigable para el usuario
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
void TaskLed(void *taskParmPtr)
{
    /*==================[Configuraciones]======================*/
    uint8_t indice = (uint8_t) taskParmPtr; // Recibimos el indice led correspondiente a la tarea

    esp_rom_gpio_pad_select_gpio(led[indice]);
    gpio_set_direction(led[indice], GPIO_MODE_OUTPUT);

    TickType_t dif;
    
    TickType_t xPeriodicity = PERIODO; // Tarea periodica

    TickType_t xLastWakeTime = xTaskGetTickCount();

    TickType_t periodo = PERIODO;

    /*==================[Bucle]======================*/
    while(1)
    {
        dif = obtenerDiferencia(indice, periodo);

        if(dif > TIEMPO_NO_VALIDO)
        {
            gpio_set_level(led[indice], 1);
            vTaskDelay(dif);
            
            gpio_set_level(led[indice], 0);
            vTaskDelayUntil(&xLastWakeTime, xPeriodicity);
        }else{vTaskDelayUntil(&xLastWakeTime, xPeriodicity);}
    } 
}