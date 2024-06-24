#ifndef PULSADOR_H_
#define PULSADOR_H_

/*==================[Inclusiones]============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"    // Libreria para el manejo de semaforos 
#include "freertos/queue.h"     // Libreria para el manejo de colas 

/*==================[Definiciones]===================================*/
#define TIEMPO_NO_VALIDO    0
#define N_PULSADOR          2      // Cantidad de pulsadores
#define N_LED               2      // Cantidad de leds 

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
    QueueHandle_t cola;             // Puntero que apunta a la cola  
}pulsadorInfo;

typedef struct
{
    uint8_t indice;
    TickType_t diferenciaTiempo;	// Variables
}ledInfo;

/*==================[Prototipos de funciones]======================*/

/**
 * @brief Funcion para inicializar el Pulsador.
 * 
 */
void inicializarPulsador(void);

/**
 * @brief Funcion para inicializar los leds.
 * 
 */
void inicializarLed(void);

#endif