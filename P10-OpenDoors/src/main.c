#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define Cant_Puertas 3

char Puerta;

enum Lectura {
    A,
    B,
    C
}Cambio_Puerta;

enum BOOLEAN {
    Abierto,
    Cerrado
}Cambio_Estado[Cant_Puertas];

void *printDoor(void *tid)
{
    while(1)
    {
        scanf("%c", &Puerta);

        switch(Puerta)
        {
            case 'A':
                Cambio_Puerta = Puerta;
                if(Cambio_Estado[Cambio_Puerta] == 0)
                {
                    printf("Puerta %c abierta, presione %c para cerrarla\n", Puerta, Puerta);
                    Cambio_Estado[Cambio_Puerta] = !Cambio_Estado[Cambio_Puerta];
                }
                else
                {
                    printf("Puerta %c cerrada, presione %c para abrirla\n", Puerta, Puerta);
                    Cambio_Estado[Cambio_Puerta] = !Cambio_Estado[Cambio_Puerta];
                }
                break;

            case 'B':
                Cambio_Puerta = Puerta;
                if(Cambio_Estado[Cambio_Puerta] == 0)
                {
                    printf("Puerta %c abierta, presione %c para cerrarla\n", Puerta, Puerta);
                    Cambio_Estado[Cambio_Puerta] = !Cambio_Estado[Cambio_Puerta];
                }
                else
                {
                    printf("Puerta %c cerrada, presione %c para abrirla\n", Puerta, Puerta);
                    Cambio_Estado[Cambio_Puerta] = !Cambio_Estado[Cambio_Puerta];
                }
                break;

            case 'C':
                Cambio_Puerta = Puerta;
                if(Cambio_Estado[Cambio_Puerta] == 0)
                {
                    printf("Puerta %c abierta, presione %c para cerrarla\n", Puerta, Puerta);
                    Cambio_Estado[Cambio_Puerta] = !Cambio_Estado[Cambio_Puerta];
                }
                else
                {
                    printf("Puerta %c cerrada, presione %c para abrirla\n", Puerta, Puerta);
                    Cambio_Estado[Cambio_Puerta] = !Cambio_Estado[Cambio_Puerta];
                }
                break;
            
            /*
            case 'D':
                Cambio_Puerta = Puerta;
                if(Cambio_Estado[Cambio_Puerta] == 0)
                {
                    printf("Puerta %c abierta, presione %c para cerrarla\n", Puerta, Puerta);
                    Cambio_Estado[Cambio_Puerta] = !Cambio_Estado[Cambio_Puerta];
                }
                else
                {
                    printf("Puerta %c cerrada, presione %c para abrirla\n", Puerta, Puerta);
                    Cambio_Estado[Cambio_Puerta] = !Cambio_Estado[Cambio_Puerta];
                }
                break;
            */

            default:
                break;
        }
    }
}

int main()
{	
	pthread_t threads;
	int status;

    status = pthread_create(&threads, NULL, printDoor, NULL);
    if(status != 0) 
    {
        printf("Oops. pthread create returned error code %d\n", status);
        exit(-1);
    }else{printf("pthread created sucessfully\n");}

    pthread_join(threads, NULL); 
}