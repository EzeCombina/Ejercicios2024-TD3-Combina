#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define Cant_Puertas 4

char *Nombres_Puertas[Cant_Puertas] = {"A", "B", "C", "Hola"};
char Lectura[100] = "\0", *Lectura_ptr;

typedef struct {
    char *Nombre;
    char *Estado;
}Puertas;

void *printDoor(void *tid)
{
    Puertas *Puerta_Seleccionada_ptr = (Puertas*) tid;
    printf("Puerta creada -> %s\n", Puerta_Seleccionada_ptr->Nombre);

    while(1) 
    {
        if(!strcmp(Lectura_ptr, Puerta_Seleccionada_ptr->Nombre)) 
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
                printf("Puerta %s abierta, presione %s para cerrarla.\n", Puerta_Seleccionada_ptr->Nombre, Puerta_Seleccionada_ptr->Nombre);
                Lectura_ptr = "\0"; 
            } 
            else 
            {
                printf("Puerta %s cerrada, presione %s para abrirla.\n", Puerta_Seleccionada_ptr->Nombre, Puerta_Seleccionada_ptr->Nombre);
                Lectura_ptr = "\0";
            }
        }
    }
}

int main()
{	
	pthread_t threads[Cant_Puertas];
	int status, i, k, cont;
    Puertas Puerta_Seleccionada[Cant_Puertas];

    cont = 0;

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
        if(Lectura != '\0' || Lectura != NULL)
        {
            Lectura_ptr = (char*)Lectura;
            for(k = 0; k < Cant_Puertas; k++)
            {
                if(strcmp(Lectura_ptr, Nombres_Puertas[k]) && strcmp(Lectura_ptr, "\0"))
                {
                    cont++;        
                }
            }
            if(cont == Cant_Puertas)
            {
                printf("Valor de puerta ingresado incorrecto.\n");
                cont = 0;
            }else{cont = 0;}
        }

        scanf("%s", Lectura);
        //scanf("%s", Lectura);
    }
}