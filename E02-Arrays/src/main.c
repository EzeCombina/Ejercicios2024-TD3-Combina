#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{

    // Apartado 'a'
    char f[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
    printf("%c", f[6]);

    // Apartado 'b'
    float b[5];
    b[3] = 1.1;
    
    // Apartado 'c'
    int g[5] = {8, 8, 8, 8, 8};

    // Apartado 'd'
    int g[5] = {8, 8, 8, 8, 8};

    // Apartado 'e'
    float c[100];   
    
    float sum = 0.0;

    for(int i = 0; i < 100; i++)
    {
        sum += c[i];
    }

    // Apartado 'f'
    double a[11];   
    double b[34]; 

    for(int i = 0; i < 11; i++)
    {
        b[i] = a[i];
    }

    return 0;

}