#ifndef PULSADOR_H_
#define PULSADOR_H_

/*==================[Inclusiones]============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"    // Libreria para el manejo de semaforos 

/*==================[Definiciones]===================================*/
#define TIEMPO_NO_VALIDO    0
#define N_PULSADOR          1   //Cantidad de pulsadores

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
    TickType_t tiempoBajo;		    // Tiempo de la última transición del estado alto a bajo
    TickType_t tiempoAlto;		    // Tiempo de la última transición del estado bajo a alto
    TickType_t diferenciaTiempo;	// Variables
    SemaphoreHandle_t semaforo;     // Puntero que apunta al semaforo 
}pulsadorInfo;

/*==================[Prototipos de funciones]======================*/

/**
 * @brief Funcion para inicializar el Pulsador.
 * 
 */
void inicializarPulsador(void);

#endif