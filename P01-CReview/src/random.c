#include "../inc/funcion.h"
#include "../inc/random.h"

void random()
{

    // Valores aleatorios de Humedad. 
    for(int n = 0; n < CantidadMediciones; n++)
    {
        x[n].Humedad = rand() % 100;
    }

    // Valores aleatorios de Velocidad del viento. 
    for(int n = 0; n < CantidadMediciones; n++)
    {
        x[n].VelocidadViento = rand() % 200;
    }

    // Valores aleatorios de Temperatura. 
    for(int n = 0; n < CantidadMediciones; n++)
    {
        x[n].Temperatura = rand() % 500;
    }

}