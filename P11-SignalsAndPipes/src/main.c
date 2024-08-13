#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(void)  {

    int pipesChild2Parent[2]; // 0 -> Read ; 1 -> Write
    pipe(pipesChild2Parent);

    char message[30];

    int rv = 0;
    int childExit;

    struct sigaction sa;

    pid_t pid;
    pid = fork();

    if(pid == 0)
    {
        close(pipesChild2Parent[0]); // Bloqueamos ya que el padre lee 

        printf("PID Hijo -> %d\n", getpid());
        sleep(5);
        write(pipesChild2Parent[1], "Mensaje desde el hijo", 22);
        sleep(10);
        printf("El proceso hijo a finalizado\n");
        exit(rv);
    }
    else
    {
        close(pipesChild2Parent[1]); // Bloqueamos ya que el hijo escribe 

        printf("PID Padre -> %d\n", getppid());
        //sleep(5);

        //
        sa.sa_handler = wait(&childExit);           //
        sa.sa_flags = 0; //SA_RESTART;              // wait(&childExit); 
        sigemptyset(&sa.sa_mask);                   // 
        sigaction(SIGCHLD, &sa, NULL);              //
        //

        read(pipesChild2Parent[0], message, 22);
        printf("El mensaje leido es el siguiente -> %s\n", message);
        sleep(2);

        printf("El proceso padre a finalizado\n");
    }
}