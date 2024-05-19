#ifndef PULSADOR_H_
#define PULSADOR_H_

/*==================[Inclusiones]============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

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
}pulsadorInfo;

/*==================[Prototipos de funciones]======================*/

/**
 * @brief Funcion para inicializar el Pulsador.
 * 
 */
void inicializarPulsador(void);

/**
 * @brief Funcion para obtener los Ticks que representan el Duty Cycle de la señal y el tiempo que se mantuvo presionado el pulsador.
 * 
 * @param indice 
 * @param periodo 
 * @return TickType_t 
 */
TickType_t obtenerDiferencia(uint8_t indice, TickType_t periodo);

#endif