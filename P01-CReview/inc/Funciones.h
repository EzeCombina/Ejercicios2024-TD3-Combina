#ifndef FUNCIONES_H
#define FUNCIONES_H    

#define CantidadMediciones 10

struct Medicion {

    float Humedad;
    float VelocidadViento;
    int Temperatura;

};

void Funcion(struct Medicion a[], int **TempMax, float **Prom);

#endif