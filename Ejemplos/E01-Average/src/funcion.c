#include <stdio.h>
#include "../inc/Funciones.h"

float Promedio(float Numeros[], int m)
{

    float Contador = 0;

    for(int n = 0; n < m; n++)
    {
        Contador += Numeros[n];
    }

    float Prom = Contador / m; 

    return Prom;

}