#include <stdio.h>
#include <stdlib.h>
#include "../inc/Funciones.h"

void Funcion(struct Medicion a[], int **TempMax, float **Prom)
{

    // Temperatura Max
    int Temp_Inicial = a[0].Temperatura; 

    for(int i = 1; i < CantidadMediciones - 1; i++)
    {
        if(a[i].Temperatura > Temp_Inicial)
        {
            Temp_Inicial = a[i].Temperatura;
        }
    }

    **TempMax = Temp_Inicial;

    // Promedio 
    float Contador = 0;

    for(int j = 0; j < CantidadMediciones; j++)
    {
        Contador += a[j].Humedad;
    }

    **Prom = Contador / CantidadMediciones; 

}