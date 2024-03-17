#include <stdio.h>
#include <stdlib.h>
#include "../lib/Funciones.h"

int main()
{

    struct Medicion x[CantidadMediciones];

    // Variables que va a retornar la funcion 
    int temp1 = 0, *TempMax;
    float temp2 = 0, *Prom;

    TempMax = &temp1;
    Prom = &temp2;

    // Valores aleatorios de Humedad. 
    for(int n = 0; n < CantidadMediciones; n++)
    {
        x[n].Humedad = rand() % 200;
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

    // Imprimir las mediciones de Humedad. 
    printf("Los valores medidos de humedad son: ");
    for(int n = 0; n < CantidadMediciones; n++)
    {
        if(n == CantidadMediciones - 1)
        {
            printf("%.2f", x[n].Humedad);
        }else{printf("%.2f, ", x[n].Humedad);}
    }

    // Funcion calculo del promedio de la humedad y la temperatura máxima 

    Funcion(x, &TempMax, &Prom);

    printf("\nLos valores medidos de temperatura son: ");
    for(int n = 0; n < CantidadMediciones; n++)
    {
        if(n == CantidadMediciones - 1)
        {
            printf("%d", x[n].Temperatura);
        }else{printf("%d, ", x[n].Temperatura);}
    }

    printf("\nLa temperatura maxima medida es de: %d", temp1);

    printf("\nEl promedio de la humedad es de: %.2f", temp2);

}