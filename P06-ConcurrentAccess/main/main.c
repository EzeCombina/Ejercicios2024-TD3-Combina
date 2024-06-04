/**
 * @file main.c
 * @author Ezequiel Combina
 * @brief 
 * @version 0.1
 * @date 2024-06-02  
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
#define PERIODO_MS      2000
#define PERIODO         pdMS_TO_TICKS(PERIODO_MS)
#define PROCESADORA     0
#define PROCESADORB     1
#define N_LED           2

/*==================[Variables globales]======================*/
gpio_int_type_t led[N_LED] = {GPIO_NUM_25, GPIO_NUM_26};

extern pulsadorInfo pulsador[N_PULSADOR];

/*==================[Prototipos de funciones]======================*/
void TaskB(void *taskParmPtr);       //Prototipo de la función de la tarea
void TaskC(void *taskParmPtr);  

/*==================[Main]======================*/
void app_main()
{
    inicializarPulsador();

    BaseType_t errB = xTaskCreatePinnedToCore(
        TaskB,                     	        // Funcion de la tarea a ejecutar
        "TaskB",   	                        // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
        0,                          	    // Parametros de tarea
        tskIDLE_PRIORITY+1,         	    // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        PROCESADORA                         // Numero de procesador
    );

    // Gestion de errores
    if(errB == pdFAIL)
    {
        ESP_LOGI(TAG, "Error al crear la tarea.");
        while(1);    // Si no pudo crear la tarea queda en un bucle infinito
    }

    BaseType_t errC = xTaskCreatePinnedToCore(
        TaskC,                     	        // Funcion de la tarea a ejecutar
        "TaskC",   	                        // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
        1,                          	    // Parametros de tarea
        tskIDLE_PRIORITY+1,         	    // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        PROCESADORA                         // Numero de procesador
    );

    // Gestion de errores
    if(errC == pdFAIL)
    {
        ESP_LOGI(TAG, "Error al crear la tarea.");
        while(1);    // Si no pudo crear la tarea queda en un bucle infinito
    }
}

/*==================[Implementacion de la tarea]======================*/
void TaskB(void *taskParmPtr)
{
    /*==================[Configuraciones]======================*/
    uint8_t i = (uint8_t) taskParmPtr; // Recibimos el indice led correspondiente a la tarea
    
    esp_rom_gpio_pad_select_gpio(led[i]);
    gpio_set_direction(led[i], GPIO_MODE_OUTPUT);
    
    TickType_t xPeriodicityTaskB; 
    TickType_t xLastWakeTimeTaskB = xTaskGetTickCount();

    /*==================[Bucle]======================*/
    while(1)
    {   
        contador = ObtenerCont();
        xPeriodicityTaskB = pdMS_TO_TICKS(contador);

        xLastWakeTimeTaskB = xTaskGetTickCount();
        gpio_set_level(led[i], 1);
        vTaskDelay(xPeriodicityTaskB / 2);
        gpio_set_level(led[i], 0);
        vTaskDelayUntil(&xLastWakeTimeTaskB, xPeriodicityTaskB);
    } 
}

void TaskC(void *taskParmPtr)
{
    /*==================[Configuraciones]======================*/
    uint8_t i = (uint8_t) taskParmPtr; // Recibimos el indice led correspondiente a la tarea
    
    esp_rom_gpio_pad_select_gpio(led[i]);
    gpio_set_direction(led[i], GPIO_MODE_OUTPUT);

    TickType_t xPeriodicityTaskC = PERIODO; 
    TickType_t xLastWakeTimeTaskC = xTaskGetTickCount();

    uint16_t contador;
    TickType_t tiempo;

    /*==================[Bucle]======================*/
    while(1)
    {   
        contador = ObtenerCont();
        tiempo = pdMS_TO_TICKS(contador);

        xLastWakeTimeTaskC = xTaskGetTickCount();
        gpio_set_level(led[i], 1);
        vTaskDelay(tiempo*2);
        gpio_set_level(led[i], 0);
        vTaskDelayUntil(&xLastWakeTimeTaskC, xPeriodicityTaskC);

        ESP_LOGI(TAG, "El valor del contador actual es de %d", contador);

        DecrementarCont();
    } 
}