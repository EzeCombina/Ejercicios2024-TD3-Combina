#include <stdio.h>

// Definicion de la estructura con los miembros correspondientes
struct Complejo
{
    float Real;
    float Imaginario;
}x, *px; // Declaracion de las variables 

int main(void)
{

    // Se la da valor a las variables 
    x.Real = 1.3;           // || struct Complejo x = {1.3, -2.2};
    x.Imaginario = -2.2;    // || struct Complejo *px;

    // Referenciar el puntero a la variable x 
    px = &x;

    // Expresiones para los miembros de la estructura en términos de la variable puntero
    px->Real += 5;
    px->Imaginario -= 5;

    printf("El numero final es -> %.2f + (%.2f)i", px->Real, px->Imaginario);

    return 0;

}