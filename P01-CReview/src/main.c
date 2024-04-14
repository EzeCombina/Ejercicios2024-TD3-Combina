#include <stdio.h>
#include <stdlib.h>
#include "random.c"
#include "funcion.c"

int TempMax = 0;
float Prom = 0.0;

int main()
{

    // Funcion para obtener valores aleatorios de mediciones
    random();

    // Imprimir las mediciones de Humedad. 
    printf("Los valores medidos de humedad son: ");
    for(int n = 0; n < CantidadMediciones; n++)
    {
        if(n == CantidadMediciones - 1)
        {
            printf("%.2f", x[n].Humedad);
        }else{printf("%.2f, ", x[n].Humedad);}
    }

    // Funcion para calcular el promedio de la humedad y la temperatura máxima 

    Funcion();

    printf("\nLos valores medidos de temperatura son: ");
    for(int n = 0; n < CantidadMediciones; n++)
    {
        if(n == CantidadMediciones - 1)
        {
            printf("%d", x[n].Temperatura);
        }else{printf("%d, ", x[n].Temperatura);}
    }

    printf("\nLa temperatura maxima medida es de: %d", TempMax);

    printf("\nEl promedio de la humedad es de: %.2f", Prom);

}