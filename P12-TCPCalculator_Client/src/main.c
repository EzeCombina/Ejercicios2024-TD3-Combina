#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <netdb.h>

typedef struct
{
    char Operador;
    float Dato1;
    float Dato2;  
}Datos;

int main()
{
    Datos datos_ingresados, *ptr;

    bool c = false;

    ptr = &datos_ingresados;

    float buf, *ptr_buf;
    ptr_buf = &buf;

	int s;

	// Creamos socket PF_INET: IPV4, TCP
	s = socket(PF_INET,SOCK_STREAM, 0);

	// Cargamos datos de direccion de server
    struct addrinfo hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family     = AF_INET; // ipv4
    hints.ai_socktype   = SOCK_STREAM; // tcp
    hints.ai_flags      = AI_PASSIVE;    /* For wildcard IP address */

    int r = getaddrinfo("localhost","4096", &hints, &result); // NULL para localhost
    if (r != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
        exit(EXIT_FAILURE);
    }
    // result es lista enlazada!. En este ejemplo usamos la primera

	// Ejecutamos connect()
    if (connect(s, (const struct sockaddr *)result->ai_addr, result->ai_addrlen) < 0)
    {
        fprintf(stderr,"ERROR connecting\r\n");
        close(s);
        exit(EXIT_FAILURE);
    }

	// Enviamos mensaje a server  ---REVISAR---
    while(1)
    {
        printf("Ingrese el operador de la ecuacion a realizar: ");
        gets(&datos_ingresados.Operador);

        if(datos_ingresados.Operador == '+' || datos_ingresados.Operador == '-' || datos_ingresados.Operador == '*' || datos_ingresados.Operador == '/' || datos_ingresados.Operador == 'x' || datos_ingresados.Operador == 'X')
        {
            break;
        }
        else
        {
            printf("Dato ingresado incorrecto\n");
        }
    }

    printf("Ingrese el primer dato (Numerador en division): ");
    scanf("%f", &datos_ingresados.Dato1);

    printf("Ingrese el segundo dato (Denominador en division): ");
    scanf("%f", &datos_ingresados.Dato2);

    printf("Operador = %c - Dato1 = %.3f - Dato2 = %.3f\n", datos_ingresados.Operador, datos_ingresados.Dato1, datos_ingresados.Dato2);

    int n = send(s, ptr, sizeof(datos_ingresados), 0);
    if(n<=0)
    {
        fprintf(stderr,"ERROR sending command\r\n");
        close(s);
        exit(EXIT_FAILURE);
    }

	// Leemos respuesta de server
    n = recv(s, ptr_buf, sizeof(ptr_buf), 0);
    if(n>0)
    {
        printf("Recibi: '%.03f' \r\n", buf);
    }

	freeaddrinfo(result);
	close(s);

	return 0;
}