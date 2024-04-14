#include "../inc/funcion.h"

void Funcion()
{

    // Temperatura Max
    int Temp_Inicial = x[0].Temperatura; 

    for(int i = 1; i < CantidadMediciones - 1; i++)
    {
        if(x[i].Temperatura > Temp_Inicial)
        {
            Temp_Inicial = x[i].Temperatura;
        }
    }

    TempMax = Temp_Inicial;

    // Promedio 
    float Contador = 0;

    for(int j = 0; j < CantidadMediciones; j++)
    {
        Contador += x[j].Humedad;
    }

    Prom = Contador / CantidadMediciones; 

}