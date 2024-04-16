#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "trafficLights.c"
#include "pulsador.c"
//#include "../include/led.h"
//#include "../include/trafficLights.h"
//#include "../include/pulsador.h"

#define T 10

void app_main()
{
    inicializarBoton();
    iniciarSemaforo();

    while(1)
    {
        actualizarBoton();
        vTaskDelay(T / portTICK_PERIOD_MS);
        actualizarSemaforo();
    }
}