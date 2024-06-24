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
#define PERIODO_MS      1000
#define PERIODO         pdMS_TO_TICKS(PERIODO_MS)
#define PROCESADORA     0
#define PROCESADORB     1

/*==================[Variables globales]======================*/
gpio_int_type_t led_param[N_LED] = {GPIO_NUM_25, GPIO_NUM_26};

extern pulsadorInfo pulsador[N_PULSADOR];

static const char *TAG2 = "MAIN";

/*==================[Prototipos de funciones]======================*/
void TaskB(void *taskParmPtr);       //Prototipo de la función de la tarea 

/*==================[Main]======================*/
void app_main()
{
    inicializarLed();
    inicializarPulsador();

    //while(1){vTaskDelay(PERIODO);};

    for(int j = 0; j < N_LED; j++)
    {
        BaseType_t errB = xTaskCreatePinnedToCore(
            TaskB,                     	        // Funcion de la tarea a ejecutar
            "TaskB",   	                        // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
            j,                          	    // Parametros de tarea
            tskIDLE_PRIORITY+1+j,         	    // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            NULL,                          		// Puntero a la tarea creada en el sistema
            PROCESADORA                         // Numero de procesador
        );

        // Gestion de errores
        if(errB == pdFAIL)
        {
            ESP_LOGI(TAG2, "Error al crear la tarea.");
            while(1){vTaskDelay(PERIODO);};    // Si no pudo crear la tarea queda en un bucle infinito
        }else{ESP_LOGI(TAG2, "Tarea creada correctamente");}
    }
}

/*==================[Implementacion de la tarea]======================*/
void TaskB(void *taskParmPtr)
{
    /*==================[Configuraciones]======================*/
    uint8_t i = (uint8_t) taskParmPtr; // Recibimos el indice led correspondiente a la tarea
    
    esp_rom_gpio_pad_select_gpio(led_param[i]);
    gpio_set_direction(led_param[i], GPIO_MODE_OUTPUT);
    
    TickType_t xPeriodicity =  PERIODO;
	TickType_t xLastWakeTime = xTaskGetTickCount();

    ledInfo flag;
    flag.diferenciaTiempo = xPeriodicity/2;         // Va a comenzar con la mitad del periodo 
    flag.indice = i;    
    ledInfo *ptr_recibido[N_LED];                   // Esto se realiza ya que sino estamos consultando un puntero NULL
    ptr_recibido[i] = &flag;

    /*==================[Bucle]======================*/
    while(1)
    {
        if(xQueueReceive(pulsador[i].cola, &ptr_recibido[i], 0) == pdTRUE) // Recibo dato desde la cola sin tiempo de espera
        {
            if(ptr_recibido[i]->diferenciaTiempo > xPeriodicity)
            {
                ptr_recibido[i]->diferenciaTiempo = xPeriodicity;
            }
            //ESP_LOGI(TAG2, "Tiempo medido %d", ptr_recibido->diferenciaTiempo);
        }

        if(ptr_recibido[i]->diferenciaTiempo > TIEMPO_NO_VALIDO)
        {
            gpio_set_level(led_param[ptr_recibido[i]->indice], 1);
            vTaskDelay(ptr_recibido[i]->diferenciaTiempo);
            gpio_set_level(led_param[ptr_recibido[i]->indice], 0);
        }
        vTaskDelayUntil(&xLastWakeTime, xPeriodicity);
    }
}