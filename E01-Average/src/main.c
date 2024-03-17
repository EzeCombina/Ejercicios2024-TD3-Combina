#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../lib/Funciones.h"

#define CORTE 9999

float Numeros[100];
int n = 0;

int main(void)
{

    printf("Ingresar numeros a continuacion.\n");
    printf("Para calcular el promedio ingresar \"9999\".\n");

    while(1)
    {
        scanf("%f", &Numeros[n]);
        if(Numeros[n] == CORTE)
        {
            break;
        }else{n++;}
    }

    float Prom = Promedio(Numeros, n);

    printf("El promedio es de: %.3f\n", Prom);

}