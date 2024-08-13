#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define Cant_Puertas 3

char Nombres_Puertas[Cant_Puertas] = {'A', 'B', 'C'};
char Lectura;

typedef struct {
    char Nombre;
    char *Estado;
}Puertas;

void *printDoor(void *tid)
{
    Puertas *Puerta_Seleccionada_ptr = (Puertas*) tid;
    printf("Puerta creada -> %c\n", Puerta_Seleccionada_ptr->Nombre);

    while(1) 
    {
        if(Lectura == Puerta_Seleccionada_ptr->Nombre) 
        {
            //printf("Puerta -> %c\n", Puerta_Seleccionada_ptr->Nombre);
            if(Puerta_Seleccionada_ptr->Estado == "Cerrada") 
            {
                Puerta_Seleccionada_ptr->Estado = "Abierta";
            } 
            else 
            {
                Puerta_Seleccionada_ptr->Estado = "Cerrada";
            }

            if(Puerta_Seleccionada_ptr->Estado == "Abierta") 
            {
                printf("Puerta %c abierta, presione %c para cerrarla.\n", Puerta_Seleccionada_ptr->Nombre, Puerta_Seleccionada_ptr->Nombre);
            } 
            else 
            {
                printf("Puerta %c cerrada, presione %c para abrirla.\n", Puerta_Seleccionada_ptr->Nombre, Puerta_Seleccionada_ptr->Nombre);
            }
        }
    }
}

int main()
{	
	pthread_t threads[Cant_Puertas];
	int status, i;
    Puertas Puerta_Seleccionada[Cant_Puertas];

    for(i = 0; i < Cant_Puertas; i++)
    {   
        Puerta_Seleccionada[i].Nombre = Nombres_Puertas[i];
        Puerta_Seleccionada[i].Estado = "Cerrada";

        status = pthread_create(&threads[i], NULL, printDoor, &Puerta_Seleccionada[i]);
        if(status != 0) 
        {
            //printf("Oops. pthread create returned error code %d\n", status);
            exit(-1);
        }//else{printf("pthread created sucessfully\n");}
    }

    while(1)
    {
        scanf("%c", &Lectura);
    }
}