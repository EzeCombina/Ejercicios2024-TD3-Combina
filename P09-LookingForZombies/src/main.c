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
        sleep(5);
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
    Este mismo proceso se termina de ejecutar normalmente con su nuevo proceso padre. (CUANDO SE COMENTA LA FUNCION WAIT)

    En el caso de que el tiempo de ejecución del proceso hijo sea mayor que el del padre, el proceso padre termina de ejecutarce y espera a 
    que el hijo termine para finalizar ambos procesos. (CON LA FUNCION WAIT)

    En caso de que el tiempo de ejecucion del hijo sea menor al del padre cuando este termine quedará en modo "defunct". Cuando el proceso
    padre termine se eliminan ambos procesos. (CON LA FUNCION WAIT)

*/