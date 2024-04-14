#include <stdio.h>
#include "trafficLights.c"

void app_main()
{
    iniciarSemaforo();

    while(1)
    {
        actualizarSemaforo();
    }
}