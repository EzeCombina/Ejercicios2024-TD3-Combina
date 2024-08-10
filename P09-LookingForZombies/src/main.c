#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)  {

    int rv = 0;
    int childExit;

    pid_t pid;
    pid = fork();

    if(pid == 0)
    {
        printf("PID Hijo -> %d\n", getpid());
        sleep(20);
        //sleep(10);
        printf("El proceso hijo a finalizado\n");
        exit(rv);
    }
    else
    {
        printf("PID Padre -> %d\n", getppid());
        sleep(10);
        //sleep(20);
        wait(&childExit);
        printf("El proceso padre a finalizado\n");
    }

}

/*

    Cuando termina el proceso padre al proceso hijo se le asigna otro proceso padre, en este caso con un ppid de 1.
    Este mismo proceso se termina de ejecutar normalmente con su nuevo proceso padre. 

    En caso de que el tiempo de ejecucion del hijo sea menor al del padre cuando este termine quedará en modo "defunct". Cuando el proceso
    padre termine se eliminan ambos procesos. 

    Cuando se usa la funcion "wait" el proceso padre espera a que termine de ejecutar el proceso hijo para terminar. 

*/