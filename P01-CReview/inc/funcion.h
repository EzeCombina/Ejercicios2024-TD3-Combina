#ifndef FUNCION_H
#define FUNCION_H

#define CantidadMediciones 10

extern int TempMax;
extern float Prom;

/**
 * @brief Funcion para obtener la Temperatura Max. y el promedio de la Humedad 
 * 
 */
void Funcion();

struct Medicion {

    float Humedad;
    float VelocidadViento;
    int Temperatura;

}x[CantidadMediciones];

#endif