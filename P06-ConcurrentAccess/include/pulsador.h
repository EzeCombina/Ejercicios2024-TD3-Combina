#ifndef PULSADOR_H_
#define PULSADOR_H_

/*==================[Inclusiones]============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"    // Libreria para el manejo de semaforos 

/*==================[Definiciones]===================================*/
#define TIEMPO_NO_VALIDO    0
#define PULSADOR1           0
#define PULSADOR2           1
#define N_PULSADOR          2   //Cantidad de pulsadores

typedef enum
{
    ALTO,
    BAJO,
    DESCENDENTE,
    ASCENDENTE
}estadoPulsador;

typedef struct
{
    gpio_int_type_t tecla;
    estadoPulsador estado;          // Variables
    //SemaphoreHandle_t semaforo;     // Puntero que apunta al semaforo 
}pulsadorInfo;

/*==================[Prototipos de funciones]======================*/

/**
 * @brief Funcion para inicializar el Pulsador.
 * 
 */
void inicializarPulsador(void);

#endif