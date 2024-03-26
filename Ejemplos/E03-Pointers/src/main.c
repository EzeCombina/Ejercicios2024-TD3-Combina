#include <stdio.h>

int main(void)
{

    long int valor1 = 200000, valor2;

    // Apartado a 
    long *ptrL;

    // Apartado b
    ptrL = &valor1;

    // Apartado c
    printf("%d\n", *ptrL);

    // Apartado d
    valor2 = *ptrL;

    // Apartado e
    printf("%d\n", valor2);

    // Apartado f
    printf("%X\n", &valor1);

    // Apartado g
    printf("%X\n", ptrL);

    return 0;

}
